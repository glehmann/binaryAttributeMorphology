/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryStatisticsKeepNObjectsImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/08/01 19:16:18 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryStatisticsKeepNObjectsImageFilter_txx
#define __itkBinaryStatisticsKeepNObjectsImageFilter_txx

#include "itkBinaryStatisticsKeepNObjectsImageFilter.h"
#include "itkProgressAccumulator.h"


namespace itk {

template<class TInputImage, class TFeatureImage>
BinaryStatisticsKeepNObjectsImageFilter<TInputImage, TFeatureImage>
::BinaryStatisticsKeepNObjectsImageFilter()
{
  m_BackgroundValue = NumericTraits<OutputImagePixelType>::NonpositiveMin();
  m_ForegroundValue = NumericTraits<OutputImagePixelType>::max();
  m_FullyConnected = false;
  m_ReverseOrdering = false;
  m_Attribute = LabelObjectType::MEAN;
  this->SetNumberOfRequiredInputs(2);
}

template<class TInputImage, class TFeatureImage>
void 
BinaryStatisticsKeepNObjectsImageFilter<TInputImage, TFeatureImage>
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


template<class TInputImage, class TFeatureImage>
void 
BinaryStatisticsKeepNObjectsImageFilter<TInputImage, TFeatureImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TFeatureImage>
void
BinaryStatisticsKeepNObjectsImageFilter<TInputImage, TFeatureImage>
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
  labelizer->SetBackgroundValue( m_BackgroundValue );
  labelizer->SetFullyConnected( m_FullyConnected );
  progress->RegisterInternalFilter(labelizer, .3f);
  
  typename LabelObjectValuatorType::Pointer valuator = LabelObjectValuatorType::New();
  valuator->SetInput( labelizer->GetOutput() );
  valuator->SetFeatureImage( this->GetFeatureImage() );
  progress->RegisterInternalFilter(valuator, .3f);
  
  typename KeepNObjectsType::Pointer opening = KeepNObjectsType::New();
  opening->SetInput( valuator->GetOutput() );
  opening->SetNumberOfObjects( m_NumberOfObjects );
  opening->SetReverseOrdering( m_ReverseOrdering );
  opening->SetAttribute( m_Attribute );
  progress->RegisterInternalFilter(opening, .2f);
  
  typename BinarizerType::Pointer binarizer = BinarizerType::New();
  binarizer->SetInput( opening->GetOutput() );
  binarizer->SetForegroundValue( m_ForegroundValue );
  binarizer->SetBackgroundValue( m_BackgroundValue );
  binarizer->SetBackgroundImage( this->GetInput() );
  progress->RegisterInternalFilter(binarizer, .2f);  

  binarizer->GraftOutput( this->GetOutput() );
  binarizer->Update();
  this->GraftOutput( binarizer->GetOutput() );
}


template<class TInputImage, class TFeatureImage>
void
BinaryStatisticsKeepNObjectsImageFilter<TInputImage, TFeatureImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
  os << indent << "BackgroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
  os << indent << "ForegroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_ForegroundValue) << std::endl;
  os << indent << "NumberOfObjects: "  << m_NumberOfObjects << std::endl;
  os << indent << "ReverseOrdering: "  << m_ReverseOrdering << std::endl;
  os << indent << "Attribute: "  << LabelObjectType::GetNameFromAttribute(m_Attribute) << " (" << m_Attribute << ")" << std::endl;
}
  
}// end namespace itk
#endif
