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
  m_Crop = false;
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
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();

  if( m_Crop )
    {
    const InputImageType * input = this->GetInput();

    if( !(input->GetMTime() > m_CropTimeStamp) && !(this->GetMTime() > m_CropTimeStamp) )
      {
      // early exit, crop sizes already computed
      // std::cout << "Don't recompute the output size again." << std::endl;
      return;
      }

    if( input->GetSource())
      {
      ProcessObject * upstream = input->GetSource();
      if (upstream)
        {
        this->SetInput(NULL);
        upstream->Update();
        this->SetInput(input);
        }
      }

    // Prefetch image region and size
    InputImageRegionType region = input->GetLargestPossibleRegion();
    SizeType size = region.GetSize();

    // now the output image size can be computed
    if( m_Negated )
      {
      if( input->GetUseBackground() && input->GetBackgroundValue() != m_Label )
        {
        // the "bad" case - the zone outside the object is at least partially
        // covered by the background, which is not explicitely defined.
      
        // simply do nothing for now
        // TODO: implement that part
        std::cerr << "Warning: Cropping according to background label is no yet implemented." << std::endl;
        std::cerr << "Warning: The full image will be used." << std::endl;
      
        }
      else
        {
        // compute the bounding box of all the objects which don't have that label
        IndexType mins;
        mins.Fill( NumericTraits< long >::max() );
        IndexType maxs;
        maxs.Fill( NumericTraits< long >::NonpositiveMin() );
        typename InputImageType::LabelObjectContainerType container = this->GetInput()->GetLabelObjectContainer();
        for( typename InputImageType::LabelObjectContainerType::const_iterator loit = container.begin();
             loit != container.end();
             loit++ )
          {
          if( loit->first != m_Label )
            {
            typename LabelObjectType::LineContainerType::const_iterator lit;
            typename LabelObjectType::LineContainerType lineContainer = loit->second->GetLineContainer();
            // iterate over all the lines
            for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
              {
              const IndexType & idx = lit->GetIndex();
              unsigned long length = lit->GetLength();
    
              // update the mins and maxs
              for( int i=0; i<ImageDimension; i++)
                {
                if( idx[i] < mins[i] )
                  {
                  mins[i] = idx[i];
                  }
                if( idx[i] > maxs[i] )
                  {
                  maxs[i] = idx[i];
                  }
                }
              // must fix the max for the axis 0
              if( idx[0] + length > maxs[0] )
                {
                maxs[0] = idx[0] + length - 1;
                }
              }
            }
          }
          
          // final computation
          SizeType regionSize;
          for( int i=0; i<ImageDimension; i++ )
            {
            regionSize[i] = maxs[i] - mins[i] + 1;
            }
          RegionType region( mins, regionSize );
          // std::cout << region << std::endl;
          
          // finally set that region as the largest output region
          this->GetOutput()->SetLargestPossibleRegion( region );

        }
      }
    else
      {
      if( input->GetUseBackground() && input->GetBackgroundValue() == m_Label )
        {
        // the other "bad" case - the label we want is not defined as a label object,
        // but implicitely, in the zones not defined.
      
        // simply do nothing for now
        // TODO: implement that part
        std::cerr << "Warning: Cropping according to background label is no yet implemented." << std::endl;
        std::cerr << "Warning: The full image will be used." << std::endl;
        
        }
      else
        {
        // just find the bounding box of the object with that label
        
        const LabelObjectType * labelObject = input->GetLabelObject( m_Label );
        typename LabelObjectType::LineContainerType::const_iterator lit;
        typename LabelObjectType::LineContainerType lineContainer = labelObject->GetLineContainer();
        IndexType mins;
        mins.Fill( NumericTraits< long >::max() );
        IndexType maxs;
        maxs.Fill( NumericTraits< long >::NonpositiveMin() );
        // iterate over all the lines
        for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
          {
          const IndexType & idx = lit->GetIndex();
          unsigned long length = lit->GetLength();

          // update the mins and maxs
          for( int i=0; i<ImageDimension; i++)
            {
            if( idx[i] < mins[i] )
              {
              mins[i] = idx[i];
              }
            if( idx[i] > maxs[i] )
              {
              maxs[i] = idx[i];
              }
            }
          // must fix the max for the axis 0
          if( idx[0] + length > maxs[0] )
            {
            maxs[0] = idx[0] + length - 1;
            }
          }
          // final computation
          SizeType regionSize;
          for( int i=0; i<ImageDimension; i++ )
            {
            regionSize[i] = maxs[i] - mins[i] + 1;
            }
          RegionType region( mins, regionSize );
          // std::cout << region << std::endl;
          
          // finally set that region as the largest output region
          this->GetOutput()->SetLargestPossibleRegion( region );

        }
      }

    m_CropTimeStamp.Modified();
    }
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
  if( ( input->GetUseBackground() && input->GetBackgroundValue() == m_Label ) ^ m_Negated )
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
        
        // should we take care to not write outside the image ?
        bool testIdxIsInside = m_Crop && ( input->GetUseBackground() && input->GetBackgroundValue() == m_Label ) ^ m_Negated;
        RegionType outputRegion = output->GetLargestPossibleRegion();

        typename InputImageType::LabelObjectType::LineContainerType::const_iterator lit;
        typename InputImageType::LabelObjectType::LineContainerType lineContainer = labelObject->GetLineContainer();
      
        for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
          {
          IndexType idx = lit->GetIndex();
          unsigned long length = lit->GetLength();
          for( int i=0; i<length; i++)
            {
            if( !testIdxIsInside || outputRegion.IsInside( idx ) )
              {
              output->SetPixel( idx, m_BackgroundValue );
              }
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

    // should we take care to not write outside the image ?
    bool testIdxIsInside = m_Crop && ( input->GetUseBackground() && input->GetBackgroundValue() == m_Label ) ^ m_Negated;
    RegionType outputRegion = output->GetLargestPossibleRegion();

    // the user want the mask to be the background of the label collection image
    typename InputImageType::LabelObjectType::LineContainerType::const_iterator lit;
    typename InputImageType::LabelObjectType::LineContainerType lineContainer = labelObject->GetLineContainer();

    for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
      {
      IndexType idx = lit->GetIndex();
      unsigned long length = lit->GetLength();
      for( int i=0; i<length; i++)
        {
        if( !testIdxIsInside || outputRegion.IsInside( idx ) )
          {
          output->SetPixel( idx, m_BackgroundValue );
          }
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


template<class TInputImage, class TOutputImage>
void
LabelCollectionImageToMaskImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << indent << "Label: " << static_cast<typename NumericTraits<LabelType>::PrintType>(m_Label) << std::endl;
  os << indent << "BackgroundValue: " << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
  os << indent << "Negated: " << m_Negated << std::endl;
  os << indent << "Crop: " << m_Crop << std::endl;
}


}// end namespace itk
#endif
