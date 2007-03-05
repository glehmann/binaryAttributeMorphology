/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkStatisticsOpeningLabelCollectionImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkStatisticsOpeningLabelCollectionImageFilter_txx
#define __itkStatisticsOpeningLabelCollectionImageFilter_txx

#include "itkStatisticsOpeningLabelCollectionImageFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template <class TImage>
StatisticsOpeningLabelCollectionImageFilter<TImage>
::StatisticsOpeningLabelCollectionImageFilter()
{
  m_Lambda = NumericTraits< double >::Zero;
  m_ReverseOrdering = false;
  m_Attribute = LabelObjectType::MEAN;
  this->SetNumberOfRequiredInputs(2);
}


template <class TImage>
void
StatisticsOpeningLabelCollectionImageFilter<TImage>
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
StatisticsOpeningLabelCollectionImageFilter<TImage>
::TemplatedGenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ImageType * output = this->GetOutput();

  TAttributeAccessor accessor;

  const typename ImageType::LabelObjectContainerType & labelObjectContainer = output->GetLabelObjectContainer();

  ProgressReporter progress( this, 0, labelObjectContainer.size() );

  typename ImageType::LabelObjectContainerType::const_iterator it = labelObjectContainer.begin();
  while( it != labelObjectContainer.end() )
    {
    typedef typename ImageType::LabelObjectType LabelObjectType;
    typename LabelObjectType::LabelType label = it->first;
    LabelObjectType * labelObject = it->second;

    if( ( !m_ReverseOrdering && accessor( labelObject ) < m_Lambda )
      || ( m_ReverseOrdering && accessor( labelObject ) > m_Lambda ) )
      {
      // must increment the iterator before removing the object to avoid invalidating the iterator
      it++;
      output->RemoveLabel( label );
      }
    else
      {
      it++;
      }

    progress.CompletedPixel();
    }
}


template <class TImage>
void
StatisticsOpeningLabelCollectionImageFilter<TImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ReverseOrdering: "  << m_ReverseOrdering << std::endl;
  os << indent << "Lambda: "  << static_cast<typename NumericTraits<double>::PrintType>(m_Lambda) << std::endl;
  os << indent << "Attribute: "  << static_cast<typename NumericTraits<AttributeType>::PrintType>(m_Attribute) << std::endl;
}

}// end namespace itk
#endif
