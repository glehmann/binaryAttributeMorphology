/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryShapeOpeningImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryShapeOpeningImageFilter_h
#define __itkBinaryShapeOpeningImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkShapeLabelObject.h"
#include "itkLabelCollectionImage.h"
#include "itkBinaryImageToLabelCollectionImageFilter.h"
#include "itkShapeLabelCollectionImageFilter.h"
#include "itkShapeOpeningLabelCollectionImageFilter.h"
#include "itkLabelCollectionImageToBinaryImageFilter.h"


namespace itk {

/** \class BinaryShapeOpeningImageFilter
 * \brief Identify local maxima whose height above the baseline is greater than h.
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage>
class ITK_EXPORT BinaryShapeOpeningImageFilter : 
    public InPlaceImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef BinaryShapeOpeningImageFilter Self;
  typedef InPlaceImageFilter<TInputImage, TInputImage>
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
  
  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  typedef ShapeLabelObject<unsigned long, ImageDimension> LabelObjectType;
  typedef typename itk::LabelCollectionImage< LabelObjectType > LabelCollectionImageType;
  typedef typename itk::BinaryImageToLabelCollectionImageFilter< InputImageType, LabelCollectionImageType > LabelizerType;
  typedef typename itk::ShapeLabelCollectionImageFilter< LabelCollectionImageType > LabelObjectValuatorType;
  typedef typename LabelObjectType::AttributeType AttributeType;
  typedef typename itk::ShapeOpeningLabelCollectionImageFilter< LabelCollectionImageType > OpeningType;
  typedef typename itk::LabelCollectionImageToBinaryImageFilter< LabelCollectionImageType, OutputImageType > BinarizerType;

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(BinaryShapeOpeningImageFilter, 
               InPlaceImageFilter);

  /**
   * Set/Get whether the connected components are defined strictly by
   * face connectivity or by face+edge+vertex connectivity.  Default is
   * FullyConnectedOff.  For objects that are 1 pixel wide, use
   * FullyConnectedOn.
   */
  itkSetMacro(FullyConnected, bool);
  itkGetConstReferenceMacro(FullyConnected, bool);
  itkBooleanMacro(FullyConnected);

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

  /**
   * Set/Get the value used as "background" in the output image.
   * Defaults to NumericTraits<PixelType>::NonpositiveMin().
   */
  itkSetMacro(BackgroundValue, OutputImagePixelType);
  itkGetConstMacro(BackgroundValue, OutputImagePixelType);

  /**
   * Set/Get the value used as "foreground" in the output image.
   * Defaults to NumericTraits<PixelType>::max().
   */
  itkSetMacro(ForegroundValue, OutputImagePixelType);
  itkGetConstMacro(ForegroundValue, OutputImagePixelType);

  itkGetConstMacro(Lambda, double);
  itkSetMacro(Lambda, double);

  itkGetConstMacro( ReverseOrdering, bool );
  itkSetMacro( ReverseOrdering, bool );
  itkBooleanMacro( ReverseOrdering );

  itkGetConstMacro( Attribute, AttributeType );
  itkSetMacro( Attribute, AttributeType );

protected:
  BinaryShapeOpeningImageFilter();
  ~BinaryShapeOpeningImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** BinaryShapeOpeningImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** BinaryShapeOpeningImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  

private:
  BinaryShapeOpeningImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool                m_FullyConnected;
  OutputImagePixelType m_BackgroundValue;
  OutputImagePixelType m_ForegroundValue;
  double m_Lambda;
  bool m_ReverseOrdering;
  AttributeType m_Attribute;
} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryShapeOpeningImageFilter.txx"
#endif

#endif


