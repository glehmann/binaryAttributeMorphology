/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelCollectionImageToBinaryImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelCollectionImageToBinaryImageFilter_h
#define __itkLabelCollectionImageToBinaryImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk {

/** \class LabelCollectionImageToBinaryImageFilter
 * \brief Convert a LabelCollectionImage to a binary image
 *
 * LabelCollectionImageToBinaryImageFilter to a binary image. All the objects in the image
 * are used as foreground.
 * The background values of the original binary image can be restored by passing this image
 * to the filter with the SetBackgroundImage() method.
 *
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa LabelCollectionImageToLabelImageFilter, LabelCollectionImageToMaskImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TOutputImage>
class ITK_EXPORT LabelCollectionImageToBinaryImageFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelCollectionImageToBinaryImageFilter Self;
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
  typedef typename OutputImageType::IndexType      IndexType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(LabelCollectionImageToBinaryImageFilter, 
               ImageToImageFilter);

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

   /** Set/Get the background image top be used to restore the background values */
  void SetBackgroundImage( const OutputImageType *input)
     {
     // Process object is not const-correct so the const casting is required.
     this->SetNthInput( 1, const_cast<OutputImageType *>(input) );
     }

  OutputImageType * GetBackgroundImage()
    {
    return static_cast<OutputImageType*>(const_cast<DataObject *>(this->ProcessObject::GetInput(1)));
    }

   /** Set the input image */
  void SetInput1( const InputImageType * input )
     {
     this->SetInput( input );
     }

   /** Set the marker image */
  void SetInput2( const OutputImageType * input )
     {
     this->SetBackgroundImage( input );
     }

protected:
  LabelCollectionImageToBinaryImageFilter();
  ~LabelCollectionImageToBinaryImageFilter() {};

  /** LabelCollectionImageToBinaryImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** LabelCollectionImageToBinaryImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  

private:
  LabelCollectionImageToBinaryImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  OutputImagePixelType m_BackgroundValue;
  OutputImagePixelType m_ForegroundValue;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLabelCollectionImageToBinaryImageFilter.txx"
#endif

#endif


