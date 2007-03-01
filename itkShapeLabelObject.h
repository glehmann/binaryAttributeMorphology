/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelObject.h,v $
  Language:  C++
  Date:      $Date: 2005/01/21 20:13:31 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkShapeLabelObject_h
#define __itkShapeLabelObject_h

#include "itkLabelObject.h"

namespace itk
{


namespace Functor {

template< class TLabelObject >
class ITK_EXPORT SizeLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef unsigned long AttributeType;

  inline const AttributeType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetSize();
    }
};

template< class TLabelObject >
class ITK_EXPORT PhysicalSizeLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeType;

  inline const AttributeType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetPhysicalSize();
    }
};

template< class TLabelObject >
class ITK_EXPORT RegionElongationLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeType;

  inline const AttributeType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetRegionElongation();
    }
};

template< class TLabelObject >
class ITK_EXPORT SizeRegionRatioLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeType;

  inline const AttributeType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetSizeRegionRatio();
    }
};

}


/** \class ShapeLabelObject
 *  \brief ShapeLabelObject class
 *
 * \ingroup DataRepresentation 
 */
template < class TLabel, unsigned int VImageDimension >
class ITK_EXPORT ShapeLabelObject : public LabelObject< TLabel, VImageDimension >
{
public:
  /** Standard class typedefs */
  typedef ShapeLabelObject         Self;
  typedef LabelObject< TLabel, VImageDimension > Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  typedef WeakPointer<const Self>  ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(ShapeLabelObject, LabelObject);

  typedef LabelCollectionImage< Self > LabelCollectionImageType;

  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  typedef typename Superclass::IndexType IndexType;

  typedef TLabel LabelType;

  typedef typename Superclass::LineType LineType;

  typedef typename Superclass::LengthType LengthType;

  typedef typename Superclass::LineContainerType LineContainerType;

  typedef ImageRegion< ImageDimension > RegionType;

  typedef typename itk::FixedArray<double, ImageDimension> CentroidType;

/*  itkGetConstMacro( Region, RegionType );
  itkSetMacro( Region, RegionType );*/
  const RegionType & GetRegion() const
    {
    return m_Region;
    }

  void SetRegion( const RegionType & region )
    {
    m_Region = region;
    }

//   itkGetConstMacro( PhysicalSize, double );
//   itkSetMacro( PhysicalSize, double );
  const double & GetPhysicalSize() const
    {
    return m_PhysicalSize;
    }

  void SetPhysicalSize( const double & size )
    {
    m_PhysicalSize = size;
    }

//   itkGetConstMacro( Size, unsigned long );
//   itkSetMacro( Size, unsigned long );
  const unsigned long & GetSize() const
    {
    return m_Size;
    }

  void SetSize( const unsigned long & size )
    {
    m_Size = size;
    }

//   itkGetConstMacro( Centroid, CentroidType );
//   itkSetMacro( Centroid, CentroidType );
  const CentroidType & GetCentroid() const
    {
    return m_Centroid;
    }

  void SetCentroid( const CentroidType & centroid )
    {
    m_Centroid = centroid;
    }

//   itkGetConstMacro( RegionElongation, double );
//   itkSetMacro( RegionElongation, double );
  const double & GetRegionElongation() const
    {
    return m_RegionElongation;
    }

  void SetRegionElongation( const double & size )
    {
    m_RegionElongation = size;
    }

//   itkGetConstMacro( SizeRegionRatio, double );
//   itkSetMacro( SizeRegionRatio, double );
  const double & GetSizeRegionRatio() const
    {
    return m_SizeRegionRatio;
    }

  void SetSizeRegionRatio( const double & size )
    {
    m_SizeRegionRatio = size;
    }


  virtual void CopyDataFrom( const Self * src )
    {
    Superclass::CopyDataFrom( src );

    m_Region = src->m_Region;
    m_Size = src->m_Size;
    m_PhysicalSize = src->m_PhysicalSize;
    m_Centroid = src->m_Centroid;
    m_RegionElongation = src->m_RegionElongation;
    m_SizeRegionRatio = src->m_SizeRegionRatio;
    }

protected:
  ShapeLabelObject()
    {
    m_Size = 0;
    m_PhysicalSize = 0;
    m_RegionElongation = 0;
    m_SizeRegionRatio = 0;
    }
  

  void PrintSelf(std::ostream& os, Indent indent) const
    {
    Superclass::PrintSelf( os, indent );

    os << indent << "Centroid: " << m_Centroid << std::endl;
    os << indent << "Region: ";
    m_Region.Print( os, indent );
    os << indent << "PhysicalSize: " << m_PhysicalSize << std::endl;
    os << indent << "Size: " << m_Size << std::endl;
    os << indent << "RegionElongation: " << m_RegionElongation << std::endl;
    os << indent << "SizeRegionRatio: " << m_SizeRegionRatio << std::endl;
    }

private:
  ShapeLabelObject(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  RegionType m_Region;
  unsigned long m_Size;
  double m_PhysicalSize;
  CentroidType m_Centroid;
  double m_RegionElongation;
  double m_SizeRegionRatio;
  

};

} // end namespace itk

#endif
