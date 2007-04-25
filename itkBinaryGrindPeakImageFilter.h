/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryGrindPeakImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFillholeImageFilter_h
#define __itkFillholeImageFilter_h

#include "itkInPlaceImageFilter.h"

namespace itk {

/** \class BinaryGrindPeakImageFilter
 * \brief Remove local minima not connected to the boundary of the image.
 *
 * BinaryGrindPeakImageFilter fills holes in a grayscale image.
 * Holes are local minima in the grayscale topography that are not
 * connected to boundaries of the image. Gray level values adjacent to
 * a hole are extrapolated across the hole.
 *
 * This filter is used to smooth over local minima without affecting
 * the values of local maxima.  If you take the difference between the
 * output of this filter and the original image (and perhaps threshold
 * the difference above a small value), you'll obtain a map of the
 * local minima.
 *
 * This filter uses the GrayscaleGeodesicErodeImageFilter.  It
 * provides its own input as the "mask" input to the geodesic
 * erosion.  The "marker" image for the geodesic erosion is
 * constructed such that boundary pixels match the boundary pixels of
 * the input image and the interior pixels are set to the maximum
 * pixel value in the input image.
 *
 * Geodesic morphology and the Fillhole algorithm is described in
 * Chapter 6 of Pierre Soille's book "Morphological Image Analysis:
 * Principles and Applications", Second Edition, Springer, 2003.
 *
 * \sa GrayscaleGeodesicErodeImageFilter
 * \sa MorphologyImageFilter, GrayscaleErodeImageFilter, GrayscaleFunctionErodeImageFilter, BinaryErodeImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage>
class ITK_EXPORT BinaryGrindPeakImageFilter : 
    public InPlaceImageFilter<TInputImage, TInputImage>
{
public:
  /** Standard class typedefs. */
  typedef BinaryGrindPeakImageFilter Self;
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
                      OutputImageType::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(BinaryGrindPeakImageFilter, 
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
  itkConceptMacro(InputOStreamWritableCheck,
                  (Concept::OStreamWritable<InputImagePixelType>));
  /** End concept checking */
#endif

  /** Set the value in the image to consider as "foreground". Defaults to
   * maximum value of InputPixelType. */
  itkSetMacro(ForegroundValue, InputImagePixelType);
 
  /** Get the value in the image considered as "foreground". Defaults to
   * maximum value of InputPixelType. */
  itkGetMacro(ForegroundValue, InputImagePixelType);
 
  /** Set the value in eroded part of the image. Defaults to zero */
  itkSetMacro(BackgroundValue, InputImagePixelType);
 
  /** Set the value in eroded part of the image. Defaults to zero */
  itkGetMacro(BackgroundValue, InputImagePixelType);
 
  
protected:
  BinaryGrindPeakImageFilter();
  ~BinaryGrindPeakImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** BinaryGrindPeakImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** BinaryGrindPeakImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  

private:
  BinaryGrindPeakImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  InputImagePixelType m_ForegroundValue;

  InputImagePixelType m_BackgroundValue;

  bool                m_FullyConnected;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryGrindPeakImageFilter.txx"
#endif

#endif


