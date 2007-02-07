/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelCollectionImage.txx,v $
  Language:  C++
  Date:      $Date: 2006/05/10 20:27:16 $
  Version:   $Revision: 1.97 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkLabelCollectionImage_txx
#define _itkLabelCollectionImage_txx

#include "itkLabelCollectionImage.h"
#include "itkProcessObject.h"

namespace itk
{

/**
 *
 */
template<class TLabelObject >
LabelCollectionImage<TLabelObject>
::LabelCollectionImage()
{
  m_BackgroundLabel = NumericTraits< LabelType >::Zero;
  m_UseBackground = true;
  this->Initialize();
}


/**
 *
 */
template<class TLabelObject >
void 
LabelCollectionImage<TLabelObject>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
//   os << indent << "PixelContainer: " << std::endl;
//   m_Buffer->Print(os, indent.GetNextIndent());

// m_Origin and m_Spacing are printed in the Superclass
}


/**
 *
 */
template<class TLabelObject >
void 
LabelCollectionImage<TLabelObject>
::Initialize()
{
  m_LabelObjectContainer.clear();
}


template<class TLabelObject >
void 
LabelCollectionImage<TLabelObject>
::Graft(const DataObject *data)
{
  // call the superclass' implementation
  Superclass::Graft( data );

  if ( data )
    {
    // Attempt to cast data to an Image
    const Self * imgData;

    try
      {
      imgData = dynamic_cast<const Self *>( data );
      }
    catch( ... )
      {
      return;
      }


    if ( imgData )
      {
      // Now copy anything remaining that is needed
/*      this->SetRoot( const_cast< NodeType * >
                                  (imgData->GetRoot() ) );
      this->m_LinkedListArray = imgData->m_LinkedListArray; */
      }
    else
      {
      // pointer could not be cast back down
      itkExceptionMacro( << "itk::Image::Graft() cannot cast "
                         << typeid(data).name() << " to "
                         << typeid(const Self *).name() );
      }
    }
}

} // end namespace itk

#endif
