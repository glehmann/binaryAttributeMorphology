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
 * \brief Produce a binary image where foreground is the regional maxima of the input image
 *
 * Regional maxima are flat zones surounded by pixels of lower value.
 *
 * If the input image is constant, the entire image can be considered as a maxima or not.
 * The desired behavior can be selected with the SetFlatIsMaxima() method.
 * 
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa ValuedLabelImageToLabelCollectionImageFilter, HConvexImageFilter, RegionalMinimaImageFilter
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
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  
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
  itkSetMacro(BackgroundLabel, OutputImagePixelType);
  itkGetConstMacro(BackgroundLabel, OutputImagePixelType);

  /**
   * Set/Get wether a flat image must be considered as a maxima or not.
   * Defaults to true.
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
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  

private:
  LabelImageToLabelCollectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_UseBackground;
  OutputImagePixelType m_BackgroundLabel;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLabelImageToLabelCollectionImageFilter.txx"
#endif

#endif


