/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryXXXKeepNObjectsImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryXXXKeepNObjectsImageFilter_h
#define __itkBinaryXXXKeepNObjectsImageFilter_h

#include "itkBinaryAttributeKeepNObjectsImageFilter.h"
#include "itkShapeLabelCollectionImageFilter.h"
#include "itkShapeLabelObject.h"

namespace itk {

/** \class BinaryXXXKeepNObjectsImageFilter
 * \brief Identify local maxima whose height above the baseline is greater than h.
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage>
class ITK_EXPORT BinaryXXXKeepNObjectsImageFilter : 
    public BinaryAttributeKeepNObjectsImageFilter<TInputImage,
      ShapeLabelObject< unsigned long, TInputImage::ImageDimension >,
      ShapeLabelCollectionImageFilter< LabelCollectionImage< ShapeLabelObject< unsigned long, TInputImage::ImageDimension > > >,
      typename Functor::XXXLabelObjectAccessor< ShapeLabelObject< unsigned long, TInputImage::ImageDimension > > >
{
public:
  /** Standard class typedefs. */
  typedef BinaryXXXKeepNObjectsImageFilter Self;
  typedef BinaryAttributeKeepNObjectsImageFilter<TInputImage,
      ShapeLabelObject< unsigned long, TInputImage::ImageDimension >,
      ShapeLabelCollectionImageFilter< LabelCollectionImage< ShapeLabelObject< unsigned long, TInputImage::ImageDimension > > >,
      typename Functor::XXXLabelObjectAccessor< ShapeLabelObject< unsigned long, TInputImage::ImageDimension > > >
      Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef TInputImage OutputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  
  typedef typename Superclass::LabelObjectType LabelObjectType;
  typedef typename Superclass::LabelCollectionImageType LabelCollectionImageType;
  typedef typename Superclass::LabelizerType LabelizerType;
  typedef typename Superclass::LabelObjectValuatorType LabelObjectValuatorType;
  typedef typename Superclass::LabelObjectValuatorType AttributeAccessorType;
  typedef typename Superclass::AttributeType AttributeType;
  typedef typename Superclass::KeepNObjectsType KeepNObjectsType;
  typedef typename Superclass::BinarizerType BinarizerType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(BinaryXXXKeepNObjectsImageFilter, 
               BinaryAttributeKeepNObjectsImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
  itkConceptMacro(InputEqualityComparableCheck,
    (Concept::EqualityComparable<InputImagePixelType>));
  itkConceptMacro(IntConvertibleToInputCheck,
    (Concept::Convertible<int, InputImagePixelType>));
  itkConceptMacro(InputOStreamWritableCheck,
    (Concept::OStreamWritable<InputImagePixelType>));
  /** End concept checking */
#endif


protected:
  BinaryXXXKeepNObjectsImageFilter() {};
  ~BinaryXXXKeepNObjectsImageFilter() {};

private:
  BinaryXXXKeepNObjectsImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

} ; // end of class

} // end namespace itk
  
#endif

