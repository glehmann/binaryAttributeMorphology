/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkObjectImageLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkObjectImageLabelMapFilter_txx
#define __itkObjectImageLabelMapFilter_txx

#include "itkObjectImageLabelMapFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkSignedMaurerDistanceMapImageFilter.h"
#include "itkMorphologicalWatershedImageFilter.h"
#include "itkMaskImageFilter.h"
#include "itkNumericTraits.h"

#include "itkLabelObject.h"
#include "itkLabelMap.h"
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "itkLabelMapToBinaryImageFilter.h"
#include "itkLabelMapToLabelImageFilter.h"
#include "itkLabelSelectionLabelMapFilter.h"
#include "itkAutoCropLabelMapFilter.h"
#include "itkLabelMapMaskImageFilter.h"

namespace itk {

template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter, class TInternalInputImageType, class TInternalOutputImageType>
ObjectImageLabelMapFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter, TInternalInputImageType, TInternalOutputImageType>
::ObjectImageLabelMapFilter()
{
  m_ConstrainPaddingToImage = true;
  m_PadSize.Fill(0);
  m_BinaryInternalOutput = true;
  m_KeepLabels = true;
  m_InternalForegroundValue = itk::NumericTraits< InternalOutputPixelType >::max();

  m_InputFilter = NULL;
  m_OutputFilter = NULL;
  
  
  m_Select = SelectType::New();
  // be sure to not use the label objects internally
  m_Select->SetInPlace( false );
  m_Select->SetNumberOfThreads( 1 );

  m_Crop = CropType::New();
  m_Crop->SetInput( m_Select->GetOutput() );
  m_Crop->SetNumberOfThreads( 1 );

  m_Pad = PadType::New();
  m_Pad->SetInput( m_Crop->GetOutput() );

  m_LM2BI = LM2BIType::New();
  m_LM2BI->SetInput( m_Pad->GetOutput() );
  m_LM2BI->SetNumberOfThreads( 1 );

  m_LI2LM = LI2LMType::New();
  m_LI2LM->SetNumberOfThreads( 1 );

  m_BI2LM = BI2LMType::New();
  m_BI2LM->SetNumberOfThreads( 1 );

}


template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter, class TInternalInputImageType, class TInternalOutputImageType>
void
ObjectImageLabelMapFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter, TInternalInputImageType, TInternalOutputImageType>
::SetFilter( InputFilterType * filter )
{
  OutputFilterType * outputFilter = dynamic_cast< OutputFilterType * >( filter );
  if( outputFilter == NULL && filter != NULL )
    {
    // TODO: can it be replaced by a concept check ?
    itkExceptionMacro("Wrong output filter type. Use SetOutputFilter() and SetInputFilter() instead of SetFilter() when input and output filter types are different.");
    }
  this->SetInputFilter( filter );
  this->SetOutputFilter( outputFilter );
}


template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter, class TInternalInputImageType, class TInternalOutputImageType>
void
ObjectImageLabelMapFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter, TInternalInputImageType, TInternalOutputImageType>
::SetInputFilter( InputFilterType * filter )
{
  if( m_InputFilter.GetPointer() != filter )
    {
    this->Modified();
    m_InputFilter = filter;
    // adapt the number of inputs and outputs
    this->SetNumberOfRequiredInputs( filter->GetNumberOfValidRequiredInputs() );
    }
}


template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter, class TInternalInputImageType, class TInternalOutputImageType>
void
ObjectImageLabelMapFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter, TInternalInputImageType, TInternalOutputImageType>
::SetOutputFilter( OutputFilterType * filter )
{
  if( m_OutputFilter.GetPointer() != filter )
    {
    this->Modified();
    m_OutputFilter = filter;
    // adapt the number of inputs and outputs
    this->SetNumberOfRequiredOutputs( filter->GetNumberOfOutputs() );
    }
}


template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter, class TInternalInputImageType, class TInternalOutputImageType>
void
ObjectImageLabelMapFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter, TInternalInputImageType, TInternalOutputImageType>
::GenerateData()
{

  this->AllocateOutputs();
  LabelMapType * output = this->GetOutput();
  output->SetBackgroundValue( this->GetInput()->GetBackgroundValue() );
  output->ClearLabels();
    
  // give the input bg value as default bg value to the output
  this->GetOutput()->SetBackgroundValue( this->GetInput()->GetBackgroundValue() );
  
  // set the input image of the first filter of our internal pipeline
  m_Select->SetInput( this->GetInput() );
  
  // configure the pipeline to produce a constrained border or not
  if( m_ConstrainPaddingToImage )
    {
    m_Crop->SetCropBorder( m_PadSize );
    SizeType zero;
    zero.Fill( 0 );
    m_Pad->SetPadSize( zero );
    }
  else
    {
    SizeType zero;
    zero.Fill( 0 );
    m_Crop->SetCropBorder( zero );
    m_Pad->SetPadSize( m_PadSize );
    }
    
  // plug the pipeline provided by the user and our internal one
  m_InputFilter->SetInput( m_LM2BI->GetOutput() );
  m_LI2LM->SetInput( m_OutputFilter->GetOutput() );
  m_BI2LM->SetInput( m_OutputFilter->GetOutput() );

  m_BI2LM->SetForegroundValue( m_InternalForegroundValue );
  m_LM2BI->SetForegroundValue( m_InternalForegroundValue );

  // initialize the progress reporter
  // TODO: really report the progress!
  ProgressReporter progress( this, 0, this->GetLabelMap()->GetNumberOfLabelObjects() );

  // initialize the iterator
  typename InputImageType::LabelObjectContainerType::const_iterator loIterator = this->GetLabelMap()->GetLabelObjectContainer().begin();

  while( loIterator != this->GetLabelMap()->GetLabelObjectContainer().end() )
    {
    // select our object
    m_Select->SetLabel( loIterator->first );
    // TODO: remove the next line - it shouldn't be required
    m_Crop->Modified();

    // to store the label objects
    LabelMapType * labelMap;

    // update the pipeline
    if( m_BinaryInternalOutput )
      {
      m_BI2LM->UpdateLargestPossibleRegion();
      labelMap = m_BI2LM->GetOutput();
      }
    else
      {
      m_LI2LM->UpdateLargestPossibleRegion();
      labelMap = m_LI2LM->GetOutput();
      }
    std::cout << "label: " << loIterator->first + 0.0 << "  " << loIterator->second->GetLabel() + 0.0 << std::endl;
    
    // stole the label objects from the last filter of the pipeline
    if( m_KeepLabels )
      {
      typename LabelMapType::LabelObjectContainerType & labelObjectContainer2 = labelMap->GetLabelObjectContainer();
      typename LabelMapType::LabelObjectContainerType::iterator it2 = labelObjectContainer2.begin();
      if( it2 != labelObjectContainer2.end() )
        {
        LabelObjectType * lo = it2->second;
        if( output->HasLabel( loIterator->first ) )
          {
          // the label has been stolen by a previously splitted object. Just move than object elsewhere
          // to free the label
          typename LabelObjectType::Pointer lotmp = output->GetLabelObject( loIterator->first );
          output->RemoveLabelObject( lotmp );
          lo->SetLabel( loIterator->first );
          output->AddLabelObject( lo );
          output->PushLabelObject( lotmp );
          }
        else
          {
          lo->SetLabel( loIterator->first );
          output->AddLabelObject( lo );
          }
          
        // then push the other objects
        it2++;
        while( it2 != labelObjectContainer2.end() )
          {
          output->PushLabelObject( it2->second );
          it2++;
          }
        }
      else
        {
        std::cout << "no result!" << std::endl;
        }
      }
    else
      {
      typename LabelMapType::LabelObjectContainerType & labelObjectContainer2 = labelMap->GetLabelObjectContainer();
      typename LabelMapType::LabelObjectContainerType::iterator it2 = labelObjectContainer2.begin();
      while( it2 != labelObjectContainer2.end() )
        {
        output->PushLabelObject( it2->second );
        it2++;
        }
      }
    
    // and proceed the next object
    loIterator++;
    
    }
output->PrintLabelObjects();
std::cout << "nb: " << output->GetNumberOfLabelObjects() << std::endl;

}


template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter, class TInternalInputImageType, class TInternalOutputImageType>
void
ObjectImageLabelMapFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter, TInternalInputImageType, TInternalOutputImageType>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

}
  
}// end namespace itk
#endif
