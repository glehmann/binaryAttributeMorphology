/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkChangeRegionLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2004/07/11 14:56:39 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkChangeRegionLabelMapFilter_txx
#define _itkChangeRegionLabelMapFilter_txx
#include "itkChangeRegionLabelMapFilter.h"


namespace itk
{


template <class TInputImage>
void 
LabelMapMaskImageFilter<TInputImage>
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

template <class TInputImage>
void 
LabelMapMaskImageFilter<TInputImage>
::GenerateOutputInformation()
{
  Superclass::GenerateOutputInformation();
  this->GetOutput()->SetLargestPossibleRegion( m_Region );
}

template <class TInputImage>
void 
LabelMapMaskImageFilter<TInputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}

template<class TInputImage, class TOutputImage>
void
LabelMapMaskImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData( LabelObjectType * labelObject )
{
  typename InputImageType::LabelObjectType::LineContainerType::const_iterator lit;
  typename InputImageType::LabelObjectType::LineContainerType lineContainer = labelObject->GetLineContainer();
  labelObject->GetLineContainer().clear();
  
  IndexType idxMin = m_Region.GetIndex();
  IndexType idxMax;
  for( int i=0; i<ImageDimension; i++ )
    {
    idxMax = idxMin[i] + m_Region.GetSize() - 1;
    }

  for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
    {
    IndexType idx = lit->GetIndex();
    unsigned long length = lit->GetLength();
    
    outside = false;
    for( int i=1; i<ImageDimension; i++ )
      {
      if( idx[i] < idxMin[i] || idx[i] > idxMax[i] )
        {
        outside = true;
        }
      }
    // check the axis 0
    if( !outside )
      {
      long lastIdx0 = idx[0] + length - 1;
      if( !( ( idx[0] < idxMin[i] && lastIdx0 < idxMin[i] )
               || ( idx[0] > idxMin[i] && lastIdx0 > idxMin[i] ) ) ) )
        {
        IndexType newIdx = idx;
        long newLenght = length;
        if( idx[0] < idxMin[0] )
          {
          length -= idxMin[0] - idx[0];
          newIdx[0] = idxMin[0];
          }
        if( lastIdx0 > idxMax[0] )
          {
          length -= lastIdx0 - idxMax[0];
          }
        
        }
      }
  
    labelObject->AddLine( newIdx, newLength );
    }

}

template<class TInputImage>
void 
ChangeRegionLabelMapFilter<TInputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "Region: " << m_Region << std::endl;
}


} // end namespace itk

#endif
