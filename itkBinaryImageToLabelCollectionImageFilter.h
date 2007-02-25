/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryImageToLabelCollectionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2007/02/18 20:03:03 $
  Version:   $Revision: 1.16 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkBinaryImageToLabelCollectionImageFilter_h
#define __itkBinaryImageToLabelCollectionImageFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkConceptChecking.h"
#include <vector>
#include <map>
#include "itkProgressReporter.h"

namespace itk
{

/**
 * \class BinaryImageToLabelCollectionImageFilter
 * \brief Label the objects in a binary image
 *
 * BinaryImageToLabelCollectionImageFilter labels the objects in a binary image.
 * Each distinct object is assigned a unique label. The filter experiments
 * with some improvements to the existing implementation, and is based on
 * run length encoding along raster lines.
 * The final object labels start with 1 and are consecutive. Objects
 * that are reached earlier by a raster order scan have a lower
 * label. This is different to the behaviour of the original connected
 * component image filter which did not produce consecutive labels or
 * impose any particular ordering.
 *
 *
 * \sa ImageToImageFilter
 */

template <class TInputImage, class TOutputImage>
class ITK_EXPORT BinaryImageToLabelCollectionImageFilter : 
    public ImageToImageFilter< TInputImage, TOutputImage > 
{
public:
  /**
   * Standard "Self" & Superclass typedef.
   */
  typedef BinaryImageToLabelCollectionImageFilter                   Self;
  typedef ImageToImageFilter< TInputImage, TOutputImage > Superclass;

  /**
   * Types from the Superclass
   */
  typedef typename Superclass::InputImagePointer InputImagePointer;

  /**
   * Extract some information from the image types.  Dimensionality
   * of the two images is assumed to be the same.
   */
  typedef typename TOutputImage::PixelType         OutputPixelType;
  typedef typename TInputImage::PixelType          InputPixelType;
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  
  /**
   * Image typedef support
   */
  typedef TInputImage                       InputImageType;
  typedef typename TInputImage::IndexType   IndexType;
  typedef typename TInputImage::SizeType    SizeType;
  typedef typename TInputImage::OffsetType  OffsetType;

  typedef TOutputImage                      OutputImageType;
  typedef typename TOutputImage::RegionType RegionType;
  typedef typename TOutputImage::IndexType  OutputIndexType;
  typedef typename TOutputImage::SizeType   OutputSizeType;
  typedef typename TOutputImage::OffsetType OutputOffsetType;

  typedef std::list<IndexType>              ListType;

  /** 
   * Smart pointer typedef support 
   */
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /**
   * Run-time type information (and related methods)
   */
  itkTypeMacro(BinaryImageToLabelCollectionImageFilter, ImageToImageFilter);
  
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

  /**
   * Set/Get whether the connected components are defined strictly by
   * face connectivity or by face+edge+vertex connectivity.  Default is
   * FullyConnectedOff.  For objects that are 1 pixel wide, use
   * FullyConnectedOn.
   */
  itkSetMacro(FullyConnected, bool);
  itkGetConstReferenceMacro(FullyConnected, bool);
  itkBooleanMacro(FullyConnected);
  
  // only set after completion
  itkGetConstReferenceMacro(ObjectCount, unsigned long);

  // Concept checking -- input and output dimensions must be the same
  itkConceptMacro(SameDimension,
    (Concept::SameDimension<itkGetStaticConstMacro(InputImageDimension),
       itkGetStaticConstMacro(OutputImageDimension)>));


  /**
   * Set/Get the value used as "background" in the output image.
   * Defaults to NumericTraits<PixelType>::NonpositiveMin().
   */
  itkSetMacro(BackgroundValue, OutputPixelType);
  itkGetConstMacro(BackgroundValue, OutputPixelType);

  /**
   * Set/Get the value used as "foreground" in the output image.
   * Defaults to NumericTraits<PixelType>::max().
   */
  itkSetMacro(ForegroundValue, InputPixelType);
  itkGetConstMacro(ForegroundValue, InputPixelType);

  /**
   * Set/Get wether a flat image must be considered as a maxima or not.
   * Defaults to true.
   */
  itkSetMacro(UseBackground, bool);
  itkGetConstMacro(UseBackground, bool);
  itkBooleanMacro(UseBackground);

protected:
  BinaryImageToLabelCollectionImageFilter() 
    {
    m_FullyConnected = false;
    m_ObjectCount = 0;
    m_UseBackground = true;
    m_BackgroundValue = NumericTraits<OutputPixelType>::NonpositiveMin();
    m_ForegroundValue = NumericTraits<InputPixelType>::max();
    }
  virtual ~BinaryImageToLabelCollectionImageFilter() {}
  BinaryImageToLabelCollectionImageFilter(const Self&) {}
  void PrintSelf(std::ostream& os, Indent indent) const;

  /**
   * Standard pipeline method. 
   */
  void GenerateData();

  /** BinaryImageToLabelCollectionImageFilter needs the entire input. Therefore
   * it must provide an implementation GenerateInputRequestedRegion().
   * \sa ProcessObject::GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion();

  /** BinaryImageToLabelCollectionImageFilter will produce all of the output.
   * Therefore it must provide an implementation of
   * EnlargeOutputRequestedRegion().
   * \sa ProcessObject::EnlargeOutputRequestedRegion() */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));

  bool m_FullyConnected;
  
private:
  bool m_UseBackground;
  OutputPixelType m_BackgroundValue;
  InputPixelType m_ForegroundValue;

  unsigned long m_ObjectCount;
  // some additional types
  typedef typename TOutputImage::RegionType::SizeType OutSizeType;

  // types to support the run length encoding of lines
  class runLength
    {
    public:
    // run length information - may be a more type safe way of doing this
    long int length;
    typename InputImageType::IndexType where; // Index of the start of the run
    unsigned long int label; // the initial label of the run
    };

  typedef std::vector<runLength> lineEncoding;

  // the map storing lines
  typedef std::map<long, lineEncoding> LineMapType;
  
  typedef std::vector<long> OffsetVec;

  // the types to support union-find operations
  typedef std::vector<unsigned long int> UnionFindType;
  UnionFindType m_UnionFind;
  UnionFindType m_Consecutive;
  // functions to support union-find operations
  void InitUnion(const unsigned long int size) 
    {
    m_UnionFind = UnionFindType(size + 1);
    }
  void InsertSet(const unsigned long int label);
  unsigned long int LookupSet(const unsigned long int label);
  void LinkLabels(const unsigned long int lab1, const unsigned long int lab2);
  unsigned long int CreateConsecutive();
  //////////////////
  bool CheckNeighbors(const OutputIndexType &A, 
                      const OutputIndexType &B);

  void CompareLines(lineEncoding &current, const lineEncoding &Neighbour);

  void FillOutput(const LineMapType &LineMap,
                  ProgressReporter &progress);

  void SetupLineOffsets(OffsetVec &LineOffsets);
};
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryImageToLabelCollectionImageFilter.txx"
#endif

#endif
