/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkShapeLabelCollectionImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShapeLabelCollectionImageFilter_txx
#define __itkShapeLabelCollectionImageFilter_txx

#include "itkShapeLabelCollectionImageFilter.h"
#include "itkProgressReporter.h"
#include "itkNeighborhoodIterator.h"
#include "itkLabelCollectionImageToLabelImageFilter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkLabelPerimeterEstimationCalculator.h"


namespace itk {

template <class TImage, class TLabelImage>
ShapeLabelCollectionImageFilter<TImage, TLabelImage>
::ShapeLabelCollectionImageFilter()
{
  m_ComputeFeretDiameter = false;
  m_ComputePerimeter = false;
}


template<class TImage, class TLabelImage>
void
ShapeLabelCollectionImageFilter<TImage, TLabelImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();

  // generate the label image, if needed
  if( m_ComputeFeretDiameter || m_ComputePerimeter )
    {
    if( !m_LabelImage )
      {
      // generate an image of the labelized image
      typedef LabelCollectionImageToLabelImageFilter< TImage, LabelImageType > LCI2IType;
      typename LCI2IType::Pointer lci2i = LCI2IType::New();
      lci2i->SetInput( this->GetOutput() );
      // respect the number of threads of the filter
      lci2i->SetNumberOfThreads( this->GetNumberOfThreads() );
      lci2i->Update();
      m_LabelImage = lci2i->GetOutput();
      }
    }

  // delegate the computation of the perimeter to a dedicated calculator
  if( m_ComputePerimeter )
    {
    typedef LabelPerimeterEstimationCalculator< LabelImageType > CalculatorType;
    typename CalculatorType::Pointer calculator = CalculatorType::New();
    calculator->SetImage( m_LabelImage );
//     calculator->SetNumberOfThreads( this->GetNumberOfThreads() );
//     calculator->Compute();

    // TODO: make it work!
    }

}


template<class TImage, class TLabelImage>
void
ShapeLabelCollectionImageFilter<TImage, TLabelImage>
::ThreadedGenerateData( LabelObjectType * labelObject )
{
  ImageType * output = this->GetOutput();

  // TODO: compute sizePerPixel, borderMin and borderMax in BeforeThreadedGenerateData() ?

  // compute the size per pixel, to be used later
  double sizePerPixel = 1;
  for( int i=0; i<ImageDimension; i++ )
    {
    sizePerPixel *= output->GetSpacing()[i];
    }
  
  // compute the max the index on the border of the image
  IndexType borderMin = output->GetLargestPossibleRegion().GetIndex();
  IndexType borderMax = borderMin;
  for( int i=0; i<ImageDimension; i++ )
    {
    borderMax[i] += borderMin[i] + output->GetLargestPossibleRegion().GetSize()[i] - 1;
    }

  // init the vars
  unsigned long size = 0;
  ContinuousIndex< double, ImageDimension> centroid;
  centroid.Fill( 0 );
  IndexType mins;
  mins.Fill( NumericTraits< long >::max() );
  IndexType maxs;
  maxs.Fill( NumericTraits< long >::NonpositiveMin() );
  unsigned long sizeOnBorder = 0;

  typename LabelObjectType::LineContainerType::const_iterator lit;
  typename LabelObjectType::LineContainerType lineContainer = labelObject->GetLineContainer();

  // iterate over all the lines
  for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
    {
    const IndexType & idx = lit->GetIndex();
    unsigned long length = lit->GetLength();

    // update the size
    size += length;

    // update the centroid - and report the progress
    // first, update the axes which are not 0
    for( int i=1; i<ImageDimension; i++ )
      {
      centroid[i] += length * idx[i];
      }
    // then, update the axis 0
    centroid[0] += idx[0] * length + ( length * ( length - 1 ) ) / 2.0;

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

    // object is on a border ?
    bool isOnBorder = false;
    for( int i=1; i<ImageDimension; i++)
      {
      if( idx[i] == borderMin[i] || idx[i] == borderMax[i])
        {
        isOnBorder = true;
        break;
        }
      }
    if( isOnBorder )
      {
      // the line touch a border on a dimension other than 0, so
      // all the line touch a border
      sizeOnBorder += length;
      }
    else
      {
      // we must check for the dimension 0
      bool isOnBorder0 = false;
      if( idx[0] == borderMin[0] )
        {
        // one more pixel on the border
        sizeOnBorder++;
        isOnBorder0 = true;
        }
      if( !isOnBorder0 || length > 1 )
        {
        // we can check for the end of the line
        if( idx[0] + length - 1 == borderMax[0] )
          {
          // one more pixel on the border
          sizeOnBorder++;
          }
        }
      }
    }

  // final computation
  typename LabelObjectType::RegionType::SizeType regionSize;
  double minSize = NumericTraits< double >::max();
  double maxSize = NumericTraits< double >::NonpositiveMin();
  for( int i=0; i<ImageDimension; i++ )
    {
    centroid[i] /= size;
    regionSize[i] = maxs[i] - mins[i] + 1;
    double s = regionSize[i] * output->GetSpacing()[i];
    minSize = std::min( s, minSize );
    maxSize = std::max( s, maxSize );
    }
  typename LabelObjectType::RegionType region( mins, regionSize );
  typename LabelObjectType::CentroidType physicalCentroid;
  output->TransformContinuousIndexToPhysicalPoint( centroid, physicalCentroid );

  // set the values in the object
  labelObject->SetSize( size );
  labelObject->SetPhysicalSize( size * sizePerPixel );
  labelObject->SetRegion( region );
  labelObject->SetCentroid( centroid );
  labelObject->SetRegionElongation( maxSize / minSize );
  labelObject->SetSizeRegionRatio( size / (double)region.GetNumberOfPixels() );
  labelObject->SetSizeOnBorder( sizeOnBorder );

  if( m_ComputeFeretDiameter )
    {
    const PixelType & label = labelObject->GetLabel();

    // init the vars
    unsigned long size = 0;
    typedef typename std::deque< IndexType > IndexListType;
    IndexListType idxList;
    
    // the iterators
    typename LabelObjectType::LineContainerType::const_iterator lit;
    typename LabelObjectType::LineContainerType lineContainer = labelObject->GetLineContainer();

    typedef typename itk::ConstNeighborhoodIterator< LabelImageType > NeighborIteratorType;
    SizeType neighborHoodRadius;
    neighborHoodRadius.Fill( 1 );
    NeighborIteratorType it( neighborHoodRadius, m_LabelImage, m_LabelImage->GetBufferedRegion() );
    ConstantBoundaryCondition<LabelImageType> lcbc;
    // use label + 1 to have a label different of the current label on the border
    lcbc.SetConstant( label + 1 );
    it.OverrideBoundaryCondition( &lcbc );
    it.GoToBegin();

    // iterate over all the lines
    for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
      {
      const IndexType & firstIdx = lit->GetIndex();
      unsigned long length = lit->GetLength();

      long endIdx0 = firstIdx[0] + length;
      for( IndexType idx = firstIdx; idx[0]<endIdx0; idx[0]++)
        {

        // move the iterator to the new location
        it += idx - it.GetIndex();

        // push the pixel in the list if it is on the border of the object
        for (unsigned i = 0; i < it.Size(); i++)
          {
          if( it.GetPixel(i) != label )
            {
            idxList.push_back( idx );
            size++;
            break;
            }
          }
        }
      }

    // we can now search the feret diameter
    double feretDiameter = 0;
    for( typename IndexListType::const_iterator iIt1 = idxList.begin();
      iIt1 != idxList.end();
      iIt1++ )
      {
      typename IndexListType::const_iterator iIt2 = iIt1;
      for( iIt2++; iIt2 != idxList.end(); iIt2++ )
        {
        // Compute the length between the 2 indexes
        double length = 0;
        for( int i=0; i<ImageDimension; i++ )
          {
          length += vcl_pow( ( iIt1->operator[]( i ) - iIt2->operator[]( i ) ) * output->GetSpacing()[i], 2 );
          }
        if( feretDiameter < length )
          {
          feretDiameter = length;
          }
        }
      }
    // final computation
    feretDiameter = vcl_sqrt( feretDiameter );

    // finally put the values in the label object
    labelObject->SetFeretDiameter( feretDiameter );

    }

//   std::cout << std::endl;
//   labelObject->Print( std::cout );
//   std::cout << std::endl;

}


template<class TImage, class TLabelImage>
void
ShapeLabelCollectionImageFilter<TImage, TLabelImage>
::AfterThreadedGenerateData()
{
  Superclass::AfterThreadedGenerateData();

  // release the label image
  m_LabelImage = NULL;
}


template<class TImage, class TLabelImage>
void
ShapeLabelCollectionImageFilter<TImage, TLabelImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << indent << "ComputeFeretDiameter: " << m_ComputeFeretDiameter << std::endl;
  os << indent << "ComputePerimeter: " << m_ComputePerimeter << std::endl;
}


}// end namespace itk
#endif
