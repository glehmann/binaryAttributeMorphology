/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAttributeKeepNObjectsLabelCollectionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAttributeKeepNObjectsLabelCollectionImageFilter_h
#define __itkAttributeKeepNObjectsLabelCollectionImageFilter_h

#include "itkInPlaceLabelCollectionImageFilter.h"

namespace itk {
/** \class AttributeKeepNObjectsLabelCollectionImageFilter
 * \brief 
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage, class TAttributeAccessor >
class ITK_EXPORT AttributeKeepNObjectsLabelCollectionImageFilter : 
    public InPlaceLabelCollectionImageFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef AttributeKeepNObjectsLabelCollectionImageFilter Self;
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
  itkTypeMacro(AttributeKeepNObjectsLabelCollectionImageFilter, 
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

  itkSetMacro( ReverseOrdering, bool );
  itkGetConstReferenceMacro( ReverseOrdering, bool );
  itkBooleanMacro( ReverseOrdering );

  itkSetMacro( NumberOfObjects, unsigned long );
  itkGetConstReferenceMacro( NumberOfObjects, unsigned long );

protected:
  AttributeKeepNObjectsLabelCollectionImageFilter();
  ~AttributeKeepNObjectsLabelCollectionImageFilter() {};

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
  AttributeKeepNObjectsLabelCollectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_ReverseOrdering;
  unsigned long m_NumberOfObjects;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkAttributeKeepNObjectsLabelCollectionImageFilter.txx"
#endif

#endif


