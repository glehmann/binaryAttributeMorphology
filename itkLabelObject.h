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

template <class TIndex>
class LabelObjectLine
{
public:
  typedef TIndex IndexType;
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
    for( int i=1; i<=IndexType::VIndexDimension; i++ )
      {
      if( m_Index[i] != idx[i] )
        {
        return false;
        }
      }
    return ( idx[0] >= m_Index[0] && idx[0] < m_Index[0] + m_Length );
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
class ITK_EXPORT LabelObject : public Object
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

  typedef LabelCollectionImage< Self > LabelCollectionImageType;

  typedef typename LabelCollectionImageType::IndexType IndexType;

  typedef TLabel LabelType;

  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  typedef LabelObjectLine< IndexType > LineType;

  typedef typename LineType::LengthType LengthType;

  typedef typename std::deque< LineType > LineContainerType;

  const LabelType & GetLabel() const
    {
    return m_Label;
    }

  void SetLabel( const LabelType & label )
    {
    if( label == m_Label )
      {
      return;
      }

    if( m_LabelCollectionImage )
      {
      if( m_LabelCollectionImage->HasLabel( label ) )
        {
        itkExceptionMacro(<< "The label is already used" );
        }
      m_LabelCollectionImage->RemoveLabelObject( this );
      m_Label = label;
      m_LabelCollectionImage->AddLabelObject( this );
      }
    else
      {
      m_Label = label;
      }
    }

  const LabelCollectionImageType * GetLabelCollectionImage() const
    {
    return m_LabelCollectionImage;
    }

  LabelCollectionImageType * GetLabelCollectionImage()
    {
    return m_LabelCollectionImage;
    }

  void SetLabelCollectionImage( LabelCollectionImageType * labelCollectionImage )
    {
    if( labelCollectionImage == m_LabelCollectionImage )
      {
      return;
      }

    if( labelCollectionImage && labelCollectionImage->HasLabel( m_Label ) )
      {
      itkExceptionMacro(<< "The label is already used" );
      }

    if( m_LabelCollectionImage )
      {
      m_LabelCollectionImage->RemoveLabelObject( this );
      }

    m_LabelCollectionImage->AddLabelObject( this );
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

protected:
  

private:
  LabelObject(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  LabelCollectionImageType * m_LabelCollectionImage;
  LabelType m_Label;
  LineContainerType m_LineContainer;
};

} // end namespace itk

#endif
