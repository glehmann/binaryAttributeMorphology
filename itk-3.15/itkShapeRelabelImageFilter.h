/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkShapeRelabelImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShapeRelabelImageFilter_h
#define __itkShapeRelabelImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkShapeLabelObject.h"
#include "itkLabelMap.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "itkShapeLabelMapFilter.h"
#include "itkShapeRelabelLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"


namespace itk {

/** \class ShapeRelabelImageFilter
 * \brief relabel objects according to their shape attributes
 *
 * ShapeRelabelImageFilter relabel a labeled image according to the shape attributes of
 * the objects. The label produced are always consecutives.
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa ShapeLabelObject, RelabelComponentImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage>
class ITK_EXPORT ShapeRelabelImageFilter : 
    public ImageToImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef ShapeRelabelImageFilter                      Self;
  typedef ImageToImageFilter<TInputImage, TInputImage> Superclass;
  typedef SmartPointer<Self>                           Pointer;
  typedef SmartPointer<const Self>                     ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef TInputImage                              OutputImageType;
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

  typedef ShapeLabelObject<InputImagePixelType, ImageDimension>                     LabelObjectType;
  typedef typename itk::LabelMap< LabelObjectType >                                 LabelMapType;
  typedef typename itk::LabelImageToLabelMapFilter< InputImageType, LabelMapType >  LabelizerType;
  typedef typename itk::ShapeLabelMapFilter< LabelMapType >                         LabelObjectValuatorType;
  typedef typename LabelObjectType::AttributeType                                   AttributeType;
  typedef typename itk::ShapeRelabelLabelMapFilter< LabelMapType >                  RelabelType;
  typedef typename itk::LabelMapToLabelImageFilter< LabelMapType, OutputImageType > BinarizerType;

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(ShapeRelabelImageFilter, 
               ImageToImageFilter);

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
   * Set/Get the order of labeling of the objects. By default, the objects with
   * the highest attribute values are labeled first. Set ReverseOrdering to true
   * make the one with the smallest attributes be labeled first.
   */
  itkGetConstMacro( ReverseOrdering, bool );
  itkSetMacro( ReverseOrdering, bool );
  itkBooleanMacro( ReverseOrdering );

  /**
   * Set/Get the attribute to use. Default is "Size".
   */
  itkGetConstMacro( Attribute, AttributeType );
  itkSetMacro( Attribute, AttributeType );
  void SetAttribute( const std::string & s )
    {
    this->SetAttribute( LabelObjectType::GetAttributeFromName( s ) );
    }


protected:
  ShapeRelabelImageFilter();
  ~ShapeRelabelImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** ShapeRelabelImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion();

  /** ShapeRelabelImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();

private:
  ShapeRelabelImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  OutputImagePixelType m_BackgroundValue;
  bool                 m_ReverseOrdering;
  AttributeType        m_Attribute;
}; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkShapeRelabelImageFilter.txx"
#endif

#endif
