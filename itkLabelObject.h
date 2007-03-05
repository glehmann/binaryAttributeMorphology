/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelObject.h,v $
  Language:  C++
  Date:      $Date: 2005/01/21 20:13:31 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelObject_h
#define __itkLabelObject_h

#include <deque>
#include <itkLightObject.h>
#include "itkLabelCollectionImage.h"

namespace itk
{


namespace Functor {

template< class TLabelObject, class TAttributeAccessor >
class LabelObjectComparator
{
public:
  typedef TLabelObject LabelObjectType;
  typedef TAttributeAccessor AttributeAccessorType;
  bool operator()( const LabelObjectType * a, const LabelObjectType * b )
    {
    return accessor( a ) > accessor( b );
    }
private:
  AttributeAccessorType accessor;
};

template< class TLabelObject, class TAttributeAccessor >
class LabelObjectReverseComparator
{
public:
  typedef TLabelObject LabelObjectType;
  typedef TAttributeAccessor AttributeAccessorType;
  bool operator()( const LabelObjectType * a, const LabelObjectType * b )
    {
    return accessor( a ) < accessor( b );
    }
private:
  AttributeAccessorType accessor;
};

}



template < unsigned int VImageDimension >
class LabelObjectLine
{
public:
  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  typedef Index< ImageDimension > IndexType;
  typedef unsigned long LengthType;

  LabelObjectLine() {};

  LabelObjectLine( const IndexType & idx, const LengthType & length )
    {
    this->SetIndex( idx );
    this->SetLength( length );
    }

  void SetIndex( const IndexType & idx )
    {
    m_Index = idx;
    }

  IndexType & GetIndex()
    {
    return m_Index;
    }

  const IndexType & GetIndex() const
    {
    return m_Index;
    }

  void SetLength( const LengthType length )
    {
    m_Length = length;
    }

  LengthType & GetLength()
    {
    return m_Length;
    }

  const LengthType & GetLength() const
    {
    return m_Length;
    }

  bool HasIndex( const IndexType idx ) const
    {
    // are we talking about the right line ?
    for( int i=1; i<ImageDimension; i++ )
      {
      if( m_Index[i] != idx[i] )
        {
        return false;
        }
      }
    return ( idx[0] >= m_Index[0] && idx[0] < m_Index[0] + m_Length );
    }

  bool IsNextIndex( const IndexType & idx ) const
    {
    // are we talking about the right line ?
    for( int i=1; i<ImageDimension; i++ )
      {
      if( m_Index[i] != idx[i] )
        {
        return false;
        }
      }
    return idx[0] == m_Index[0] + m_Length;
    }

private:
  IndexType m_Index;
  LengthType m_Length;
};

// template <class TLabelObject > class LabelCollectionImage;


/** \class LabelObject
 *  \brief LabelObject class
 * 
 * This class derives from the Object class.
 *
 * The class is templated over the type of the elements.
 *
 * Template parameters for class LabelObject:
 *
 * - TAttribute = Element type stored in the node
 *
 * \ingroup DataRepresentation 
 */
template < class TLabel, unsigned int VImageDimension >
class ITK_EXPORT LabelObject : public LightObject
{
public:
  /** Standard class typedefs */
  typedef LabelObject         Self;
  typedef Object              Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  typedef WeakPointer<const Self>  ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LabelObject, Object);

  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  typedef itk::Index< ImageDimension > IndexType;

  typedef TLabel LabelType;

  typedef LabelObjectLine< VImageDimension > LineType;

  typedef typename LineType::LengthType LengthType;

  typedef typename std::deque< LineType > LineContainerType;

  const LabelType & GetLabel() const
    {
    return m_Label;
    }

  void SetLabel( const LabelType & label )
    {
    m_Label = label;
    }

  bool HasIndex( const IndexType & idx ) const
    {
    for( typename LineContainerType::const_iterator it=m_LineContainer.begin();
      it != m_LineContainer.end();
      it++ )
      {
      if( it->HasIndex( idx ) )
        {
        return true;
        }
      }
    return false;
    }

  void AddIndex( const IndexType & idx ) 
    {
    if( !m_LineContainer.empty() )
      {
      // can we use the last line to add that index ?
      LineType & lastLine = * m_LineContainer.rbegin();
      if( lastLine.IsNextIndex( idx ) )
        {
        lastLine.SetLength( lastLine.GetLength() + 1 );
        return;
        }
      }
    // create a new line
    this->AddLine( idx, 1 );
    }

  void AddLine( const IndexType & idx, const LengthType & length )
    {
    LineType line( idx, length );
    this->AddLine( line );
    }

  void AddLine( const LineType & line )
    {
    // TODO: add an assert to be sure that some indexes in the line are not already stored here
    m_LineContainer.push_back( line );
    }
    
  void PrintSelf(std::ostream& os, Indent indent) const
    {
//     Superclass::PrintSelf( os, indent );
    os << indent << "LineContainer: " << & m_LineContainer << std::endl;
    os << indent << "Label: " << static_cast<typename NumericTraits<LabelType>::PrintType>(m_Label) << std::endl; 
    }

  const LineContainerType & GetLineContainer() const
    {
    return m_LineContainer;
    }

  virtual void CopyDataFrom( const Self * src )
    {
    m_LineContainer = src->m_LineContainer;
    m_Label = src->m_Label;
    }

protected:
  LabelObject()
    {
    m_Label = NumericTraits< LabelType >::Zero;
    m_LineContainer.clear();
    }
  

private:
  LabelObject(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  LineContainerType m_LineContainer;
  LabelType m_Label;
};

} // end namespace itk

#endif
