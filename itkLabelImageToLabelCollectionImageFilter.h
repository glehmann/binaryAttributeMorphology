/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelImageToLabelCollectionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelImageToLabelCollectionImageFilter_h
#define __itkLabelImageToLabelCollectionImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk {

/** \class LabelImageToLabelCollectionImageFilter
 * \brief convert a labeled image to a label collection image
 *
 * LabelImageToLabelCollectionImageFilter converts a label image to a label collection image.
 * The labels are the same in the input and the output image.
 * 
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa BinaryImageToLabelCollectionImageFilter, LabelCollectionImageToLabelImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TOutputImage>
class ITK_EXPORT LabelImageToLabelCollectionImageFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelImageToLabelCollectionImageFilter Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage>
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
  
  typedef typename OutputImageType::Pointer         OutputImagePointer;
  typedef typename OutputImageType::ConstPointer    OutputImageConstPointer;
  typedef typename OutputImageType::RegionType      OutputImageRegionType;
  typedef typename OutputImageType::PixelType       OutputImagePixelType;
  typedef typename OutputImageType::LabelObjectType LabelObjectType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(LabelImageToLabelCollectionImageFilter, 
               ImageToImageFilter);

  /**
   * Set/Get the value used as "background" in the output image.
   * Defaults to NumericTraits<PixelType>::NonpositiveMin().
   */
  itkSetMacro(BackgroundValue, OutputImagePixelType);
  itkGetConstMacro(BackgroundValue, OutputImagePixelType);

  /**
   * Set/Get wether the LabelCollectionImage produced should use the background
   * or not (true by default).
   */
  itkSetMacro(UseBackground, bool);
  itkGetConstMacro(UseBackground, bool);
  itkBooleanMacro(UseBackground);

protected:
  LabelImageToLabelCollectionImageFilter();
  ~LabelImageToLabelCollectionImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** LabelImageToLabelCollectionImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** LabelImageToLabelCollectionImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  virtual void BeforeThreadedGenerateData();

  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId );

  virtual void AfterThreadedGenerateData();
  

private:
  LabelImageToLabelCollectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_UseBackground;
  OutputImagePixelType m_BackgroundValue;
  
  typename std::vector< OutputImagePointer > m_TemporaryImages;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLabelImageToLabelCollectionImageFilter.txx"
#endif

#endif


