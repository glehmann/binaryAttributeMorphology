/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkReconstructionLabelCollectionImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkReconstructionLabelCollectionImageFilter_txx
#define __itkReconstructionLabelCollectionImageFilter_txx

#include "itkReconstructionLabelCollectionImageFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template <class TImage, class TMaskImage>
ReconstructionLabelCollectionImageFilter<TImage, TMaskImage>
::ReconstructionLabelCollectionImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
  m_ForegroundValue = NumericTraits< MaskImagePixelType >::max();
}


template <class TImage, class TMaskImage>
void
ReconstructionLabelCollectionImageFilter<TImage, TMaskImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  Superclass::GenerateData();

  ImageType * output = this->GetOutput();
  const MaskImageType * maskImage = this->GetMaskImage();

  ProgressReporter progress( this, 0, output->GetRequestedRegion().GetNumberOfPixels() );

  const typename ImageType::LabelObjectContainerType & labelObjectContainer = output->GetLabelObjectContainer();
  typename ImageType::LabelObjectContainerType::const_iterator it = labelObjectContainer.begin();
  
  while( it != labelObjectContainer.end() )
    {
    typedef typename ImageType::LabelObjectType LabelObjectType;
    LabelObjectType * labelObject = it->second;
    const PixelType & label = it->first;

    typename LabelObjectType::LineContainerType::const_iterator lit;
    typename LabelObjectType::LineContainerType lineContainer = labelObject->GetLineContainer();

    // iterate over all the lines to find a pixel inside the object
    bool removeLabelObject = false;
    
    for( lit = lineContainer.begin();
      lit != lineContainer.end() && !removeLabelObject;
      lit++ )
      {
      const IndexType & firstIdx = lit->GetIndex();
      unsigned long length = lit->GetLength();

      long endIdx0 = firstIdx[0] + length;
      for( IndexType idx = firstIdx;
        idx[0]<endIdx0 && !removeLabelObject;
        idx[0]++ )
        {
        const MaskImagePixelType & v = maskImage->GetPixel( idx );
        if( v == m_ForegroundValue )
          {
          removeLabelObject = true;
          }
        progress.CompletedPixel();
        }
      }
      
      // increment the iterator before removing the object to avoid invalidating the iterator
      it++;
      
      if( removeLabelObject )
        {
        output->RemoveLabel( label );
        }

    }
}


template <class TImage, class TAttributeAccessor>
void
AttributeOpeningLabelCollectionImageFilter<TImage, TAttributeAccessor>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "ForegroundValue: "  << static_cast<typename NumericTraits<MaskImagePixelType>::PrintType>(m_ForegroundValue) << std::endl;
}

}// end namespace itk
#endif
