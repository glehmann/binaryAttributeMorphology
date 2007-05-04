/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryReconstructionByErosionImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2006/08/01 19:16:18 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryReconstructionByErosionImageFilter_txx
#define __itkBinaryReconstructionByErosionImageFilter_txx

#include "itkBinaryReconstructionByErosionImageFilter.h"
#include "itkProgressAccumulator.h"


namespace itk {

template<class TInputImage>
BinaryReconstructionByErosionImageFilter<TInputImage>
::BinaryReconstructionByErosionImageFilter()
{
  m_BackgroundValue = NumericTraits<OutputImagePixelType>::NonpositiveMin();
  m_ForegroundValue = NumericTraits<OutputImagePixelType>::max();
  m_FullyConnected = false;
  this->SetNumberOfRequiredInputs(2);
}

template<class TInputImage>
void 
BinaryReconstructionByErosionImageFilter<TInputImage>
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
BinaryReconstructionByErosionImageFilter<TInputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage>
void
BinaryReconstructionByErosionImageFilter<TInputImage>
::GenerateData()
{
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Allocate the output
  this->AllocateOutputs();
  
  typename NotType::Pointer notMask = NotType::New();
  notMask->SetInput( this->GetMaskImage() );
  notMask->SetForegroundValue( m_ForegroundValue );
  notMask->SetBackgroundValue( m_BackgroundValue );
  progress->RegisterInternalFilter(notMask, .1f);
  notMask->Update();
  
  typename NotType::Pointer notMarker = NotType::New();
  notMarker->SetInput( this->GetMarkerImage() );
  notMarker->SetForegroundValue( m_ForegroundValue );
  notMarker->SetBackgroundValue( m_BackgroundValue );
  progress->RegisterInternalFilter(notMarker, .1f);
  notMarker->Update();
  
  typename LabelizerType::Pointer labelizer = LabelizerType::New();
  labelizer->SetInput( notMask->GetOutput() );
  labelizer->SetForegroundValue( m_ForegroundValue );
  labelizer->SetBackgroundValue( m_BackgroundValue );
  labelizer->SetFullyConnected( m_FullyConnected );
  progress->RegisterInternalFilter(labelizer, .3f);
  
  typename ReconstructionType::Pointer reconstruction = ReconstructionType::New();
  reconstruction->SetInput( labelizer->GetOutput() );
  reconstruction->SetMarkerImage( notMarker->GetOutput() );
  reconstruction->SetForegroundValue( m_ForegroundValue );
  progress->RegisterInternalFilter(reconstruction, .3f);
  
  // invert the image during the binarization
  typename BinarizerType::Pointer binarizer = BinarizerType::New();
  binarizer->SetInput( reconstruction->GetOutput() );
  binarizer->SetLabel( m_BackgroundValue );
  binarizer->SetNegated( true );
  binarizer->SetBackgroundValue( m_ForegroundValue );
  binarizer->SetFeatureImage( this->GetMaskImage() );
  progress->RegisterInternalFilter(binarizer, .2f);

  binarizer->GraftOutput( this->GetOutput() );
  binarizer->Update();
  this->GraftOutput( binarizer->GetOutput() );
}


template<class TInputImage>
void
BinaryReconstructionByErosionImageFilter<TInputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
  os << indent << "BackgroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
  os << indent << "ForegroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_ForegroundValue) << std::endl;
}
  
}// end namespace itk
#endif
