/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelCollectionImageToMaskImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelCollectionImageToMaskImageFilter_h
#define __itkLabelCollectionImageToMaskImageFilter_h

#include "itkLabelCollectionImageFilter.h"
#include "itkBarrier.h"

namespace itk {

/** \class LabelCollectionImageToMaskImageFilter
 * \brief Mask and image with a LabelCollectionImage
 *
 * LabelCollectionImageToMaskImageFilter mask the content of an input image according
 * to the content of the input LabelCollectionImage. The masked pixel of the input image
 * are set to the BackgroundValue.
 * LabelCollectionImageToMaskImageFilter can keep the input image for one label only, with
 * Negated = false (the default) or it can mask the input image for a single label, when
 * Negated equals true. In Both cases, the label is set with SetLabel(). 
 *
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa LabelCollectionImageToBinaryImageFilter, LabelCollectionImageToLabelImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TOutputImage>
class ITK_EXPORT LabelCollectionImageToMaskImageFilter : 
    public LabelCollectionImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelCollectionImageToMaskImageFilter Self;
  typedef LabelCollectionImageFilter<TInputImage, TOutputImage>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename InputImageType::LabelObjectType LabelObjectType;

  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  typedef typename OutputImageType::IndexType      IndexType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(LabelCollectionImageToMaskImageFilter, 
               ImageToImageFilter);

   /** Set the feature image */
  void SetFeatureImage(TOutputImage *input)
     {
     // Process object is not const-correct so the const casting is required.
     this->SetNthInput( 1, const_cast<TOutputImage *>(input) );
     }

  /** Get the feature image */
  OutputImageType * GetFeatureImage()
    {
    return static_cast<OutputImageType*>(const_cast<DataObject *>(this->ProcessObject::GetInput(1)));
    }

   /** Set the input image */
  void SetInput1(TInputImage *input)
     {
     this->SetInput( input );
     }

   /** Set the feature image */
  void SetInput2(TOutputImage *input)
     {
     this->SetFeatureImage( input );
     }

  /**
   * Set/Get the value used as "background" in the output image.
   * Defaults to NumericTraits<PixelType>::Zero.
   */
  itkSetMacro(BackgroundValue, OutputImagePixelType);
  itkGetConstMacro(BackgroundValue, OutputImagePixelType);

  /**
   * The label to mask or to not mask, depending on the value of the Negated ivar.
   */
  itkSetMacro(Label, InputImagePixelType);
  itkGetConstMacro(Label, InputImagePixelType);

  /**
   * Set/Get whether the Label should be masked or not.
   */
  itkSetMacro(Negated, bool);
  itkGetConstReferenceMacro(Negated, bool);
  itkBooleanMacro(Negated);

protected:
  LabelCollectionImageToMaskImageFilter();
  ~LabelCollectionImageToMaskImageFilter() {};

  /** LabelCollectionImageToMaskImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** LabelCollectionImageToMaskImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));

  virtual void GenerateOutputInformation();
  
  virtual void BeforeThreadedGenerateData();

  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId );

  virtual void ThreadedGenerateData( LabelObjectType * labelObject );
  

private:
  LabelCollectionImageToMaskImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  InputImagePixelType m_Label;
  OutputImagePixelType m_BackgroundValue;
  bool m_Negated;
  bool m_Crop;

  TimeStamp m_CropTimeStamp;
  IndexType m_OutputIndex;
  SizeType m_OutputSize;

  typename Barrier::Pointer m_Barrier;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLabelCollectionImageToMaskImageFilter.txx"
#endif

#endif


