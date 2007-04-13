/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelCollectionImageToMaskImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelCollectionImageToMaskImageFilter_txx
#define __itkLabelCollectionImageToMaskImageFilter_txx

#include "itkLabelCollectionImageToMaskImageFilter.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"

namespace itk {

template <class TInputImage, class TOutputImage>
LabelCollectionImageToMaskImageFilter<TInputImage, TOutputImage>
::LabelCollectionImageToMaskImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
  m_Label = NumericTraits< InputImagePixelType >::max();
  m_BackgroundValue = NumericTraits< OutputImagePixelType >::Zero;
}

template <class TInputImage, class TOutputImage>
void 
LabelCollectionImageToMaskImageFilter<TInputImage, TOutputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  if ( !input )
    { return; }
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );

  OutputImagePointer input2 = this->GetFeatureImage();
  if ( !input2 )
    { return; }
  input2->SetRequestedRegion( input2->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage>
void 
LabelCollectionImageToMaskImageFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage>
void
LabelCollectionImageToMaskImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  OutputImageType * output = this->GetOutput();
  InputImageType * input = const_cast<InputImageType *>(this->GetInput());
  const OutputImageType * input2 = this->GetFeatureImage();
  ProgressReporter progress( this, 0, 1 );

  if( input->GetUseBackground() && input->GetBackgroundValue() == m_Label )
    {
    // the user want the mask to be the background of the label collection image
    // copy the feature image to the output image
    ImageRegionConstIterator< OutputImageType > featureIt( input2, output->GetRequestedRegion() );
    ImageRegionIterator< OutputImageType > outputIt( output, output->GetRequestedRegion() );

    for ( featureIt.GoToBegin(), outputIt.GoToBegin();
          !featureIt.IsAtEnd();
          ++featureIt, ++outputIt )
      {
      outputIt.Set( featureIt.Get() );
      }
    
    // and mark the pixels from the label objects with the background value
    typename InputImageType::LabelObjectContainerType::const_iterator it;
    const typename InputImageType::LabelObjectContainerType & labelObjectContainer = input->GetLabelObjectContainer();
    for( it = labelObjectContainer.begin(); it != labelObjectContainer.end(); it++ )
      {
      const typename InputImageType::LabelObjectType * labeObject = it->second;
//       const typename InputImageType::LabelType & label = labeObject->GetLabel();
  
      typename InputImageType::LabelObjectType::LineContainerType::const_iterator lit;
      typename InputImageType::LabelObjectType::LineContainerType lineContainer = labeObject->GetLineContainer();
  
      for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
        {
        IndexType idx = lit->GetIndex();
        unsigned long length = lit->GetLength();
        for( int i=0; i<length; i++)
          {
          output->SetPixel( idx, m_BackgroundValue );
          idx[0]++;
//           progress.CompletedPixel();
          }
        }
      }
    }
  else
    {
    output->FillBuffer( m_BackgroundValue );
  
    const typename InputImageType::LabelObjectType * labelObject = input->GetLabelObject( m_Label );
  
    typename InputImageType::LabelObjectType::LineContainerType::const_iterator lit;
    typename InputImageType::LabelObjectType::LineContainerType lineContainer = labelObject->GetLineContainer();
  
    for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
      {
      IndexType idx = lit->GetIndex();
      unsigned long length = lit->GetLength();
      for( int i=0; i<length; i++)
        {
        output->SetPixel( idx, input2->GetPixel( idx ) );
        idx[0]++;
  //       progress.CompletedPixel();
        }
      }
    }
}

}// end namespace itk
#endif
