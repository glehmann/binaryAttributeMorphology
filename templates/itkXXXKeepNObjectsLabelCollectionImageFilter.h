/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkXXXKeepNObjectsLabelCollectionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkXXXKeepNObjectsLabelCollectionImageFilter_h
#define __itkXXXKeepNObjectsLabelCollectionImageFilter_h

#include "itkAttributeKeepNObjectsLabelCollectionImageFilter.h"
#include "itkShapeLabelObject.h"

namespace itk {
/** \class XXXKeepNObjectsLabelCollectionImageFilter
 * \brief 
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage >
class ITK_EXPORT XXXKeepNObjectsLabelCollectionImageFilter : 
    public AttributeKeepNObjectsLabelCollectionImageFilter<TImage, Functor::XXXLabelObjectAccessor< typename TImage::LabelObjectType > >
{
public:
  /** Standard class typedefs. */
  typedef XXXKeepNObjectsLabelCollectionImageFilter Self;
  typedef AttributeKeepNObjectsLabelCollectionImageFilter<TImage, Functor::XXXLabelObjectAccessor< typename TImage::LabelObjectType > >
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
  
  typedef typename Superclass::AttributeAccessorType AttributeAccessorType;
  typedef typename AttributeAccessorType::AttributeType AttributeType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(XXXKeepNObjectsLabelCollectionImageFilter, 
               AttributeKeepNObjectsLabelCollectionImageFilter);

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

protected:
  XXXKeepNObjectsLabelCollectionImageFilter() {};
  ~XXXKeepNObjectsLabelCollectionImageFilter() {};

private:
  XXXKeepNObjectsLabelCollectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

} ; // end of class

} // end namespace itk
  
#endif


