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
  m_BinaryInternalOutput = false;
  m_KeepLabels = true;
  m_InternalForegroundValue = itk::NumericTraits< InternalOutputPixelType >::max();

  m_InputFilter = NULL;
  m_OutputFilter = NULL;
  
  
  m_Select = SelectType::New();
  // be sure to *not* use the label objects internally
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
    }
}


template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter, class TInternalInputImageType, class TInternalOutputImageType>
void
ObjectImageLabelMapFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter, TInternalInputImageType, TInternalOutputImageType>
::GenerateData()
{

  this->AllocateOutputs();
  LabelMapType * output = this->GetOutput();
  // preserve the background value
  output->SetBackgroundValue( this->GetInput()->GetBackgroundValue() );
  output->ClearLabels();
    
  // give the input bg value as default bg value to the output
  this->GetOutput()->SetBackgroundValue( this->GetInput()->GetBackgroundValue() );
  
  // set the input image of the first filter of our internal pipeline
  m_Select->SetInput( this->GetInput() );
  
  // configure the pipeline to produce a constrained border or not.
  // auto crop filter "CropBorder" feature constrain the padding to the input image,
  // and the pad filter don't constrain it, so use one or the other
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
  // set the input to both kind of filters in charge of creating the label object
  // so we don't have to implement a condition
  m_LI2LM->SetInput( m_OutputFilter->GetOutput() );
  m_BI2LM->SetInput( m_OutputFilter->GetOutput() );

  m_LM2BI->SetForegroundValue( m_InternalForegroundValue );
  m_BI2LM->SetForegroundValue( m_InternalForegroundValue );

  // initialize the progress reporter
  // TODO: really report the progress!
  ProgressReporter progress( this, 0, this->GetLabelMap()->GetNumberOfLabelObjects() );

  // initialize the iterator
  typename InputImageType::LabelObjectContainerType::const_iterator loIterator = this->GetLabelMap()->GetLabelObjectContainer().begin();

  while( loIterator != this->GetLabelMap()->GetLabelObjectContainer().end() )
    {
    // select our object
    m_Select->SetLabel( loIterator->first );
    // TODO: remove the next line - it shouldn't be required. 
    // It seems to be a bug in the autocrop filter :-(
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
    // std::cout << "label: " << loIterator->first + 0.0 << "  " << loIterator->second->GetLabel() + 0.0 << std::endl;
    
    // stole the label objects from the last filter of the pipeline, to put them in the output
    // label map of the current filter
    if( m_KeepLabels )
      {
      // try to keep the current label. That's easy if a single object is produced.
      // If more than one is produced, it is pushed in the label map without trying
      // to get a specific label. If a label is already there, it means that a previous
      // object has stolen the label, so the label of the thief must be changed.
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
        // std::cout << "no result!" << std::endl;
        }
      }
    else
      {
      // don't try to preserve the label - simply push the label objects as they come
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
}


template <class TInputImage, class TOutputImage, class TInputFilter, class TOutputFilter, class TInternalInputImageType, class TInternalOutputImageType>
void
ObjectImageLabelMapFilter<TInputImage, TOutputImage, TInputFilter, TOutputFilter, TInternalInputImageType, TInternalOutputImageType>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ConstrainPaddingToImage: " << m_ConstrainPaddingToImage << std::endl;
  os << indent << "PadSize: " << m_PadSize << std::endl;
  os << indent << "BinaryInternalOutput: " << m_BinaryInternalOutput << std::endl;
  os << indent << "KeepLabels: " << m_KeepLabels << std::endl;
  os << indent << "InternalForegroundValue: " << m_InternalForegroundValue << std::endl;
  os << indent << "InputFilter: " << this->m_InputFilter->GetNameOfClass() 
     << " " << this->m_InputFilter.GetPointer() << std::endl;
  os << indent << "OutputFilter: " << this->m_OutputFilter->GetNameOfClass() 
     << " " << this->m_OutputFilter.GetPointer() << std::endl;

}
  
}// end namespace itk
#endif
