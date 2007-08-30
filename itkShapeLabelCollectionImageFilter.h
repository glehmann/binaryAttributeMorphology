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
#include "itkLabelPerimeterEstimationCalculator.h"

#include "itkInPlaceLabelCollectionImageFilter.h"

#define PI 3.14159265358979323846

namespace itk {
/** \class ShapeLabelCollectionImageFilter
 * \brief The valuator class for the ShapeLabelObject
 *
 * ShapeLabelCollectionImageFilter can be used to set the attributes values
 * of the ShapeLabelObject in a LabelCollectionImage.
 *
 * ShapeLabelCollectionImageFilter take an optional parameter, used only to optimize
 * the computation time and the memory usage when the perimeter or the feret diameter
 * are used: the exact copy of the input LabelCollectionImage but stored in an Image.
 * It can be set with SetLabelImage(). It is cleared at the end of the computation, and
 * so must be reset before running Update() again. It is not part of the pipeline management
 * design, to let the subclasses of ShapeLabelCollectionImageFilter use the
 * pipeline desing to specify a really required input.
 *
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage, class TLabelImage=Image< typename TImage::PixelType, TImage::ImageDimension > >
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
  typedef typename ImageType::LabelObjectType LabelObjectType;
  typedef typename LabelObjectType::MatrixType MatrixType;
  typedef typename LabelObjectType::VectorType VectorType;
  
  typedef TLabelImage LabelImageType;
  typedef typename LabelImageType::Pointer         LabelImagePointer;
  typedef typename LabelImageType::ConstPointer    LabelImageConstPointer;
  typedef typename LabelImageType::PixelType       LabelPixelType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  typedef LabelPerimeterEstimationCalculator< LabelImageType > PerimeterCalculatorType;

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


  /** Set the label image */
  void SetLabelImage( const TLabelImage *input )
     {
     m_LabelImage = input;
     }

  /** */
  static long factorial( long n );

  /** */
  static long doubleFactorial( long n );

  /** */
  static double gammaN2p1( long n );

  /** */
  static double hyperSphereVolume( double radius );

  /** */
  static double hyperSphereArea( double radius );

  /** */
  static double hyperSphereRadiusFromVolume( double radius );


protected:
  ShapeLabelCollectionImageFilter();
  ~ShapeLabelCollectionImageFilter() {};

  virtual void ThreadedGenerateData( LabelObjectType * labelObject );
  
  virtual void BeforeThreadedGenerateData();

  virtual void AfterThreadedGenerateData();

  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  ShapeLabelCollectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_ComputeFeretDiameter;

  bool m_ComputePerimeter;

  LabelImageConstPointer m_LabelImage;

  typename PerimeterCalculatorType::Pointer m_PerimeterCalculator;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkShapeLabelCollectionImageFilter.txx"
#endif

#endif


