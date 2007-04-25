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

  if( !m_Negated )
    {
    // we will keep the values from the feature image if the same pixel in the label image
    // equals the label given by the user. The other pixels are set to the background value.
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
  else
    {
    // we will keep the pixels from the feature image if the same pixel from the label image
    // is not equal to the label provided by the user. The pixels with the label provided by the
    // user are set to the background value
    if( input->GetUseBackground() && input->GetBackgroundValue() == m_Label )
      {

      // fill the image with the background value
      output->FillBuffer( m_BackgroundValue );

      // and copy the feature image where the label objects are
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
            output->SetPixel( idx, input2->GetPixel( idx ) );
            idx[0]++;
  //           progress.CompletedPixel();
            }
          }
        }
      }
    else
      {
      // copy the feature image to the output image
      ImageRegionConstIterator< OutputImageType > featureIt( input2, output->GetRequestedRegion() );
      ImageRegionIterator< OutputImageType > outputIt( output, output->GetRequestedRegion() );
  
      for ( featureIt.GoToBegin(), outputIt.GoToBegin();
            !featureIt.IsAtEnd();
            ++featureIt, ++outputIt )
        {
        outputIt.Set( featureIt.Get() );
        }

      // and mark the label object as background
      const typename InputImageType::LabelObjectType * labelObject = input->GetLabelObject( m_Label );
    
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
    //       progress.CompletedPixel();
          }
        }
      }
    }
}

}// end namespace itk
#endif
