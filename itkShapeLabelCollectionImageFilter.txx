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


namespace itk {

template <class TImage>
ShapeLabelCollectionImageFilter<TImage>
::ShapeLabelCollectionImageFilter()
{
}


template<class TImage>
void
ShapeLabelCollectionImageFilter<TImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ImageType * output = this->GetOutput();

  ProgressReporter progress( this, 0, output->GetRequestedRegion().GetNumberOfPixels() );

  // compute the size per pixel, to be used laer
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
    borderMax[i] += output->GetLargestPossibleRegion().GetSize()[i] - 1;
    }

  typename ImageType::LabelObjectContainerType::const_iterator it;
  const typename ImageType::LabelObjectContainerType & labelObjectContainer = output->GetLabelObjectContainer();
  for( it = labelObjectContainer.begin(); it != labelObjectContainer.end(); it++ )
    {
    typedef typename ImageType::LabelObjectType LabelObjectType;
    LabelObjectType * labelObject = it->second;

    // init the vars
    unsigned long size = 0;
    typename LabelObjectType::CentroidType centroid;
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
//       std::cout << "idx: " << idx << "  length: " << length << std:: endl;

      // update the size
      size += length;

      // update the centroid - and report the progress
      // first, update the axes which are not 0
      for( int i=1; i<ImageDimension; i++ )
        {
        centroid[i] += length * idx[i];
        }
      // then, update the axis 0
      long i0 = idx[0];
      for( int i=0; i<length; i++)
        {
        centroid[0] += i0 + i;
        progress.CompletedPixel();
        }

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
        maxs[0] = idx[0] + length;
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

    // set the values in the object
    labelObject->SetSize( size );
    labelObject->SetPhysicalSize( size * sizePerPixel );
    labelObject->SetRegion( region );
    labelObject->SetCentroid( centroid );
    labelObject->SetRegionElongation( maxSize / minSize );
    labelObject->SetSizeRegionRatio( size / (double)region.GetNumberOfPixels() );
    labelObject->SetSizeOnBorder( sizeOnBorder );

//     std::cout << std::endl;
//     labelObject->Print( std::cout );
//     std::cout << std::endl;

    }
}


}// end namespace itk
#endif
