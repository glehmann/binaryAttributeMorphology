/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelVarianceOpeningImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelVarianceOpeningImageFilter_h
#define __itkLabelVarianceOpeningImageFilter_h

#include "itkLabelAttributeOpeningImageFilter.h"
#include "itkStatisticsLabelCollectionImageFilter.h"
#include "itkStatisticsLabelObject.h"

namespace itk {

/** \class LabelVarianceOpeningImageFilter
 * \brief Identify local maxima whose height above the baseline is greater than h.
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TFeatureImage>
class ITK_EXPORT LabelVarianceOpeningImageFilter : 
    public LabelAttributeOpeningImageFilter<TInputImage,
      StatisticsLabelObject< unsigned long, TInputImage::ImageDimension >,
      StatisticsLabelCollectionImageFilter< LabelCollectionImage< StatisticsLabelObject< unsigned long, TInputImage::ImageDimension > >, TFeatureImage >,
      typename Functor::VarianceLabelObjectAccessor< StatisticsLabelObject< unsigned long, TInputImage::ImageDimension > > >
{
public:
  /** Standard class typedefs. */
  typedef LabelVarianceOpeningImageFilter Self;
  typedef LabelAttributeOpeningImageFilter<TInputImage,
      StatisticsLabelObject< unsigned long, TInputImage::ImageDimension >,
      StatisticsLabelCollectionImageFilter< LabelCollectionImage< StatisticsLabelObject< unsigned long, TInputImage::ImageDimension > >, TFeatureImage >,
      typename Functor::VarianceLabelObjectAccessor< StatisticsLabelObject< unsigned long, TInputImage::ImageDimension > > >
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
  typedef typename Superclass::OpeningType OpeningType;
  typedef typename Superclass::BinarizerType BinarizerType;

  typedef TFeatureImage FeatureImageType;
  typedef typename FeatureImageType::Pointer         FeatureImagePointer;
  typedef typename FeatureImageType::ConstPointer    FeatureImageConstPointer;
  typedef typename FeatureImageType::PixelType       FeatureImagePixelType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(LabelVarianceOpeningImageFilter, 
               LabelAttributeOpeningImageFilter);

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
  LabelVarianceOpeningImageFilter() {};
  ~LabelVarianceOpeningImageFilter() {};

  virtual void CustomizeInternalFilters( LabelizerType *, LabelObjectValuatorType * valuator, OpeningType *, BinarizerType* )
    {
    valuator->SetFeatureImage( this->GetFeatureImage() );
    }

private:
  LabelVarianceOpeningImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

} ; // end of class

} // end namespace itk
  
#endif


