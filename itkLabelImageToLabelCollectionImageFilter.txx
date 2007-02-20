/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelImageToLabelCollectionImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelImageToLabelCollectionImageFilter_txx
#define __itkLabelImageToLabelCollectionImageFilter_txx

#include "itkLabelImageToLabelCollectionImageFilter.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkImageRegionConstIteratorWithIndex.h"

namespace itk {

template <class TInputImage, class TOutputImage>
LabelImageToLabelCollectionImageFilter<TInputImage, TOutputImage>
::LabelImageToLabelCollectionImageFilter()
{
  m_UseBackground = false;
  m_BackgroundLabel = NumericTraits<OutputImagePixelType>::NonpositiveMin();
}

template <class TInputImage, class TOutputImage>
void 
LabelImageToLabelCollectionImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  if ( !input )
    { return; }
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage>
void 
LabelImageToLabelCollectionImageFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage>
void
LabelImageToLabelCollectionImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  OutputImageType * output = this->GetOutput();

  output->SetUseBackground( m_UseBackground );
  output->SetBackgroundLabel( m_BackgroundLabel );

  ProgressReporter progress( this, 0, output->GetRequestedRegion().GetNumberOfPixels() );

  ImageRegionConstIteratorWithIndex< InputImageType > it( this->GetInput(), output->GetRequestedRegion() );

  for( it.GoToBegin(); !it.IsAtEnd(); ++it )
    {
    output->SetPixel( it.GetIndex(), it.Get() );
    progress.CompletedPixel();
    }

}


template<class TInputImage, class TOutputImage>
void
LabelImageToLabelCollectionImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "UseBackground: "  << m_UseBackground << std::endl;
  os << indent << "BackgroundLabel: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundLabel) << std::endl;
}
  
}// end namespace itk
#endif
