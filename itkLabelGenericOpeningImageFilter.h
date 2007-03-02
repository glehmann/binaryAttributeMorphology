/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelGenericOpeningImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelGenericOpeningImageFilter_h
#define __itkLabelGenericOpeningImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkLabelCollectionImage.h"
#include "itkLabelImageToLabelCollectionImageFilter.h"
#include "itkGenericOpeningLabelCollectionImageFilter.h"
#include "itkLabelCollectionImageToLabelImageFilter.h"


namespace itk {

/** \class LabelGenericOpeningImageFilter
 * \brief Identify local maxima whose height above the baseline is greater than h.
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TLabelObject, class TLabelObjectValuator, class TAttributeAccessor>
class ITK_EXPORT LabelGenericOpeningImageFilter : 
    public InPlaceImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelGenericOpeningImageFilter Self;
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

  typedef TLabelObject LabelObjectType;
  typedef typename itk::LabelCollectionImage< LabelObjectType > LabelCollectionImageType;
  typedef typename itk::LabelImageToLabelCollectionImageFilter< InputImageType, LabelCollectionImageType > LabelizerType;
  typedef TLabelObjectValuator LabelObjectValuatorType;
  typedef TAttributeAccessor AttributeAccessorType;
  typedef typename AttributeAccessorType::AttributeType AttributeType;
  typedef typename itk::GenericOpeningLabelCollectionImageFilter< LabelCollectionImageType, AttributeAccessorType > OpeningType;
  typedef typename itk::LabelCollectionImageToLabelImageFilter< LabelCollectionImageType, OutputImageType > BinarizerType;

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(LabelGenericOpeningImageFilter, 
               InPlaceImageFilter);

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

  itkGetConstMacro(Lambda, AttributeType);
  itkSetMacro(Lambda, AttributeType);

  itkGetConstMacro( ReverseOrdering, bool );
  itkSetMacro( ReverseOrdering, bool );
  itkBooleanMacro( ReverseOrdering );

protected:
  LabelGenericOpeningImageFilter();
  ~LabelGenericOpeningImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** LabelGenericOpeningImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** LabelGenericOpeningImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  
  virtual void CustomizeInternalFilters( LabelizerType *, LabelObjectValuatorType *, OpeningType *, BinarizerType* ) {};


private:
  LabelGenericOpeningImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OutputImagePixelType m_BackgroundValue;
  AttributeType m_Lambda;
  bool m_ReverseOrdering;
} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLabelGenericOpeningImageFilter.txx"
#endif

#endif


