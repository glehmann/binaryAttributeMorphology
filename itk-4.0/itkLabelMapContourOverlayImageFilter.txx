/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelMapContourOverlayImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelMapContourOverlayImageFilter_txx
#define __itkLabelMapContourOverlayImageFilter_txx

#include "itkLabelMapContourOverlayImageFilter.h"
#include "itkNumericTraits.h"
#include "itkProgressReporter.h"
#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkObjectByObjectLabelMapFilter.h"
#include "itkFlatStructuringElement.h"
#include "itkBinaryDilateImageFilter.h"
#include "itkBinaryErodeImageFilter.h"
#include "itkSubtractImageFilter.h"
#include "itkBinaryContourImageFilter.h"
#include "itkSliceBySliceImageFilter.h"
#include "itkLabelUniqueLabelMapFilter.h"


namespace itk {

template<class TInputImage, class TFeatureImage, class TOutputImage>
LabelMapContourOverlayImageFilter<TInputImage, TFeatureImage, TOutputImage>
::LabelMapContourOverlayImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
  m_Opacity = 0.5;
  m_Type = PLAIN;
  m_Priority = HIGH_LABEL_ON_TOP;
  SizeType s;
  s.Fill( 1 );
  m_ContourThickness = SizeType( s );
  s.Fill( 0 );
  m_DilationRadius = SizeType( s );
  m_SliceDimension = ImageDimension - 1;
}

template<class TInputImage, class TFeatureImage, class TOutputImage>
void 
LabelMapContourOverlayImageFilter<TInputImage, TFeatureImage, TOutputImage>
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

template <class TInputImage, class TFeatureImage, class TOutputImage>
void 
LabelMapContourOverlayImageFilter<TInputImage, TFeatureImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TFeatureImage, class TOutputImage>
void 
LabelMapContourOverlayImageFilter<TInputImage, TFeatureImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  typedef typename itk::ObjectByObjectLabelMapFilter< InputImageType, InputImageType > OBOType;
  typename OBOType::Pointer obo = OBOType::New();
  obo->SetInput( this->GetInput() );
  SizeType rad = m_DilationRadius;
  for( int i=0; i<ImageDimension; i++ )
    {
    rad[i]++;
    }
  obo->SetPadSize( rad );
  // obo->SetInPlace( false );
  
  // dilate the image
  typedef typename OBOType::InternalInputImageType InternalImageType;
  typedef typename itk::FlatStructuringElement< ImageDimension > KernelType;
  typedef typename itk::BinaryDilateImageFilter< InternalImageType, InternalImageType, KernelType > DilateType;
  typename DilateType::Pointer dilate = DilateType::New();
  dilate->SetKernel( KernelType::Ball( m_DilationRadius ) );
  obo->SetInputFilter( dilate );

//   typedef typename itk::CastImageFilter< InternalImageType, InternalImageType, KernelType > CastType;
//   typename CastType::Pointer cast = CastType::New();
//   cast->SetInPlace( false );

  typedef typename itk::BinaryErodeImageFilter< InternalImageType, InternalImageType, KernelType > ErodeType;
  typename ErodeType::Pointer erode = ErodeType::New();
  erode->SetKernel( KernelType::Ball( m_ContourThickness ) );
  erode->SetInput( dilate->GetOutput() );

  typedef typename itk::SubtractImageFilter< InternalImageType, InternalImageType > SubtractType;
  typename SubtractType::Pointer sub = SubtractType::New();
  sub->SetInput( 0, dilate->GetOutput() );
  sub->SetInput( 1, erode->GetOutput() );


  typedef typename itk::SliceBySliceImageFilter< InternalImageType, InternalImageType > SliceType;
  typedef typename SliceType::InternalInputImageType SliceInternalImageType;
  typename SliceType::Pointer slice = SliceType::New();

  typedef typename itk::CastImageFilter< SliceInternalImageType, SliceInternalImageType > SliceCastType;
  typename SliceCastType::Pointer scast = SliceCastType::New();
  scast->SetInPlace( false );
  slice->SetInputFilter( scast );

  typedef typename itk::FlatStructuringElement< ImageDimension - 1 > SliceKernelType;
  typedef typename itk::BinaryErodeImageFilter< SliceInternalImageType, SliceInternalImageType, SliceKernelType > SliceErodeType;
  typename SliceErodeType::Pointer serode = SliceErodeType::New();
  typename SliceKernelType::RadiusType srad;
  int j=0;
  for( int i=0; i<ImageDimension; i++ )
    {
    if( j != m_SliceDimension )
      {
      srad[j] = m_ContourThickness[i];
      j++;
      }
    }
  serode->SetKernel( SliceKernelType::Ball( srad ) );
  serode->SetInput( scast->GetOutput() );

  typedef typename itk::SubtractImageFilter< SliceInternalImageType, SliceInternalImageType > SliceSubtractType;
  typename SliceSubtractType::Pointer ssub = SliceSubtractType::New();
  ssub->SetInput( 0, scast->GetOutput() );
  ssub->SetInput( 1, serode->GetOutput() );
  slice->SetOutputFilter( ssub );

  // search the contour, or not
  if( m_Type == PLAIN )
    {
    // nothing to do
    obo->SetOutputFilter( dilate );
    }
  else if( m_Type == COUNTOUR )
    {
//     typedef typename itk::BinaryContourImageFilter< InternalImageType, InternalImageType > ContourType;
//     typename ContourType::Pointer contour = ContourType::New();
//     contour->SetInput( dilate->GetOutput() );
//     obo->SetOutputFilter( contour );
     obo->SetOutputFilter( sub );    
    }
  else if( m_Type == SLICE_CONTOUR )
    {
    slice->SetInput( dilate->GetOutput() );
    slice->SetDimension( m_SliceDimension );
    obo->SetOutputFilter( slice );
    
//     typedef typename SliceType::InternalInputImageType SliceInternalType;
//     typedef typename itk::BinaryContourImageFilter< SliceInternalType, SliceInternalType > SliceContourType;
//     typename SliceContourType::Pointer slice_contour = SliceContourType::New();
//     slice->SetFilter( slice_contour );
    }
  else
    {
    itkExceptionMacro(<< "Unsupported Type: " << m_Type);
    }
  
  // choose which labels will be on top of the oters
  typedef typename itk::LabelUniqueLabelMapFilter< InputImageType > UniqueType;
  typename UniqueType::Pointer uniq = UniqueType::New();
  uniq->SetInput( obo->GetOutput() );
  
  m_TempImage = uniq->GetOutput();
  m_TempImage->Update();
  m_TempImage->DisconnectPipeline();
  

  long nbOfThreads = this->GetNumberOfThreads();
  if( itk::MultiThreader::GetGlobalMaximumNumberOfThreads() != 0 )
    {
    nbOfThreads = std::min( this->GetNumberOfThreads(), itk::MultiThreader::GetGlobalMaximumNumberOfThreads() );
    }
  // number of threads can be constrained by the region size, so call the SplitRequestedRegion
  // to get the real number of threads which will be used
  typename TOutputImage::RegionType splitRegion;  // dummy region - just to call the following method
  nbOfThreads = this->SplitRequestedRegion(0, nbOfThreads, splitRegion);
  // std::cout << "nbOfThreads: " << nbOfThreads << std::endl;

  m_Barrier = Barrier::New();
  m_Barrier->Initialize( nbOfThreads );

  Superclass::BeforeThreadedGenerateData();

}


template<class TInputImage, class TFeatureImage, class TOutputImage>
void 
LabelMapContourOverlayImageFilter<TInputImage, TFeatureImage, TOutputImage>
::ThreadedGenerateData( const OutputImageRegionType& outputRegionForThread, int threadId )
{
  OutputImageType * output = this->GetOutput();
  InputImageType * input = const_cast<InputImageType *>(this->GetInput());
  const FeatureImageType * input2 = this->GetFeatureImage();

  FunctorType function;
  function.SetBackgroundValue( input->GetBackgroundValue() );
  function.SetOpacity( m_Opacity );
  
  ImageRegionConstIterator< FeatureImageType > featureIt( input2, outputRegionForThread );
  ImageRegionIterator< OutputImageType > outputIt( output, outputRegionForThread );

  for ( featureIt.GoToBegin(), outputIt.GoToBegin();
        !featureIt.IsAtEnd();
        ++featureIt, ++outputIt )
    {
    outputIt.Set( function( featureIt.Get(), input->GetBackgroundValue() ) );
    }

  // wait for the other threads to complete that part
  m_Barrier->Wait();

  // and delegate to the superclass implementation to use the thread support for the label objects
  Superclass::ThreadedGenerateData( outputRegionForThread, threadId );
}


template<class TInputImage, class TFeatureImage, class TOutputImage>
void
LabelMapContourOverlayImageFilter<TInputImage, TFeatureImage, TOutputImage>
::ThreadedProcessLabelObject( LabelObjectType * labelObject )
{
  OutputImageType * output = this->GetOutput();
  InputImageType * input = const_cast<InputImageType *>(this->GetInput());
  const FeatureImageType * input2 = this->GetFeatureImage();

  FunctorType function;
  function.SetBackgroundValue( input->GetBackgroundValue() );
  function.SetOpacity( m_Opacity );

  const typename LabelObjectType::LabelType & label = labelObject->GetLabel();

  // the user want the mask to be the background of the label collection image
  typename InputImageType::LabelObjectType::LineContainerType::const_iterator lit;
  typename InputImageType::LabelObjectType::LineContainerType & lineContainer = labelObject->GetLineContainer();

  for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
    {
    IndexType idx = lit->GetIndex();
    unsigned long length = lit->GetLength();
    for( unsigned int i=0; i<length; i++)
      {
      output->SetPixel( idx, function( input2->GetPixel(idx), label ) );
      idx[0]++;
      }
    }

}


template<class TInputImage, class TFeatureImage, class TOutputImage>
void
LabelMapContourOverlayImageFilter<TInputImage, TFeatureImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << indent << "Opacity: " << m_Opacity << std::endl;
}


}// end namespace itk
#endif
