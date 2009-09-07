/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelMapContourOverlayImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelMapContourOverlayImageFilter_h
#define __itkLabelMapContourOverlayImageFilter_h

#include "itkLabelMapFilter.h"
#include "itkBarrier.h"
#include "itkLabelOverlayFunctor.h"

namespace itk {

/** \class LabelMapContourOverlayImageFilter
 * \brief Mask and image with a LabelMap
 *
 * LabelMapContourOverlayImageFilter mask the content of an input image according
 * to the content of the input LabelMap. The masked pixel of the input image
 * are set to the BackgroundValue.
 * LabelMapContourOverlayImageFilter can keep the input image for one label only, with
 * Negated = false (the default) or it can mask the input image for a single label, when
 * Negated equals true. In Both cases, the label is set with SetLabel(). 
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa LabelMapToBinaryImageFilter, LabelMapToLabelImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TFeatureImage, class TOutputImage=Image< RGBPixel< typename TFeatureImage::PixelType >, TFeatureImage::ImageDimension > >
class ITK_EXPORT LabelMapContourOverlayImageFilter : 
    public LabelMapFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef LabelMapContourOverlayImageFilter                Self;
  typedef LabelMapFilter<TInputImage, TOutputImage> Superclass;
  typedef SmartPointer<Self>                        Pointer;
  typedef SmartPointer<const Self>                  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage                              InputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename InputImageType::LabelObjectType LabelObjectType;
  typedef typename LabelObjectType::LabelType      LabelType;

  typedef TFeatureImage                             FeatureImageType;
  typedef typename FeatureImageType::Pointer        FeatureImagePointer;
  typedef typename FeatureImageType::ConstPointer   FeatureImageConstPointer;
  typedef typename FeatureImageType::RegionType     FeatureImageRegionType;
  typedef typename FeatureImageType::PixelType      FeatureImagePixelType;

  typedef TOutputImage                             OutputImageType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  typedef typename OutputImageType::IndexType      IndexType;
  typedef typename OutputImageType::SizeType       SizeType;
  typedef typename OutputImageType::RegionType     RegionType;
  
  typedef typename Functor::LabelOverlayFunctor<FeatureImagePixelType, InputImagePixelType, OutputImagePixelType> FunctorType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  enum
    {
    PLAIN = 0,
    COUNTOUR = 1,
    SLICE_CONTOUR=2
    };

  enum
    {
    HIGH_LABEL_ON_TOP = 0,
    LOW_LABEL_ON_TOP = 1,
    };

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(LabelMapContourOverlayImageFilter, 
               ImageToImageFilter);

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
  void SetInput1(TInputImage *input)
    {
    this->SetInput( input );
    }

  /** Set the feature image */
  void SetInput2(TFeatureImage *input)
    {
    this->SetFeatureImage( input );
    }

  /** Set/Get the opacity of the colored label image. The value must be
   * between 0 and 1
   */
  itkSetMacro( Opacity, double );
  itkGetConstReferenceMacro( Opacity, double );

  /** Set/Get the overlay type - PLAIN by default.
   */
  itkSetMacro( Type, int );
  itkGetConstReferenceMacro( Type, int );

  /** Set/Get the object priority - HIGH_LABEL_ON_TOP by default.
   */
  itkSetMacro( Priority, int );
  itkGetConstReferenceMacro( Priority, int );

  /** Set/Get the object dilation radius - 0 by default.
   */
  itkSetMacro( DilationRadius, SizeType );
  itkGetConstReferenceMacro( DilationRadius, SizeType );

  /** Set/Get the contour thickness - 1 by default.
   */
  itkSetMacro( ContourThickness, SizeType );
  itkGetConstReferenceMacro( ContourThickness, SizeType );

  /** Set/Get the slice dimension - defaults to image dimension - 1.
   */
  itkSetMacro( SliceDimension, int );
  itkGetConstReferenceMacro( SliceDimension, int );

protected:
  LabelMapContourOverlayImageFilter();
  ~LabelMapContourOverlayImageFilter() {};

  /** LabelMapContourOverlayImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion();

  /** LabelMapContourOverlayImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));

  virtual void BeforeThreadedGenerateData();

  virtual void ThreadedGenerateData(const OutputImageRegionType& outputRegionForThread, int threadId );

  virtual void ThreadedProcessLabelObject( LabelObjectType * labelObject );
  
  void PrintSelf(std::ostream& os, Indent indent) const;

  virtual InputImageType * GetLabelMap()
    {
    return m_TempImage;
    }

private:
  LabelMapContourOverlayImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  double                    m_Opacity;
  typename Barrier::Pointer m_Barrier;
  int                       m_Type;
  int                       m_Priority;
  SizeType                  m_ContourThickness;
  SizeType                  m_DilationRadius;
  int                       m_SliceDimension;
  
  InputImagePointer         m_TempImage;
  
}; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkLabelMapContourOverlayImageFilter.txx"
#endif

#endif
