/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkObjectImageLabelMapFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkObjectImageLabelMapFilter_h
#define __itkObjectImageLabelMapFilter_h

#include "itkLabelMapFilter.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkLabelMapToBinaryImageFilter.h"
#include "itkLabelSelectionLabelMapFilter.h"
#include "itkAutoCropLabelMapFilter.h"
#include "itkPadLabelMapFilter.h"


namespace itk {

/** \class ObjectImageLabelMapFilter
 * \brief TODO
 *
 *
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa WatershedImageFilter, MorphologicalWatershedFromMarkersImageFilter, RelabelComponentImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TOutputImage=TInputImage,
  class TInputFilter=ImageToImageFilter< 
    Image< unsigned char, ::itk::GetImageDimension<TInputImage >::ImageDimension >,
    Image< unsigned char, ::itk::GetImageDimension<TOutputImage>::ImageDimension > >,
  class TOutputFilter=TInputFilter,
  class TInternalInputImage=ITK_TYPENAME TInputFilter::InputImageType,
  class TInternalOutputImage=ITK_TYPENAME TOutputFilter::OutputImageType >
class ITK_EXPORT ObjectImageLabelMapFilter : 
    public LabelMapFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef ObjectImageLabelMapFilter Self;
  typedef LabelMapFilter<TInputImage, TOutputImage>
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
  typedef typename OutputImageType::SizeType       SizeType;
  typedef OutputImageType                          LabelMapType;
  typedef typename LabelMapType::LabelObjectType   LabelObjectType;
  
  typedef TInputFilter                          InputFilterType;
  typedef TOutputFilter                         OutputFilterType;
  
  typedef TInternalInputImage                         InternalInputImageType;
  typedef typename InternalInputImageType::RegionType InternalRegionType;
  typedef typename InternalInputImageType::SizeType   InternalSizeType;
  typedef typename InternalInputImageType::IndexType  InternalIndexType;
  typedef typename InternalInputImageType::OffsetType InternalOffsetType;
  typedef typename InternalInputImageType::PixelType  InternalInputPixelType;

  typedef TInternalOutputImage                        InternalOutputImageType;
  typedef typename InternalOutputImageType::PixelType InternalOutputPixelType;

  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  // filter types used internally
  typedef itk::LabelSelectionLabelMapFilter< LabelMapType > SelectType;
  typedef itk::AutoCropLabelMapFilter< LabelMapType > CropType;
  typedef itk::PadLabelMapFilter< LabelMapType > PadType;
  typedef itk::LabelMapToBinaryImageFilter< LabelMapType, InternalInputImageType> LM2BIType;
  typedef itk::LabelImageToLabelMapFilter< InternalOutputImageType, LabelMapType> LI2LMType;
  typedef itk::BinaryImageToLabelMapFilter< InternalOutputImageType, LabelMapType> BI2LMType;

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(ObjectImageLabelMapFilter, 
               LabelMapFilter);

  void SetFilter(InputFilterType * filter);
  InputFilterType * GetFilter()
    {
    return this->m_InputFilter;
    }

  const InputFilterType * GetFilter() const
    {
    return this->m_InputFilter;
    }

  void SetInputFilter( InputFilterType * filter );
  itkGetObjectMacro( InputFilter, InputFilterType );

  void SetOutputFilter( OutputFilterType * filter );
  itkGetObjectMacro( OutputFilter, OutputFilterType );


  itkSetMacro(ConstrainPaddingToImage, bool);
  itkGetMacro(ConstrainPaddingToImage, bool);
  itkBooleanMacro(ConstrainPaddingToImage);

  itkSetMacro(BinaryInternalOutput, bool);
  itkGetMacro(BinaryInternalOutput, bool);
  itkBooleanMacro(BinaryInternalOutput);

  itkSetMacro(KeepLabels, bool);
  itkGetMacro(KeepLabels, bool);
  itkBooleanMacro(KeepLabels);

  itkSetMacro(PadSize, SizeType);
  itkGetMacro(PadSize, SizeType);

protected:
  ObjectImageLabelMapFilter();
  ~ObjectImageLabelMapFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  virtual void GenerateData();
  

private:
  ObjectImageLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool     m_ConstrainPaddingToImage;
  SizeType m_PadSize;
  bool     m_BinaryInternalOutput;

  // we'll do our best to reuse the label of the input object in the output one. However, this is
  // possible only if the internal pipeline produce a single 
  bool     m_KeepLabels;
  
  InternalOutputPixelType m_InternalForegroundValue;
  

  typename InputFilterType::Pointer       m_InputFilter;
  typename OutputFilterType::Pointer      m_OutputFilter;

  typename SelectType::Pointer m_Select;
  typename CropType::Pointer m_Crop;
  typename PadType::Pointer m_Pad;
  typename LM2BIType::Pointer m_LM2BI;
  typename LI2LMType::Pointer m_LI2LM;
  typename BI2LMType::Pointer m_BI2LM;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkObjectImageLabelMapFilter.txx"
#endif

#endif


