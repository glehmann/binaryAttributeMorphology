/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkShapeLabelCollectionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShapeLabelCollectionImageFilter_h
#define __itkShapeLabelCollectionImageFilter_h

#include "itkInPlaceLabelCollectionImageFilter.h"

namespace itk {
/** \class ShapeLabelCollectionImageFilter
 * \brief The valuator class for the ShapeLabelObject
 *
 * ShapeLabelCollectionImageFilter can be used to set the attributes values
 * of the ShapeLabelObject in a LabelCollectionImage.
 *
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage>
class ITK_EXPORT ShapeLabelCollectionImageFilter : 
    public InPlaceLabelCollectionImageFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef ShapeLabelCollectionImageFilter Self;
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
  typedef typename ImageType::SizeType        SizeType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(ShapeLabelCollectionImageFilter, 
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
   * Set/Get whether the maximum Feret diameter should be computed by default
   * or not. The defaut value in true for 2D images, and false for the other,
   * because of the high computation time required for that last case.
   */
  itkSetMacro(ComputeFeretDiameter, bool);
  itkGetConstReferenceMacro(ComputeFeretDiameter, bool);
  itkBooleanMacro(ComputeFeretDiameter);

  /**
   * Set/Get whether the perimeter should be computed by default
   * or not. The defaut value in true for 2D images, and false for the other,
   * because of the high computation time required for that last case.
   */
  itkSetMacro(ComputePerimeter, bool);
  itkGetConstReferenceMacro(ComputePerimeter, bool);
  itkBooleanMacro(ComputePerimeter);


protected:
  ShapeLabelCollectionImageFilter();
  ~ShapeLabelCollectionImageFilter() {};

  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  
  void GenerateExtendedData();

private:
  ShapeLabelCollectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_ComputeFeretDiameter;

  bool m_ComputePerimeter;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkShapeLabelCollectionImageFilter.txx"
#endif

#endif


