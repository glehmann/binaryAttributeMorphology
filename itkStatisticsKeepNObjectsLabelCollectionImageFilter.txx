/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkStatisticsKeepNObjectsLabelCollectionImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkStatisticsKeepNObjectsLabelCollectionImageFilter_txx
#define __itkStatisticsKeepNObjectsLabelCollectionImageFilter_txx

#include "itkStatisticsKeepNObjectsLabelCollectionImageFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template <class TImage>
StatisticsKeepNObjectsLabelCollectionImageFilter<TImage>
::StatisticsKeepNObjectsLabelCollectionImageFilter()
{
  m_ReverseOrdering = false;
  m_NumberOfObjects = 1;
  m_Attribute = LabelObjectType::MEAN;
}


template <class TImage>
void
StatisticsKeepNObjectsLabelCollectionImageFilter<TImage>
::GenerateData()
{
  switch( m_Attribute )
    {
    case LabelObjectType::MINIMUM:
      TemplatedGenerateData< typename Functor::MinimumLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::MAXIMUM:
      TemplatedGenerateData< typename Functor::MaximumLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::MEAN:
      TemplatedGenerateData< typename Functor::MeanLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::SUM:
      TemplatedGenerateData< typename Functor::SumLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::SIGMA:
      TemplatedGenerateData< typename Functor::SigmaLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::VARIANCE:
      TemplatedGenerateData< typename Functor::VarianceLabelObjectAccessor< LabelObjectType > >();
      break;
    case LabelObjectType::MEDIAN:
      TemplatedGenerateData< typename Functor::MedianLabelObjectAccessor< LabelObjectType > >();
      break;
    default:
      Superclass::GenerateData();
      break;
    }
}


template <class TImage>
template <class TAttributeAccessor>
void
StatisticsKeepNObjectsLabelCollectionImageFilter<TImage>
::TemplatedGenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ImageType * output = this->GetOutput();

  typedef typename ImageType::LabelObjectContainerType LabelObjectContainerType;
  const LabelObjectContainerType & labelObjectContainer = output->GetLabelObjectContainer();
  typedef typename std::vector< typename LabelObjectType::Pointer > VectorType;

  ProgressReporter progress( this, 0, 2 * labelObjectContainer.size() );

  // get the label objects in a vector, so they can be sorted
  VectorType labelObjects;
  labelObjects.reserve( labelObjectContainer.size() );
  for( typename LabelObjectContainerType::const_iterator it = labelObjectContainer.begin();
    it != labelObjectContainer.end();
    it++ )
    {
    labelObjects.push_back( it->second );
    progress.CompletedPixel();
    }

  // instantiate the comparator and sort the vector
  if( m_NumberOfObjects < labelObjectContainer.size() )
    {
    typename VectorType::iterator end = labelObjects.begin() + m_NumberOfObjects;
    if( m_ReverseOrdering )
      {
      Functor::LabelObjectReverseComparator< LabelObjectType, TAttributeAccessor > comparator;
      std::nth_element( labelObjects.begin(), end, labelObjects.end(), comparator );
      }
    else
      {
      Functor::LabelObjectComparator< LabelObjectType, TAttributeAccessor > comparator;
      std::nth_element( labelObjects.begin(), end, labelObjects.end(), comparator );
      }
//   progress.CompletedPixel();
  
    // and remove the last objects of the map
    for( typename VectorType::const_iterator it = end;
      it != labelObjects.end();
      it++ )
      {
      output->RemoveLabelObject( *it );
      progress.CompletedPixel();
      }
    }
}


template <class TImage>
void
StatisticsKeepNObjectsLabelCollectionImageFilter<TImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "ReverseOrdering: "  << m_ReverseOrdering << std::endl;
  os << indent << "NumberOfObjects: "  << m_NumberOfObjects << std::endl;
  os << indent << "Attribute: "  << static_cast<typename NumericTraits<AttributeType>::PrintType>(m_Attribute) << std::endl;
}

}// end namespace itk
#endif
