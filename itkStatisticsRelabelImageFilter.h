/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkStatisticsRelabelImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkStatisticsRelabelImageFilter_h
#define __itkStatisticsRelabelImageFilter_h

#include "itkInPlaceImageFilter.h"
#include "itkStatisticsLabelObject.h"
#include "itkLabelCollectionImage.h"
#include "itkLabelImageToLabelCollectionImageFilter.h"
#include "itkStatisticsLabelCollectionImageFilter.h"
#include "itkStatisticsRelabelLabelCollectionImageFilter.h"
#include "itkLabelCollectionImageToLabelImageFilter.h"


namespace itk {

/** \class StatisticsRelabelImageFilter
 * \brief Identify local maxima whose height above the baseline is greater than h.
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TFeatureImage>
class ITK_EXPORT StatisticsRelabelImageFilter : 
    public InPlaceImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef StatisticsRelabelImageFilter Self;
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
  
  typedef TFeatureImage FeatureImageType;
  typedef typename FeatureImageType::Pointer         FeatureImagePointer;
  typedef typename FeatureImageType::ConstPointer    FeatureImageConstPointer;
  typedef typename FeatureImageType::PixelType       FeatureImagePixelType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  typedef StatisticsLabelObject<unsigned long, ImageDimension> LabelObjectType;
  typedef typename itk::LabelCollectionImage< LabelObjectType > LabelCollectionImageType;
  typedef typename itk::LabelImageToLabelCollectionImageFilter< InputImageType, LabelCollectionImageType > LabelizerType;
  typedef typename itk::StatisticsLabelCollectionImageFilter< LabelCollectionImageType, TFeatureImage > LabelObjectValuatorType;
  typedef typename LabelObjectType::AttributeType AttributeType;
  typedef typename itk::StatisticsRelabelLabelCollectionImageFilter< LabelCollectionImageType > RelabelType;
  typedef typename itk::LabelCollectionImageToLabelImageFilter< LabelCollectionImageType, OutputImageType > BinarizerType;

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(StatisticsRelabelImageFilter, 
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

  itkGetConstMacro( ReverseOrdering, bool );
  itkSetMacro( ReverseOrdering, bool );
  itkBooleanMacro( ReverseOrdering );

  itkGetConstMacro( UseBackground, bool );
  itkSetMacro( UseBackground, bool );
  itkBooleanMacro( UseBackground );

  itkGetConstMacro( Attribute, AttributeType );
  itkSetMacro( Attribute, AttributeType );
  void SetAttribute( const std::string & s )
    {
    this->SetAttribute( LabelObjectType::GetAttributeFromName( s ) );
    }


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
  void SetInput1(InputImageType *input)
     {
     this->SetInput( input );
     }

   /** Set the marker image */
  void SetInput2(FeatureImageType *input)
     {
     this->SetFeatureImage( input );
     }

protected:
  StatisticsRelabelImageFilter();
  ~StatisticsRelabelImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** StatisticsRelabelImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** StatisticsRelabelImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();

private:
  StatisticsRelabelImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OutputImagePixelType m_BackgroundValue;
  bool m_UseBackground;
  bool m_ReverseOrdering;
  AttributeType m_Attribute;
} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkStatisticsRelabelImageFilter.txx"
#endif

#endif

