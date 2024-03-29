/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryStatisticsKeepNObjectsImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryStatisticsKeepNObjectsImageFilter_h
#define __itkBinaryStatisticsKeepNObjectsImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkStatisticsLabelObject.h"
#include "itkLabelMap.h"
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkStatisticsLabelMapFilter.h"
#include "itkStatisticsKeepNObjectsLabelMapFilter.h"
#include "itkLabelMapToBinaryImageFilter.h"


namespace itk {

/** \class BinaryStatisticsKeepNObjectsImageFilter
 * \brief keep N objects according to their statistics attributes
 *
 * BinaryStatisticsKeepNObjectsImageFilter keep the N objects in a binary image
 * with the highest (or lowest) attribute value. The attributes are the ones
 * of the StatisticsLabelObject.
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa StatisticsLabelObject, LabelStatisticsKeepNObjectsImageFilter, BinaryShapeKeepNObjectsImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TFeatureImage>
class ITK_EXPORT BinaryStatisticsKeepNObjectsImageFilter : 
    public ImageToImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef BinaryStatisticsKeepNObjectsImageFilter      Self;
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
  
  typedef TFeatureImage                              FeatureImageType;
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

  typedef StatisticsLabelObject<unsigned long, ImageDimension>                       LabelObjectType;
  typedef typename itk::LabelMap< LabelObjectType >                                  LabelMapType;
  typedef typename itk::BinaryImageToLabelMapFilter< InputImageType, LabelMapType >  LabelizerType;
  typedef typename itk::StatisticsLabelMapFilter< LabelMapType, TFeatureImage >      LabelObjectValuatorType;
  typedef typename LabelObjectType::AttributeType                                    AttributeType;
  typedef typename itk::StatisticsKeepNObjectsLabelMapFilter< LabelMapType >         KeepNObjectsType;
  typedef typename itk::LabelMapToBinaryImageFilter< LabelMapType, OutputImageType > BinarizerType;

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(BinaryStatisticsKeepNObjectsImageFilter, 
               ImageToImageFilter);

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

  /**
   * Set/Get the number of objects to keep
   */
  itkGetConstMacro(NumberOfObjects, unsigned long);
  itkSetMacro(NumberOfObjects, unsigned long);

  /**
   * Set/Get the ordering of the objects. By default, the ones with the
   * highest value are kept. Turming ReverseOrdering to true make this filter
   * keep the objects with the smallest values
   */
  itkGetConstMacro( ReverseOrdering, bool );
  itkSetMacro( ReverseOrdering, bool );
  itkBooleanMacro( ReverseOrdering );

  /**
   * Set/Get the attribute to use to select the object to keep. The default
   * is "Mean".
   */
  itkGetConstMacro( Attribute, AttributeType );
  itkSetMacro( Attribute, AttributeType );
  void SetAttribute( const std::string & s )
    {
    this->SetAttribute( LabelObjectType::GetAttributeFromName( s ) );
    }


   /** Set the feature image */
  void SetFeatureImage(TFeatureImage *input)
    {
    // Process object is not const-correct so the const casting is required.
    this->SetNthInput( 1, const_cast<TFeatureImage *>(input) );
    }

  /** Get the feature image */
  FeatureImageType * GetFeatureImage()
    {
    return static_cast<FeatureImageType*>(const_cast<DataObject *>(this->ProcessObject::GetInput(1)));
    }

   /** Set the input image */
  void SetInput1(InputImageType *input)
    {
    this->SetInput( input );
    }

  /** Set the feature image */
  void SetInput2(FeatureImageType *input)
    {
    this->SetFeatureImage( input );
    }

protected:
  BinaryStatisticsKeepNObjectsImageFilter();
  ~BinaryStatisticsKeepNObjectsImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** BinaryStatisticsKeepNObjectsImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion();

  /** BinaryStatisticsKeepNObjectsImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  
private:
  BinaryStatisticsKeepNObjectsImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool                 m_FullyConnected;
  OutputImagePixelType m_BackgroundValue;
  OutputImagePixelType m_ForegroundValue;
  unsigned long        m_NumberOfObjects;
  bool                 m_ReverseOrdering;
  AttributeType        m_Attribute;
}; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryStatisticsKeepNObjectsImageFilter.txx"
#endif

#endif
