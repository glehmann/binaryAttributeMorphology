/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAttributePositionLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAttributePositionLabelMapFilter_txx
#define __itkAttributePositionLabelMapFilter_txx

#include "itkAttributePositionLabelMapFilter.h"
#include "itkProgressReporter.h"
#include "itkLabelMapUtilities.h"


namespace itk {

template <class TImage, class TAttributeAccessor, bool VPhysicalPosition>
AttributePositionLabelMapFilter<TImage, TAttributeAccessor, VPhysicalPosition>
::AttributePositionLabelMapFilter()
{
}

template <class TImage, class TAttributeAccessor, bool VPhysicalPosition>
void
AttributePositionLabelMapFilter<TImage, TAttributeAccessor, VPhysicalPosition>
::ThreadedGenerateData( LabelObjectType * labelObject )
{
  LabelMapUtilities::PositionThreadedGenerateData<Self, TAttributeAccessor, VPhysicalPosition>( this, labelObject );
}

template <class TImage, class TAttributeAccessor, bool VPhysicalPosition>
void
AttributePositionLabelMapFilter<TImage, TAttributeAccessor, VPhysicalPosition>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}

}// end namespace itk
#endif
