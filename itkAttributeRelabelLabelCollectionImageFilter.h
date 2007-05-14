/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAttributeRelabelLabelCollectionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAttributeRelabelLabelCollectionImageFilter_h
#define __itkAttributeRelabelLabelCollectionImageFilter_h

#include "itkInPlaceLabelCollectionImageFilter.h"
#include "itkAttributeLabelObject.h"

namespace itk {
/** \class AttributeRelabelLabelCollectionImageFilter
 * \brief relabel objects according to their shape attributes
 *
 * AttributeRelabelLabelCollectionImageFilter relabel a label collection image according to the
 * attribute of the objects.
 * The attribute is provide by an attribute accessor given in template parameter.
 *
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa AttributeLabelObject, RelabelComponentImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage, class TAttributeAccessor=
    typename Functor::AttributeLabelObjectAccessor< typename TImage::LabelObjectType > >
class ITK_EXPORT AttributeRelabelLabelCollectionImageFilter : 
    public InPlaceLabelCollectionImageFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef AttributeRelabelLabelCollectionImageFilter Self;
  typedef InPlaceLabelCollectionImageFilter<TImage>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TImage ImageType;
  typedef typename ImageType::Pointer         ImagePointer;
  typedef typename ImageType::ConstPointer    ImageConstPointer;
  typedef typename ImageType::PixelType       PixelType;
  typedef typename ImageType::IndexType       IndexType;
  typedef typename ImageType::LabelObjectType LabelObjectType;
  
  typedef TAttributeAccessor AttributeAccessorType;
  typedef typename AttributeAccessorType::AttributeValueType AttributeValueType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(AttributeRelabelLabelCollectionImageFilter, 
               InPlaceLabelCollectionImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
/*  itkConceptMacro(InputEqualityComparableCheck,
    (Concept::EqualityComparable<InputImagePixelType>));
  itkConceptMacro(IntConvertibleToInputCheck,
    (Concept::Convertible<int, InputImagePixelType>));
  itkConceptMacro(InputOStreamWritableCheck,
    (Concept::OStreamWritable<InputImagePixelType>));*/
  /** End concept checking */
#endif

  /**
   * Set/Get the order of labeling of the objects. By default, the objects with
   * the highest attribute values are labeled first. Set ReverseOrdering to true
   * make the one with the smallest attributes be labeled first.
   */
  itkSetMacro( ReverseOrdering, bool );
  itkGetConstReferenceMacro( ReverseOrdering, bool );
  itkBooleanMacro( ReverseOrdering );

protected:
  AttributeRelabelLabelCollectionImageFilter();
  ~AttributeRelabelLabelCollectionImageFilter() {};

  void GenerateData();

  void PrintSelf(std::ostream& os, Indent indent) const;

  class ReverseComparator
    {
    public:
    bool operator()( const typename LabelObjectType::Pointer & a, const typename LabelObjectType::Pointer & b )
      {
      return accessor( a ) < accessor( b );
      }
     AttributeAccessorType accessor;
    };

  class Comparator
    {
    public:
    bool operator()( const typename LabelObjectType::Pointer & a, const typename LabelObjectType::Pointer & b )
      {
      return accessor( a ) > accessor( b );
      }
     AttributeAccessorType accessor;
    };

private:
  AttributeRelabelLabelCollectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_ReverseOrdering;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAttributeRelabelLabelCollectionImageFilter.txx"
#endif

#endif


