/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkReconstructionLabelCollectionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkReconstructionLabelCollectionImageFilter_h
#define __itkReconstructionLabelCollectionImageFilter_h

#include "itkInPlaceLabelCollectionImageFilter.h"

namespace itk {
/** \class ReconstructionLabelCollectionImageFilter
 * \brief 
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage, class TMaskImage>
class ITK_EXPORT ReconstructionLabelCollectionImageFilter : 
    public InPlaceLabelCollectionImageFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef ReconstructionLabelCollectionImageFilter Self;
  typedef InPlaceLabelCollectionImageFilter<TImage>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TImage ImageType;
  typedef typename ImageType::Pointer         ImagePointer;
  typedef typename ImageType::ConstPointer    ImageConstPointer;
  typedef typename ImageType::PixelType       PixelType;
  typedef typename ImageType::IndexType       IndexType;
  
  typedef TMaskImage MaskImageType;
  typedef typename MaskImageType::Pointer         MaskImagePointer;
  typedef typename MaskImageType::ConstPointer    MaskImageConstPointer;
  typedef typename MaskImageType::PixelType       MaskImagePixelType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(ReconstructionLabelCollectionImageFilter, 
               InPlaceLabelCollectionImageFilter);

#ifdef ITK_USE_CONCEPT_CHECKING
  /** Begin concept checking */
/*  itkConceptMacro(InputEqualityComparableCheck,
    (Concept::EqualityComparable<InputImagePixelType>));
  itkConceptMacro(IntConvertibleToInputCheck,
    (Concept::Convertible<int, InputImagePixelType>));
  itkConceptMacro(InputOStreamWritableCheck,
    (Concept::OStreamWritable<InputImagePixelType>));*/
  /** End concept checking */
#endif

   /** Set the marker image */
  void SetMaskImage(TMaskImage *input)
     {
     // Process object is not const-correct so the const casting is required.
     this->SetNthInput( 1, const_cast<TMaskImage *>(input) );
     }

  /** Get the marker image */
  MaskImageType * GetMaskImage()
    {
    return static_cast<MaskImageType*>(const_cast<DataObject *>(this->ProcessObject::GetInput(1)));
    }

   /** Set the input image */
  void SetInput1(TImage *input)
     {
     this->SetInput( input );
     }

   /** Set the marker image */
  void SetInput2(TMaskImage *input)
     {
     this->SetMaskImage( input );
     }

  /**
   * Set/Get the value used as "foreground" in the output image.
   * Defaults to NumericTraits<MaskPixelType>::max().
   */
  itkSetMacro(ForegroundValue, InputImagePixelType);
  itkGetConstMacro(ForegroundValue, InputImagePixelType);

protected:
  ReconstructionLabelCollectionImageFilter();
  ~ReconstructionLabelCollectionImageFilter() {};

  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  ReconstructionLabelCollectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  
  MaskImagePixelType m_ForegroundValue;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkReconstructionLabelCollectionImageFilter.txx"
#endif

#endif


