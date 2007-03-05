/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAttributeOpeningLabelCollectionImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAttributeOpeningLabelCollectionImageFilter_txx
#define __itkAttributeOpeningLabelCollectionImageFilter_txx

#include "itkAttributeOpeningLabelCollectionImageFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template <class TImage, class TAttributeAccessor>
AttributeOpeningLabelCollectionImageFilter<TImage, TAttributeAccessor>
::AttributeOpeningLabelCollectionImageFilter()
{
  m_Lambda = NumericTraits< AttributeValueType >::Zero;
  m_ReverseOrdering = false;
}


template <class TImage, class TAttributeAccessor>
void
AttributeOpeningLabelCollectionImageFilter<TImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ImageType * output = this->GetOutput();

  AttributeAccessorType accessor;

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


template <class TImage, class TAttributeAccessor>
void
AttributeOpeningLabelCollectionImageFilter<TImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ReverseOrdering: "  << m_ReverseOrdering << std::endl;
  os << indent << "Lambda: "  << static_cast<typename NumericTraits<AttributeValueType>::PrintType>(m_Lambda) << std::endl;
}

}// end namespace itk
#endif
