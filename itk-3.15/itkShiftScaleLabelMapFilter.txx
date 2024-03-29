/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkShiftScaleLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShiftScaleLabelMapFilter_txx
#define __itkShiftScaleLabelMapFilter_txx

#include "itkShiftScaleLabelMapFilter.h"
#include "itkProgressReporter.h"


namespace itk {

template <class TImage>
ShiftScaleLabelMapFilter<TImage>
::ShiftScaleLabelMapFilter()
{
  m_Shift = 0;
  m_Scale = 1;
  m_ChangeBackgroundValue = false;
}


template <class TImage>
void
ShiftScaleLabelMapFilter<TImage>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  ImageType * output = this->GetOutput();

  // get the label objects
  typedef typename ImageType::LabelObjectContainerType LabelObjectContainerType;
  LabelObjectContainerType labelObjects = output->GetLabelObjectContainer();

  ProgressReporter progress( this, 0, labelObjects.size() );
  
  // change the background, if requested
  if( m_ChangeBackgroundValue )
    {
    PixelType label = static_cast<PixelType>( m_Scale * output->GetBackgroundValue() + m_Shift );
    output->SetBackgroundValue( label );
    }

  // and put back the objects in the map
  output->ClearLabels();
  for( typename LabelObjectContainerType::iterator it = labelObjects.begin();
    it != labelObjects.end();
    it++ )
    {
    LabelObjectType * lo = it->second;
    PixelType label = static_cast<PixelType>( m_Scale * lo->GetLabel() + m_Shift );
    lo->SetLabel( label );
    output->AddLabelObject( lo );
    
    // go to the next label
    progress.CompletedPixel();
    }

}


template <class TImage>
void
ShiftScaleLabelMapFilter<TImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Shift: "  << m_Shift << std::endl;
  os << indent << "Scale: "  << m_Scale << std::endl;
  os << indent << "ChangeBackgroundValue: "  << m_ChangeBackgroundValue << std::endl;
}

}// end namespace itk
#endif
