/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryReconstructionByDilationImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/08/01 19:16:18 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryReconstructionByDilationImageFilter_txx
#define __itkBinaryReconstructionByDilationImageFilter_txx

#include "itkBinaryReconstructionByDilationImageFilter.h"
#include "itkProgressAccumulator.h"


namespace itk {

template<class TInputImage>
BinaryReconstructionByDilationImageFilter<TInputImage>
::BinaryReconstructionByDilationImageFilter()
{
  m_BackgroundValue = NumericTraits<OutputImagePixelType>::NonpositiveMin();
  m_ForegroundValue = NumericTraits<OutputImagePixelType>::max();
  m_FullyConnected = false;
  this->SetNumberOfRequiredInputs(2);
}

template<class TInputImage>
void 
BinaryReconstructionByDilationImageFilter<TInputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetMarkerImage());
  if( input )
    {
    input->SetRequestedRegion( input->GetLargestPossibleRegion() );
    }

  input = const_cast<InputImageType *>(this->GetMaskImage());
  if( input )
    {
    input->SetRequestedRegion( input->GetLargestPossibleRegion() );
    }
}


template<class TInputImage>
void 
BinaryReconstructionByDilationImageFilter<TInputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage>
void
BinaryReconstructionByDilationImageFilter<TInputImage>
::GenerateData()
{
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Allocate the output
  this->AllocateOutputs();
  
  typename LabelizerType::Pointer labelizer = LabelizerType::New();
  labelizer->SetInput( this->GetMaskImage() );
  labelizer->SetForegroundValue( m_ForegroundValue );
  labelizer->SetBackgroundValue( m_BackgroundValue );
  labelizer->SetFullyConnected( m_FullyConnected );
  progress->RegisterInternalFilter(labelizer, .4f);
  
  typename ReconstructionType::Pointer reconstruction = ReconstructionType::New();
  reconstruction->SetInput( labelizer->GetOutput() );
  reconstruction->SetMarkerImage( this->GetMarkerImage() );
  reconstruction->SetForegroundValue( m_ForegroundValue );
  progress->RegisterInternalFilter(reconstruction, .3f);
  
  typename BinarizerType::Pointer binarizer = BinarizerType::New();
  binarizer->SetInput( reconstruction->GetOutput() );
  binarizer->SetForegroundValue( m_ForegroundValue );
  binarizer->SetBackgroundValue( m_BackgroundValue );
  binarizer->SetBackgroundImage( this->GetMaskImage() );
  progress->RegisterInternalFilter(binarizer, .3f);  

  binarizer->GraftOutput( this->GetOutput() );
  binarizer->Update();
  this->GraftOutput( binarizer->GetOutput() );
}


template<class TInputImage>
void
BinaryReconstructionByDilationImageFilter<TInputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
  os << indent << "BackgroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
  os << indent << "ForegroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_ForegroundValue) << std::endl;
}
  
}// end namespace itk
#endif
