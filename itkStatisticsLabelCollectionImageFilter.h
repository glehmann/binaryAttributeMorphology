/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkStatisticsLabelCollectionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkStatisticsLabelCollectionImageFilter_h
#define __itkStatisticsLabelCollectionImageFilter_h

#include "itkShapeLabelCollectionImageFilter.h"

namespace itk {
/** \class StatisticsLabelCollectionImageFilter
 * \brief 
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage, class TFeatureImage>
class ITK_EXPORT StatisticsLabelCollectionImageFilter : 
    public ShapeLabelCollectionImageFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef StatisticsLabelCollectionImageFilter Self;
  typedef ShapeLabelCollectionImageFilter<TImage>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TImage ImageType;
  typedef typename ImageType::Pointer         ImagePointer;
  typedef typename ImageType::ConstPointer    ImageConstPointer;
  typedef typename ImageType::PixelType       PixelType;
  typedef typename ImageType::IndexType       IndexType;
  typedef typename ImageType::PointType       PointType;
  typedef typename ImageType::LabelObjectType::MatrixType       MatrixType;
  typedef typename ImageType::LabelObjectType::VectorType       VectorType;
  
  typedef TFeatureImage FeatureImageType;
  typedef typename FeatureImageType::Pointer         FeatureImagePointer;
  typedef typename FeatureImageType::ConstPointer    FeatureImageConstPointer;
  typedef typename FeatureImageType::PixelType       FeatureImagePixelType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(StatisticsLabelCollectionImageFilter, 
               ShapeLabelCollectionImageFilter);

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

   /** Set the marker image */
  void SetFeatureImage(TFeatureImage *input)
     {
     // Process object is not const-correct so the const casting is required.
     this->SetNthInput( 1, const_cast<TFeatureImage *>(input) );
     }

  /** Get the marker image */
  FeatureImageType * GetFeatureImage()
    {
    return static_cast<FeatureImageType*>(const_cast<DataObject *>(this->ProcessObject::GetInput(1)));
    }

   /** Set the input image */
  void SetInput1(TImage *input)
     {
     this->SetInput( input );
     }

   /** Set the marker image */
  void SetInput2(TFeatureImage *input)
     {
     this->SetFeatureImage( input );
     }

protected:
  StatisticsLabelCollectionImageFilter();
  ~StatisticsLabelCollectionImageFilter() {};

  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  
private:
  StatisticsLabelCollectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkStatisticsLabelCollectionImageFilter.txx"
#endif

#endif


