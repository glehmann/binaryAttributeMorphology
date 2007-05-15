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
  m_Negated = false;
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
}


template <class TInputImage, class TOutputImage>
void 
LabelCollectionImageToMaskImageFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage>
void 
LabelCollectionImageToMaskImageFilter<TInputImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  m_Barrier = Barrier::New();
  m_Barrier->Initialize( this->GetNumberOfThreads() );

  Superclass::BeforeThreadedGenerateData();

}


template <class TInputImage, class TOutputImage>
void 
LabelCollectionImageToMaskImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData( const OutputImageRegionType& outputRegionForThread, int threadId )
{
  OutputImageType * output = this->GetOutput();
  InputImageType * input = const_cast<InputImageType *>(this->GetInput());
  const OutputImageType * input2 = this->GetFeatureImage();

  // we will keep the values from the feature image if the same pixel in the label image
  // equals the label given by the user. The other pixels are set to the background value.
  if( input->GetUseBackground() && input->GetBackgroundValue() == m_Label )
    {
    // the user want the mask to be the background of the label collection image
    // copy the feature image to the output image
    ImageRegionConstIterator< OutputImageType > featureIt( input2, outputRegionForThread );
    ImageRegionIterator< OutputImageType > outputIt( output, outputRegionForThread );

    for ( featureIt.GoToBegin(), outputIt.GoToBegin();
          !featureIt.IsAtEnd();
          ++featureIt, ++outputIt )
      {
      outputIt.Set( featureIt.Get() );
      }
    }
  else
    {
    ImageRegionIterator< OutputImageType > outputIt( output, outputRegionForThread );

    for ( outputIt.GoToBegin(); !outputIt.IsAtEnd(); ++outputIt )
      {
      outputIt.Set( m_BackgroundValue );
      }
    }

  // wait for the other threads to complete that part
  m_Barrier->Wait();

  if( input->GetUseBackground() && input->GetBackgroundValue() == m_Label )
    {
    // and delegate to the superclass implementation to use the thread support for the label objects
    Superclass::ThreadedGenerateData( outputRegionForThread, threadId );
    }
  else
    {
    // need only one thread - take the first one
    if( threadId == 0 )
      {
      const LabelObjectType * labelObject = this->GetLabelCollectionImage()->GetLabelObject( m_Label );
      ProgressReporter progress( this, 0, labelObject->GetLineContainer().size() );

      if( !m_Negated )
        {
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
            }
          progress.CompletedPixel();
          }
        }
      else
        {
        // and mark the label object as background
        typename InputImageType::LabelObjectType::LineContainerType::const_iterator lit;
        typename InputImageType::LabelObjectType::LineContainerType lineContainer = labelObject->GetLineContainer();
      
        for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
          {
          IndexType idx = lit->GetIndex();
          unsigned long length = lit->GetLength();
          for( int i=0; i<length; i++)
            {
            output->SetPixel( idx, m_BackgroundValue );
            idx[0]++;
            }
          progress.CompletedPixel();
          }
        }

      }
    }
}


template<class TInputImage, class TOutputImage>
void
LabelCollectionImageToMaskImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData( LabelObjectType * labelObject )
{
  OutputImageType * output = this->GetOutput();
  InputImageType * input = const_cast<InputImageType *>(this->GetInput());
  const OutputImageType * input2 = this->GetFeatureImage();

  if( !m_Negated )
    {
    // we will keep the values from the feature image if the same pixel in the label image
    // equals the label given by the user. The other pixels are set to the background value.

    // the user want the mask to be the background of the label collection image
    typename InputImageType::LabelObjectType::LineContainerType::const_iterator lit;
    typename InputImageType::LabelObjectType::LineContainerType lineContainer = labelObject->GetLineContainer();

    for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
      {
      IndexType idx = lit->GetIndex();
      unsigned long length = lit->GetLength();
      for( int i=0; i<length; i++)
        {
        output->SetPixel( idx, m_BackgroundValue );
        idx[0]++;
        }
      }
    }
  else
    {
    // we will keep the pixels from the feature image if the same pixel from the label image
    // is not equal to the label provided by the user. The pixels with the label provided by the
    // user are set to the background value

    // and copy the feature image where the label objects are
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
        }
      }
    }
}

}// end namespace itk
#endif
