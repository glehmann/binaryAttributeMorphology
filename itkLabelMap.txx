/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelMap.txx,v $
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
#ifndef _itkLabelMap_txx
#define _itkLabelMap_txx

#include "itkLabelMap.h"
#include "itkProcessObject.h"

namespace itk
{

/**
 *
 */
template<class TLabelObject >
LabelMap<TLabelObject>
::LabelMap()
{
  m_BackgroundValue = NumericTraits< LabelType >::Zero;
  m_UseBackground = true;
  this->Initialize();
}


/**
 *
 */
template<class TLabelObject >
void 
LabelMap<TLabelObject>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);
  
  os << indent << "BackgroundValue: " << static_cast<typename NumericTraits<LabelType>::PrintType>(m_BackgroundValue) << std::endl;
  os << indent << "UseBackground: " << static_cast<typename NumericTraits<bool>::PrintType>(m_UseBackground) << std::endl;
  os << indent << "LabelObjectContainer: " << & m_LabelObjectContainer << std::endl;
}


/**
 *
 */
template<class TLabelObject >
void 
LabelMap<TLabelObject>
::Initialize()
{
  m_LabelObjectContainer.clear();
}


/**
 *
 */
template<class TLabelObject >
void 
LabelMap<TLabelObject>
::Allocate()
{
  this->Initialize();
}

template<class TLabelObject >
void 
LabelMap<TLabelObject>
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
      m_LabelObjectContainer = imgData->m_LabelObjectContainer;
      m_UseBackground = imgData->m_UseBackground;
      m_BackgroundValue = imgData->m_BackgroundValue;
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
typename LabelMap<TLabelObject>::LabelObjectType * 
LabelMap<TLabelObject>
::GetLabelObject( const LabelType & label )
{
  if( ! this->HasLabel( label ) )
    {
    itkExceptionMacro( << "No label object with label "
      << static_cast<typename NumericTraits< LabelType >::PrintType>(label)
      << "." );
    }
  if( m_UseBackground && m_BackgroundValue == label )
    {
    itkExceptionMacro( << "Label "
      << static_cast<typename NumericTraits< LabelType >::PrintType>(label)
      << " is the background label." );
    }
  return m_LabelObjectContainer[label].GetPointer();
}


template<class TLabelObject >
const typename LabelMap<TLabelObject>::LabelObjectType * 
LabelMap<TLabelObject>
::GetLabelObject( const LabelType & label ) const
{
  if( ! this->HasLabel( label ) )
    {
    itkExceptionMacro( << "No label object with label "
      << static_cast<typename NumericTraits< LabelType >::PrintType>(label)
      << "." );
    }
  if( m_UseBackground && m_BackgroundValue == label )
    {
    itkExceptionMacro( << "Label "
      << static_cast<typename NumericTraits< LabelType >::PrintType>(label)
      << " is the background label." );
    }
  return m_LabelObjectContainer.find( label )->second.GetPointer();
}


template<class TLabelObject >
bool 
LabelMap<TLabelObject>
::HasLabel( const LabelType label ) const
{
  if( m_UseBackground && label == m_BackgroundValue )
    {
    return true;
    }
  return m_LabelObjectContainer.find( label ) != m_LabelObjectContainer.end();
}


template<class TLabelObject >
const typename LabelMap<TLabelObject>::LabelType &
LabelMap<TLabelObject>
::GetPixel( const IndexType & idx ) const
{
  for( typename LabelObjectContainerType::const_iterator it = m_LabelObjectContainer.begin();
    it != m_LabelObjectContainer.end();
    it++ )
    {
    if( it->second->HasIndex( idx ) )
      {
      return it->second->GetLabel();
      }
    }
  return m_BackgroundValue;
}


template<class TLabelObject >
void 
LabelMap<TLabelObject>
::SetPixel( const IndexType & idx, const LabelType & label )
{
  if( m_UseBackground && label == m_BackgroundValue )
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
    labelObject->AddIndex( idx );
    this->AddLabelObject( labelObject );
//     std::cout<< m_LabelObjectContainer.size() << std::endl;
    }
  
}


template<class TLabelObject >
void 
LabelMap<TLabelObject>
::SetLine( const IndexType & idx, const unsigned long & length, const LabelType & label )
{
  if( m_UseBackground && label == m_BackgroundValue )
    {
    // just do nothing
    return;
    }

  typename LabelObjectContainerType::iterator it = m_LabelObjectContainer.find( label );

  if( it != m_LabelObjectContainer.end() )
    {
    // the label already exist - add the pixel to it
    (*it).second->AddLine( idx, length );
    }
  else
    {
    // the label does not exist yet - create a new one
    LabelObjectPointerType labelObject = LabelObjectType::New();
    labelObject->SetLabel( label );
    labelObject->AddLine( idx, length );
    this->AddLabelObject( labelObject );
//     std::cout<< m_LabelObjectContainer.size() << std::endl;
    }
}


template<class TLabelObject >
typename LabelMap<TLabelObject>::LabelObjectType *
LabelMap<TLabelObject>
::GetLabelObject( const IndexType & idx ) const
{
  for( typename LabelObjectContainerType::const_iterator it = m_LabelObjectContainer.begin();
    it != m_LabelObjectContainer.end();
    it++ )
    {
    if( it->second->HasIndex( idx ) )
      {
      return it->second.GetPointer();
      }
    }
    itkExceptionMacro( << "No label object at index " << idx << "." );
//   return NULL;
}


template<class TLabelObject >
void
LabelMap<TLabelObject>
::AddLabelObject( LabelObjectType * labelObject )
{
  assert( labelObject != NULL );
  assert( !this->HasLabel( labelObject->GetLabel() ) );

  m_LabelObjectContainer[ labelObject->GetLabel() ] = labelObject;
}


template<class TLabelObject >
void
LabelMap<TLabelObject>
::RemoveLabelObject( LabelObjectType * labelObject )
{
  assert( labelObject != NULL );
  this->RemoveLabel( labelObject->GetLabel() );
}


template<class TLabelObject >
void
LabelMap<TLabelObject>
::RemoveLabel( const LabelType & label )
{
  if( m_UseBackground && label == m_BackgroundValue )
    {
    // just do nothing
    return;
    }
  m_LabelObjectContainer.erase( label );
}


template<class TLabelObject >
void
LabelMap<TLabelObject>
::ClearLabels()
{
  m_LabelObjectContainer.clear();
}


template<class TLabelObject >
const typename LabelMap<TLabelObject>::LabelObjectContainerType &
LabelMap<TLabelObject>
::GetLabelObjectContainer() const
{
  return m_LabelObjectContainer;
}


template<class TLabelObject >
unsigned long
LabelMap<TLabelObject>
::GetNumberOfObjects() const
{
  return m_LabelObjectContainer.size();
}


template<class TLabelObject >
void 
LabelMap<TLabelObject>
::PrintObjects(std::ostream& os) const
{
  for( typename LabelObjectContainerType::const_iterator it = m_LabelObjectContainer.begin();
    it != m_LabelObjectContainer.end();
    it++ )
    {
    assert( it->second.IsNotNull() );
    it->second->Print( os );
    os << std::endl;
    }
}




} // end namespace itk

#endif
