/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelCollectionImage.h,v $
  Language:  C++
  Date:      $Date: 2006/04/20 14:54:09 $
  Version:   $Revision: 1.136 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkLabelCollectionImage_h
#define __itkLabelCollectionImage_h

#include "itkImageBase.h"
#include "itkImageRegion.h"
#include "itkFixedArray.h"
#include "itkWeakPointer.h"
#include <map>

namespace itk
{
/** \class LabelCollectionImage
 *  \brief Templated n-dimensional image class.
 *
 * \ingroup ImageObjects */
template <class TLabelObject >
class ITK_EXPORT LabelCollectionImage : public ImageBase<TLabelObject::ImageDimension>
{
public:
  /** Standard class typedefs */
  typedef LabelCollectionImage               Self;
  typedef ImageBase<TLabelObject::ImageDimension>  Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  typedef WeakPointer<const Self>  ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(LabelCollectionImage, ImageBase);

  typedef TLabelObject LabelObjectType;

  typedef typename LabelObjectType::Popinter LabelObjectPointerType;

  /** Dimension of the image.  This constant is used by functions that are
   * templated over image type (as opposed to being templated over pixel type
   * and dimension) when they need compile time access to the dimension of
   * the image. */
  itkStaticConstMacro(ImageDimension, unsigned int, LabelObjectType::ImageDimension);

  /** Label typedef support. */
  typedef typename LabelObjectType::LabelType  LabelType;
  typedef LabelType PixelType;

  /** the LabelObject container type */
  typedef std::map< LabelType, LabelObjectPointerType > LabelObjectContainerType;

  /** Index typedef support. An index is used to access pixel values. */
  typedef typename Superclass::IndexType  IndexType;

  /** Offset typedef support. An offset is used to access pixel values. */
  typedef typename Superclass::OffsetType OffsetType;

  /** Size typedef support. A size is used to define region bounds. */
  typedef typename Superclass::SizeType  SizeType;

  /** Direction typedef support. A matrix of direction cosines. */
  typedef typename Superclass::DirectionType  DirectionType;

  /** Region typedef support. A region is used to specify a subset of an image. */
  typedef typename Superclass::RegionType  RegionType;

  /** Spacing typedef support.  Spacing holds the size of a pixel.  The
   * spacing is the geometric distance between image samples. */
  typedef typename Superclass::SpacingType SpacingType;

  /** Origin typedef support.  The origin is the geometric coordinates
   * of the index (0,0). */
  typedef typename Superclass::PointType PointType;

  /** Offset typedef (relative position between indices) */
  typedef typename Superclass::OffsetValueType OffsetValueType;

  /** Convenience methods to set the LargestPossibleRegion,
   *  BufferedRegion and RequestedRegion. Allocate must still be called.
   */
  void SetRegions(RegionType region)
    {
    this->SetLargestPossibleRegion(region);
    this->SetBufferedRegion(region);
    this->SetRequestedRegion(region);
    };

  void SetRegions(SizeType size)
    {
    RegionType region; region.SetSize(size);
    this->SetLargestPossibleRegion(region);
    this->SetBufferedRegion(region);
    this->SetRequestedRegion(region);
    };

  /** Restore the data object to its initial state. This means releasing
   * memory. */
  virtual void Initialize();

  virtual void Graft(const DataObject *data);

  LabelObjectType * GetLabelObject( const LabelType & label )
    {
    return m_LabelObjectContainer[label].GetPointer();
    }

  const LabelObjectType * GetLabelObject( const LabelType & label ) const
    {
    return m_LabelObjectContainer[label].GetPointer();
    }
    
  bool HasLabel( const LabelType label ) const;
  
  const LabelType & GetPixel( const IndexType & idx ) const;
  
  void SetPixel( const IndexType & idx, const LabelType & label );

  itkGetMacro(BackgroundLabel, LabelType);
  itkSetMacro(BackgroundLabel, LabelType);  
  
  itkGetMacro(UseBackground, bool);
  itkSetMacro(UseBackground, bool);  
  itkBooleanMacro(UseBackground);

protected:
  LabelCollectionImage();
  virtual ~LabelCollectionImage() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  LabelCollectionImage(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  LabelObjectContainerType m_LabelObjectContainer;
  LabelType m_BackgroundLabel;
  bool m_UseBackground;
};

} // end namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
# include "itkLabelCollectionImage.txx"
#endif

#endif
