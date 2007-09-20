/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkStatisticsLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkStatisticsLabelMapFilter_txx
#define __itkStatisticsLabelMapFilter_txx

#include "itkStatisticsLabelMapFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkProgressReporter.h"
#include "itkHistogram.h"
#include "vnl/algo/vnl_real_eigensystem.h"
#include "vnl/algo/vnl_symmetric_eigensystem.h"


namespace itk {

template <class TImage, class TFeatureImage>
StatisticsLabelMapFilter<TImage, TFeatureImage>
::StatisticsLabelMapFilter()
{
  this->SetNumberOfRequiredInputs(2);
}


template <class TImage, class TFeatureImage>
void
StatisticsLabelMapFilter<TImage, TFeatureImage>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();

  // get the min and max of the feature image, to use those value as the bounds of our
  // histograms
  typedef MinimumMaximumImageCalculator< FeatureImageType > MinMaxCalculatorType;
  typename MinMaxCalculatorType::Pointer minMax = MinMaxCalculatorType::New();
  minMax->SetImage( this->GetFeatureImage() );
  minMax->Compute();

  m_Minimum = minMax->GetMinimum();
  m_Maximum = minMax->GetMaximum();

}


template <class TImage, class TFeatureImage>
void
StatisticsLabelMapFilter<TImage, TFeatureImage>
::ThreadedGenerateData( LabelObjectType * labelObject )
{
  Superclass::ThreadedGenerateData( labelObject );

  ImageType * output = this->GetOutput();
  const FeatureImageType * featureImage = this->GetFeatureImage();

  typedef Statistics::Histogram< double > HistogramType;

  typename HistogramType::SizeType histogramSize;
  histogramSize.Fill( 256 );
  typename HistogramType::MeasurementVectorType featureImageMin;
  featureImageMin.Fill( m_Minimum );
  typename HistogramType::MeasurementVectorType featureImageMax;
  featureImageMax.Fill( m_Maximum );

  typename HistogramType::Pointer histogram = HistogramType::New();
  histogram->SetClipBinsAtEnds( false );
  histogram->Initialize( histogramSize, featureImageMin, featureImageMax );

  typename LabelObjectType::LineContainerType::const_iterator lit;
  typename LabelObjectType::LineContainerType lineContainer = labelObject->GetLineContainer();

  FeatureImagePixelType min = NumericTraits< FeatureImagePixelType >::max();
  FeatureImagePixelType max = NumericTraits< FeatureImagePixelType >::NonpositiveMin();
  double sum = 0;
  double sum2 = 0;
  double sum3 = 0;
  double sum4 = 0;
  IndexType minIdx;
  IndexType maxIdx;
  PointType centerOfGravity;
  centerOfGravity.Fill( 0 );
  MatrixType centralMoments;
  centralMoments.Fill( 0 );
  MatrixType principalAxes;
  principalAxes.Fill( 0 );
  VectorType principalMoments;
  principalMoments.Fill( 0 );

  // iterate over all the lines
  for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
    {
    const IndexType & firstIdx = lit->GetIndex();
    unsigned long length = lit->GetLength();

    typename HistogramType::MeasurementVectorType mv;
    long endIdx0 = firstIdx[0] + length;
    for( IndexType idx = firstIdx; idx[0]<endIdx0; idx[0]++)
      {
      const FeatureImagePixelType & v = featureImage->GetPixel( idx );
      mv[0] = v;
      histogram->IncreaseFrequency( mv, 1 );

      // update min and max
      if( v <= min )
        {
        min = v;
        minIdx = idx;
        }
      if( v >= max )
        {
        max = v;
        maxIdx = idx;
        }

      //increase the sums
      sum += v;
      sum2 += vcl_pow( (double)v, 2 );
      sum3 += vcl_pow( (double)v, 3 );
      sum4 += vcl_pow( (double)v, 4 );

      // moments
      PointType physicalPosition;
      output->TransformIndexToPhysicalPoint(idx, physicalPosition);
      for(unsigned int i=0; i<ImageDimension; i++)
        {
        centerOfGravity[i] += physicalPosition[i] * v; 
        centralMoments[i][i] += v * physicalPosition[i] * physicalPosition[i];
        for(unsigned int j=i+1; j<ImageDimension; j++)
          {
          double weight = v * physicalPosition[i] * physicalPosition[j];
          centralMoments[i][j] += weight;
          centralMoments[j][i] += weight;
          }
        }

      }
    }

  // final computations
  const typename HistogramType::FrequencyType & totalFreq = histogram->GetTotalFrequency();
  double mean = sum / totalFreq;
  double variance = ( sum2 - ( vcl_pow( sum, 2 ) / totalFreq ) ) / ( totalFreq - 1 );
  double sigma = vcl_sqrt( variance );
  double mean2 = mean * mean;
  double skewness = ( ( sum3 - 3.0 * mean * sum2) / totalFreq + 2.0 * mean * mean2 ) / ( variance * sigma );
  double kurtosis = ( ( sum4 - 4.0 * mean * sum3 + 6.0 * mean2 * sum2) / totalFreq - 3.0 * mean2 * mean2 ) / ( variance * variance ) - 3.0;

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
    for(unsigned int j=0; j<ImageDimension; j++)
      {
      centralMoments[i][j] /= sum;
      }
    }

  // Center the second order moments
  for(unsigned int i=0; i<ImageDimension; i++)
    {
    for(unsigned int j=0; j<ImageDimension; j++)
      {
      centralMoments[i][j] -= centerOfGravity[i] * centerOfGravity[j];
      }
    }

  // Compute principal moments and axes
  vnl_symmetric_eigensystem<double> eigen( centralMoments.GetVnlMatrix() );
  vnl_diag_matrix<double> pm = eigen.D;
  for(unsigned int i=0; i<ImageDimension; i++)
    {
    principalMoments[i] = pm(i,i) * sum;
    }
  principalAxes = eigen.V.transpose();

  // Add a final reflection if needed for a proper rotation,
  // by multiplying the last row by the determinant
  vnl_real_eigensystem eigenrot( principalAxes.GetVnlMatrix() );
  vnl_diag_matrix< vcl_complex<double> > eigenval = eigenrot.D;
  vcl_complex<double> det( 1.0, 0.0 );

  for(unsigned int i=0; i<ImageDimension; i++)
    {
    det *= eigenval( i, i );
    }

  for(unsigned int i=0; i<ImageDimension; i++)
    {
    principalAxes[ ImageDimension-1 ][i] *= std::real( det );
    }

  double minPrincipalMoment = NumericTraits< double >::max();
  double maxPrincipalMoment = NumericTraits< double >::NonpositiveMin();
  for( int i=0; i<ImageDimension; i++ )
    {
    minPrincipalMoment = std::min( principalMoments[i], minPrincipalMoment );
    maxPrincipalMoment = std::max( principalMoments[i], maxPrincipalMoment );
    }

  double elongation = 0;
  if( minPrincipalMoment != 0 )
    {
    elongation = maxPrincipalMoment / minPrincipalMoment;
    }

  // finally put the values in the label object
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
  labelObject->SetPrincipalAxes( principalAxes );
  labelObject->SetPrincipalMoments( principalMoments );
  labelObject->SetCentralMoments( centralMoments );
  labelObject->SetSkewness( skewness );
  labelObject->SetKurtosis( kurtosis );
  labelObject->SetElongation( elongation );

//     std::cout << std::endl;
//     labelObject->Print( std::cout );
//     std::cout << std::endl;

}


}// end namespace itk
#endif