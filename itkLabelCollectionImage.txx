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


template<class TLabelObject >
bool 
LabelCollectionImage<TLabelObject>
::HasLabel( const LabelType label ) const
{
  if( m_UseBackground && label == m_BackgroundLabel )
    {
    return true;
    }
  return m_LabelObjectContainer.find( label ) != m_LabelObjectContainer.end();
}


template<class TLabelObject >
const typename LabelCollectionImage<TLabelObject>::LabelType &
LabelCollectionImage<TLabelObject>
::GetPixel( const IndexType & idx ) const
{
  
}


template<class TLabelObject >
void 
LabelCollectionImage<TLabelObject>
::SetPixel( const IndexType & idx, const LabelType & label )
{
  if( m_UseBackground && label == m_BackgroundLabel )
    {
    // just do nothing
    return;
    }
  typename LabelObjectContainerType::iterator it = m_LabelObjectContainer.find( label );
  if( it != m_LabelObjectContainer.end() )
    {
    // the label already exist - add the pixel to it
    (*it).second->AddIndex( idx );
    }
  else
    {
    // the label does not exist yet - create a new one
    LabelObjectPointerType labelObject = LabelObjectType::New();
    labelObject->SetLabel( label );
    labelObject->SetLabelCollectionImage( this ); // also add the label to the container
    labelObject->AddIndex( idx );
    }
  
}


template<class TLabelObject >
typename LabelCollectionImage<TLabelObject>::LabelObjectType *
LabelCollectionImage<TLabelObject>
::GetLabelObject( const IndexType & idx ) const
{
  for( typename LabelObjectContainerType::iterator it = m_LabelObjectContainer.begin();
    it != m_LabelObjectContainer.end();
    it++ )
    {
    if( (*it)->HasIndex( idx ) )
      {
      return (*it)->GetPointer();
      }
    }
  return NULL;
}


template<class TLabelObject >
void
LabelCollectionImage<TLabelObject>
::AddLabelObject( LabelObjectType * labelObject )
{
  assert( labelObject != NULL );
//   assert( ! this->HasLabel( labelObject->GetLabel() ) );

  LabelType label = labelObject->GetLabel();
std::cout << "label: " << label << std::endl;
  m_LabelObjectContainer[ label ] = NULL;
}


template<class TLabelObject >
void
LabelCollectionImage<TLabelObject>
::RemoveLabelObject( LabelObjectType * labelObject )
{
  assert( labelObject != NULL );
  this->RemoveLabel( labelObject->GetLabel() );
}


template<class TLabelObject >
void
LabelCollectionImage<TLabelObject>
::RemoveLabel( const LabelType & label )
{
  if( m_UseBackground && label == m_BackgroundLabel )
    {
    // just do nothing
    }
  m_LabelObjectContainer.erase( label );
}


} // end namespace itk

#endif
