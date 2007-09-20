/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryImageToStatisticsLabelMapFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryImageToStatisticsLabelMapFilter_h
#define __itkBinaryImageToStatisticsLabelMapFilter_h

#include "itkImageToImageFilter.h"
#include "itkStatisticsLabelObject.h"
#include "itkLabelMap.h"
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkStatisticsLabelMapFilter.h"


namespace itk {

/** \class BinaryImageToStatisticsLabelMapFilter
 * \brief a convenient class to convert a binary image to a label map and valuate the statistics attributes at once
 *
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa StatisticsLabelObject, LabelStatisticsOpeningImageFilter, BinaryStatisticsOpeningImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TFeatureImage, class TOutputImage=LabelMap< StatisticsLabelObject< unsigned long, TInputImage::ImageDimension > > >
class ITK_EXPORT BinaryImageToStatisticsLabelMapFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef BinaryImageToStatisticsLabelMapFilter Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;

  typedef TOutputImage OutputImageType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  typedef typename OutputImageType::LabelObjectType LabelObjectType;
  
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

  typedef typename itk::BinaryImageToLabelMapFilter< InputImageType, OutputImageType > LabelizerType;
  typedef typename itk::StatisticsLabelMapFilter< OutputImageType, FeatureImageType > LabelObjectValuatorType;

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(BinaryImageToStatisticsLabelMapFilter, 
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
  itkSetMacro(ForegroundValue, InputImagePixelType);
  itkGetConstMacro(ForegroundValue, InputImagePixelType);

  /**
   * Set/Get whether the maximum Feret diameter should be computed or not. The
   * defaut value is false, because of the high computation time required.
   */
  itkSetMacro(ComputeFeretDiameter, bool);
  itkGetConstReferenceMacro(ComputeFeretDiameter, bool);
  itkBooleanMacro(ComputeFeretDiameter);

  /**
   * Set/Get whether the perimeter should be computed or not. The defaut value
   * is false, because of the high computation time required.
   */
  itkSetMacro(ComputePerimeter, bool);
  itkGetConstReferenceMacro(ComputePerimeter, bool);
  itkBooleanMacro(ComputePerimeter);

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
  BinaryImageToStatisticsLabelMapFilter();
  ~BinaryImageToStatisticsLabelMapFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** BinaryImageToStatisticsLabelMapFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** BinaryImageToStatisticsLabelMapFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  

private:
  BinaryImageToStatisticsLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool                m_FullyConnected;
  OutputImagePixelType m_BackgroundValue;
  InputImagePixelType m_ForegroundValue;
  bool m_ComputeFeretDiameter;
  bool m_ComputePerimeter;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryImageToStatisticsLabelMapFilter.txx"
#endif

#endif


