/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryAttributeOpeningImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/08/01 19:16:18 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryAttributeOpeningImageFilter_txx
#define __itkBinaryAttributeOpeningImageFilter_txx

#include "itkBinaryAttributeOpeningImageFilter.h"
#include "itkProgressAccumulator.h"


namespace itk {

template<class TInputImage, class TLabelObject, class TLabelObjectValuator, class TAttributeAccessor>
BinaryAttributeOpeningImageFilter<TInputImage, TLabelObject, TLabelObjectValuator, TAttributeAccessor>
::BinaryAttributeOpeningImageFilter()
{
  m_BackgroundValue = NumericTraits<OutputImagePixelType>::NonpositiveMin();
  m_ForegroundValue = NumericTraits<OutputImagePixelType>::max();
  m_FullyConnected = false;
}

template<class TInputImage, class TLabelObject, class TLabelObjectValuator, class TAttributeAccessor>
void 
BinaryAttributeOpeningImageFilter<TInputImage, TLabelObject, TLabelObjectValuator, TAttributeAccessor>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  if( input )
    {
    input->SetRequestedRegion( input->GetLargestPossibleRegion() );
    }
}


template<class TInputImage, class TLabelObject, class TLabelObjectValuator, class TAttributeAccessor>
void 
BinaryAttributeOpeningImageFilter<TInputImage, TLabelObject, TLabelObjectValuator, TAttributeAccessor>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TLabelObject, class TLabelObjectValuator, class TAttributeAccessor>
void
BinaryAttributeOpeningImageFilter<TInputImage, TLabelObject, TLabelObjectValuator, TAttributeAccessor>
::GenerateData()
{
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Allocate the output
  this->AllocateOutputs();
  
  typename LabelizerType::Pointer labelizer = LabelizerType::New();
  labelizer->SetInput( this->GetInput() );
  labelizer->SetForegroundValue( m_ForegroundValue );
  labelizer->SetBackgroundLabel( m_BackgroundValue );
  labelizer->SetFullyConnected( m_FullyConnected );
  progress->RegisterInternalFilter(labelizer, .3f);
  
  typename LabelObjectValuatorType::Pointer valuator = LabelObjectValuatorType::New();
  valuator->SetInput( labelizer->GetOutput() );
  progress->RegisterInternalFilter(valuator, .3f);
  
  typename OpeningType::Pointer opening = OpeningType::New();
  opening->SetInput( valuator->GetOutput() );
  opening->SetLambda( m_Lambda );
  progress->RegisterInternalFilter(opening, .2f);
  
  typename BinarizerType::Pointer binarizer = BinarizerType::New();
  binarizer->SetInput( opening->GetOutput() );
  binarizer->SetForegroundValue( m_ForegroundValue );
  binarizer->SetBackgroundValue( m_BackgroundValue );
  progress->RegisterInternalFilter(binarizer, .2f);  

  binarizer->GraftOutput( this->GetOutput() );
  binarizer->Update();
  this->GraftOutput( binarizer->GetOutput() );
}


template<class TInputImage, class TLabelObject, class TLabelObjectValuator, class TAttributeAccessor>
void
BinaryAttributeOpeningImageFilter<TInputImage, TLabelObject, TLabelObjectValuator, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  //TODO
}
  
}// end namespace itk
#endif
