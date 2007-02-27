/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelCollectionImageToBinaryImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelCollectionImageToBinaryImageFilter_txx
#define __itkLabelCollectionImageToBinaryImageFilter_txx

#include "itkLabelCollectionImageToBinaryImageFilter.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

namespace itk {

template <class TInputImage, class TOutputImage>
LabelCollectionImageToBinaryImageFilter<TInputImage, TOutputImage>
::LabelCollectionImageToBinaryImageFilter()
{
  m_BackgroundValue = NumericTraits<OutputImagePixelType>::NonpositiveMin();
  m_ForegroundValue = NumericTraits<OutputImagePixelType>::max();
}

template <class TInputImage, class TOutputImage>
void 
LabelCollectionImageToBinaryImageFilter<TInputImage, TOutputImage>
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
LabelCollectionImageToBinaryImageFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage>
void
LabelCollectionImageToBinaryImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  OutputImageType * output = this->GetOutput();
  const InputImageType * input = this->GetInput();
  ProgressReporter progress( this, 0, output->GetRequestedRegion().GetNumberOfPixels() );

  if( input->GetUseBackground() )
    {
    // fill the output with background value - they will be overiden with the foreground value
    // later, if there is some objects
    if( this->GetNumberOfInputs() == 2 )
      {
      if ( this->GetBackgroundImage()->GetRequestedRegion().GetSize() != this->GetInput()->GetRequestedRegion().GetSize() )
        {
        itkExceptionMacro( << "Background and input images must have the same size." );
        }
      // fill the background with the background values from the background image
      ImageRegionConstIterator< OutputImageType > bgIt( this->GetBackgroundImage(), output->GetRequestedRegion() );
      ImageRegionIterator< OutputImageType > oIt( output, output->GetRequestedRegion() );
      for( oIt.GoToBegin(), bgIt.GoToBegin();
        !oIt.IsAtEnd();
        ++oIt, ++bgIt )
        {
        const OutputImagePixelType & bg = bgIt.Get();
        if( bg != m_ForegroundValue )
          {
          oIt.Set( bg );
          }
        else
          {
          oIt.Set( m_BackgroundValue );
          }
        }
      }
    else
      {
      // use the background value
      output->FillBuffer( m_BackgroundValue );
      }
    }
  else
    {
    // fill the output buffer with the foreground value and exit
    output->FillBuffer( m_ForegroundValue );
    return;
    }

  typename InputImageType::LabelObjectContainerType::const_iterator it;
  const typename InputImageType::LabelObjectContainerType & labelObjectContainer = input->GetLabelObjectContainer();
  for( it = labelObjectContainer.begin(); it != labelObjectContainer.end(); it++ )
    {
    const typename InputImageType::LabelObjectType * labeObject = it->second;
    // const typename InputImageType::LabelType & label = labeObject->GetLabel();

    typename InputImageType::LabelObjectType::LineContainerType::const_iterator lit;
    typename InputImageType::LabelObjectType::LineContainerType lineContainer = labeObject->GetLineContainer();

    for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
      {
      IndexType idx = lit->GetIndex();
      unsigned long length = lit->GetLength();
      for( int i=0; i<length; i++)
        {
        output->SetPixel( idx, m_ForegroundValue );
        idx[0]++;
        progress.CompletedPixel();
        }
      }
    }
}

}// end namespace itk
#endif
