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
      }

    // final computation
    typename LabelObjectType::RegionType::SizeType regionSize;
    for( int i=0; i<ImageDimension; i++ )
      {
      centroid[i] /= size;
      regionSize[i] = maxs[i] - mins[i] + 1;
      }
    typename LabelObjectType::RegionType region( mins, regionSize );

    // set the values in the object
    labelObject->SetSize( size );
    labelObject->SetPhysicalSize( size * sizePerPixel );
    labelObject->SetRegion( region );
    labelObject->SetCentroid( centroid );

//     std::cout << std::endl;
//     labelObject->Print( std::cout );
//     std::cout << std::endl;

    }
}


}// end namespace itk
#endif
