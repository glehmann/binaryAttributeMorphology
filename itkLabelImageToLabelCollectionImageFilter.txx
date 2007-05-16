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
  m_UseBackground = true;
  m_BackgroundValue = NumericTraits<OutputImagePixelType>::NonpositiveMin();
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
::BeforeThreadedGenerateData()
{
  // init the temp images - one per thread
  m_TemporaryImages.resize( this->GetNumberOfThreads() );

  for( int i=0; i<this->GetNumberOfThreads(); i++ )
    {
    if( i == 0 )
      {
      // the first one is the output image
      m_TemporaryImages[0] = this->GetOutput();
      }
    else
      {
      // the other must be created
      m_TemporaryImages[i] = OutputImageType::New();
      }
      
    // set the minimum data needed to create the objects properly
    m_TemporaryImages[i]->SetUseBackground( m_UseBackground );
    m_TemporaryImages[i]->SetBackgroundValue( m_BackgroundValue );

    }
}


template<class TInputImage, class TOutputImage>
void
LabelImageToLabelCollectionImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData( const OutputImageRegionType& regionForThread, int threadId )
{
  ProgressReporter progress( this, threadId, regionForThread.GetNumberOfPixels() );

  ImageRegionConstIteratorWithIndex< InputImageType > it( this->GetInput(), regionForThread );

  for( it.GoToBegin(); !it.IsAtEnd(); ++it )
    {
    m_TemporaryImages[threadId]->SetPixel( it.GetIndex(), it.Get() );
    progress.CompletedPixel();
    }

}


template<class TInputImage, class TOutputImage>
void
LabelImageToLabelCollectionImageFilter<TInputImage, TOutputImage>
::AfterThreadedGenerateData()
{

  OutputImageType * output = this->GetOutput();

  // merge the lines from the temporary images in the output image
  // don't use the first image - that's the output image
  for( int i=1; i<this->GetNumberOfThreads(); i++ )
    {
    typedef typename OutputImageType::LabelObjectContainerType LabelObjectContainerType;
    const LabelObjectContainerType & labelObjectContainer = m_TemporaryImages[i]->GetLabelObjectContainer();

    for( typename LabelObjectContainerType::const_iterator it = labelObjectContainer.begin();
      it != labelObjectContainer.end();
      it++ )
      {
      LabelObjectType * labelObject = it->second;
      if( output->HasLabel( labelObject->GetLabel() ) )
        {
        // merge the lines in the output's object
        typename LabelObjectType::LineContainerType & src = labelObject->GetLineContainer();
        typename LabelObjectType::LineContainerType & dest = output->GetLabelObject( labelObject->GetLabel() )->GetLineContainer();
        dest.insert( dest.end(), src.begin(), src.end() );
        }
      else
        {
        // simply take the object
        output->AddLabelObject( labelObject );
        }
      }
      
    }
    
  // release the data in the temp images
  m_TemporaryImages.clear();
  
}


template<class TInputImage, class TOutputImage>
void
LabelImageToLabelCollectionImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "UseBackground: "  << m_UseBackground << std::endl;
  os << indent << "BackgroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
}
  
}// end namespace itk
#endif
