/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkStatisticsLabelCollectionImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkStatisticsLabelCollectionImageFilter_txx
#define __itkStatisticsLabelCollectionImageFilter_txx

#include "itkStatisticsLabelCollectionImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkProgressReporter.h"
#include "itkHistogram.h"


namespace itk {

template <class TImage, class TFeatureImage>
StatisticsLabelCollectionImageFilter<TImage, TFeatureImage>
::StatisticsLabelCollectionImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
}


template <class TImage, class TFeatureImage>
void
StatisticsLabelCollectionImageFilter<TImage, TFeatureImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ImageType * output = this->GetOutput();
  const FeatureImageType * featureImage = this->GetFeatureImage();

  ProgressReporter progress( this, 0, output->GetRequestedRegion().GetNumberOfPixels() );

  // first, get the min and max of the feature image, to use those value as the bounds of our
  // histograms
  typedef MinimumMaximumImageCalculator< FeatureImageType > MinMaxCalculatorType;
  typename MinMaxCalculatorType::Pointer minMax = MinMaxCalculatorType::New();
  minMax->SetImage( featureImage );
  minMax->Compute();
  
  typedef Statistics::Histogram< double > HistogramType;

  typename HistogramType::SizeType histogramSize;
  histogramSize.Fill( 256 );
  typename HistogramType::MeasurementVectorType featureImageMin;
  featureImageMin.Fill( minMax->GetMinimum() );
  typename HistogramType::MeasurementVectorType featureImageMax;
  featureImageMax.Fill( minMax->GetMaximum() );

  typename ImageType::LabelObjectContainerType::const_iterator it;
  const typename ImageType::LabelObjectContainerType & labelObjectContainer = output->GetLabelObjectContainer();
  for( it = labelObjectContainer.begin(); it != labelObjectContainer.end(); it++ )
    {
    typedef typename ImageType::LabelObjectType LabelObjectType;
    LabelObjectType * labelObject = it->second;

    typename HistogramType::Pointer histogram = HistogramType::New();
    histogram->SetClipBinsAtEnds( false );
    histogram->Initialize( histogramSize, featureImageMin, featureImageMax );

    typename LabelObjectType::LineContainerType::const_iterator lit;
    typename LabelObjectType::LineContainerType lineContainer = labelObject->GetLineContainer();

    FeatureImagePixelType min = NumericTraits< FeatureImagePixelType >::max();
    FeatureImagePixelType max = NumericTraits< FeatureImagePixelType >::NonpositiveMin();
    double sum = 0;
    double sumOfSquares = 0;
    IndexType minIdx;
    IndexType maxIdx;
    PointType centerOfGravity;
    centerOfGravity.Fill( 0 );

    // iterate over all the lines
    for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
      {
      const IndexType & firstIdx = lit->GetIndex();
      unsigned long length = lit->GetLength();

      typename HistogramType::MeasurementVectorType mv;
      unsigned long endIdx0 = firstIdx[0] + length;
      for( IndexType idx = firstIdx; idx[0]<endIdx0; idx[0]++)
        {
        const FeatureImagePixelType & v = featureImage->GetPixel( idx );
        mv[0] = v;
        histogram->IncreaseFrequency( mv, 1 );

        // update min and max
        if( v < min )
          {
          min = v;
          minIdx = idx;
          }
        if( v > max )
          {
          max = v;
          maxIdx = idx;
          }
  
        //increase the sums
        sum += v;
        sumOfSquares += vcl_pow( (double)v, 2 );

        // center of gravity
        PointType physicalPosition;
        output->TransformIndexToPhysicalPoint(idx, physicalPosition);
        for(unsigned int i=0; i<ImageDimension; i++)
          {
          centerOfGravity[i] += physicalPosition[i] * v; 
          }

        }
      }

    // final computations
    const typename HistogramType::FrequencyType & totalFreq = histogram->GetTotalFrequency();
    double mean = sum / totalFreq;
    double variance = ( sumOfSquares - ( vcl_pow( sum, 2 ) / totalFreq ) ) / ( totalFreq - 1 );
    double sigma = vcl_sqrt( variance );

    // the median
    double median = 0;
    double count = 0;  // will not be fully set, so do not use later !
    for( unsigned long i=0;
      i<histogram->Size();
      i++)
      {
      count += histogram->GetFrequency( i );

      if( count >= ( totalFreq / 2 ) )
        {
        median = histogram->GetMeasurementVector( i )[0];
        break;
        }
      }

    // Normalize using the total mass
    for(unsigned int i=0; i<ImageDimension; i++)
      {
      centerOfGravity[i] /= sum;
      }

    // finally put the value in the label object
    labelObject->SetMinimum( (double)min );
    labelObject->SetMaximum( (double)max );
    labelObject->SetSum( sum );
    labelObject->SetMean( mean );
    labelObject->SetMedian( median );
    labelObject->SetVariance( variance );
    labelObject->SetSigma( sigma );
    labelObject->SetMinimumIndex( minIdx );
    labelObject->SetMaximumIndex( maxIdx );
    labelObject->SetCenterOfGravity( centerOfGravity );

    std::cout << std::endl;
    labelObject->Print( std::cout );
    std::cout << std::endl;
    }
}


}// end namespace itk
#endif
