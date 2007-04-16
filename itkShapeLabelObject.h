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
  typedef unsigned long AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetSize();
    }
};

template< class TLabelObject >
class ITK_EXPORT PhysicalSizeLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetPhysicalSize();
    }
};

template< class TLabelObject >
class ITK_EXPORT RegionElongationLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetRegionElongation();
    }
};

template< class TLabelObject >
class ITK_EXPORT SizeRegionRatioLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetSizeRegionRatio();
    }
};

template< class TLabelObject >
class ITK_EXPORT SizeOnBorderLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef unsigned long AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetSizeOnBorder();
    }
};

template< class TLabelObject >
class ITK_EXPORT CentroidLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef typename LabelObjectType::CentroidType AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetCentroid();
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

  typedef unsigned int AttributeType;
  static const AttributeType SIZE=0;
  static const AttributeType PHYSICAL_SIZE=1;
  static const AttributeType REGION_ELONGATION=2;
  static const AttributeType SIZE_REGION_RATIO=3;
  static const AttributeType CENTROID=4;
  static const AttributeType REGION=5;
  static const AttributeType SIZE_ON_BORDER=6;

  static AttributeType GetAttributeFromName( const std::string & s )
    {
    if( s == "Size" )
      {
      return SIZE;
      }
    else if( s == "PhysicalSize" )
      {
      return PHYSICAL_SIZE;
      }
    else if( s == "RegionElongation" )
      {
      return REGION_ELONGATION;
      }
    else if( s == "SizeRegionRatio" )
      {
      return SIZE_REGION_RATIO;
      }
    else if( s == "Centroid" )
      {
      return CENTROID;
      }
    else if( s == "Region" )
      {
      return REGION;
      }
    else if( s == "SizeOnBorder" )
      {
      return SIZE_ON_BORDER;
      }
    // can't recognize the namespace
    throw std::runtime_error("Unknown attribute.");
    }

  static std::string GetNameFromAttribute( const AttributeType & a )
    {
    switch( a )
      {
      case SIZE:
        return "Size";
        break;
      case PHYSICAL_SIZE:
        return "PhysicalSize";
        break;
      case REGION_ELONGATION:
        return "RegionElongation";
        break;
      case SIZE_REGION_RATIO:
        return "SizeRegionRatio";
        break;
      case CENTROID:
        return "Centroid";
      case REGION:
        return "Region";
        break;
      case SIZE_ON_BORDER:
        return "SizeOnBorder";
        break;
      }
      // can't recognize the namespace
      throw std::runtime_error("Unknown attribute.");
    }

  typedef ImageRegion< ImageDimension > RegionType;

  typedef typename itk::Point<double, ImageDimension> CentroidType;

/*  itkGetConstMacro( Region, RegionType );
  itkSetMacro( Region, RegionType );*/
  const RegionType & GetRegion() const
    {
    return m_Region;
    }

  void SetRegion( const RegionType & v )
    {
    m_Region = v;
    }

//   itkGetConstMacro( PhysicalSize, double );
//   itkSetMacro( PhysicalSize, double );
  const double & GetPhysicalSize() const
    {
    return m_PhysicalSize;
    }

  void SetPhysicalSize( const double & v )
    {
    m_PhysicalSize = v;
    }

//   itkGetConstMacro( Size, unsigned long );
//   itkSetMacro( Size, unsigned long );
  const unsigned long & GetSize() const
    {
    return m_Size;
    }

  void SetSize( const unsigned long & v )
    {
    m_Size = v;
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

  void SetRegionElongation( const double & v )
    {
    m_RegionElongation = v;
    }

//   itkGetConstMacro( SizeRegionRatio, double );
//   itkSetMacro( SizeRegionRatio, double );
  const double & GetSizeRegionRatio() const
    {
    return m_SizeRegionRatio;
    }

  void SetSizeRegionRatio( const double & v )
    {
    m_SizeRegionRatio = v;
    }

//   itkGetConstMacro( SizeOnBorder, bool );
//   itkSetMacro( SizeOnBorder, bool );
  const unsigned long & GetSizeOnBorder() const
    {
    return m_SizeOnBorder;
    }

  void SetSizeOnBorder( const unsigned long & v )
    {
    m_SizeOnBorder = v;
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
    m_SizeOnBorder = src->m_SizeOnBorder;
    }

protected:
  ShapeLabelObject()
    {
    m_Size = 0;
    m_PhysicalSize = 0;
    m_RegionElongation = 0;
    m_SizeRegionRatio = 0;
    m_SizeOnBorder = false;
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
    os << indent << "SizeOnBorder: " << m_SizeOnBorder << std::endl;
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
  unsigned long m_SizeOnBorder;
  

};

} // end namespace itk

#endif
