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

template< class TLabelObject >
class ITK_EXPORT FeretDiameterLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetFeretDiameter();
    }
};

template< class TLabelObject >
class ITK_EXPORT BinaryPrincipalMomentsLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef typename LabelObjectType::VectorType AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetBinaryPrincipalMoments();
    }
};

template< class TLabelObject >
class ITK_EXPORT BinaryPrincipalAxesLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef typename LabelObjectType::MatrixType AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetBinaryPrincipalAxes();
    }
};

template< class TLabelObject >
class ITK_EXPORT BinaryElongationLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetBinaryElongation();
    }
};

template< class TLabelObject >
class ITK_EXPORT PerimeterLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetPerimeter();
    }
};

template< class TLabelObject >
class ITK_EXPORT RoundnessLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetRoundness();
    }
};

}


/** \class ShapeLabelObject
 *  \brief A Label object to store the common attributes related to the shape of the object
 *
 * ShapeLabelObject stores  the common attributes related to the shape of the object
 *
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA de Jouy-en-Josas, France.
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
  static const AttributeType FERET_DIAMETER=7;
  static const AttributeType BINARY_PRINCIPAL_MOMENTS=8;
  static const AttributeType BINARY_PRINCIPAL_AXES=9;
  static const AttributeType BINARY_ELONGATION=10;
  static const AttributeType PERIMETER=11;
  static const AttributeType ROUNDNESS=12;

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
    else if( s == "FeretDiameter" )
      {
      return FERET_DIAMETER;
      }
    else if( s == "BinaryPrincipalMoments" )
      {
      return BINARY_PRINCIPAL_MOMENTS;
      }
    else if( s == "BinaryPrincipalAxes" )
      {
      return BINARY_PRINCIPAL_AXES;
      }
    else if( s == "BinaryElongation" )
      {
      return BINARY_ELONGATION;
      }
    else if( s == "Perimeter" )
      {
      return PERIMETER;
      }
    else if( s == "Roundness" )
      {
      return ROUNDNESS;
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
      case FERET_DIAMETER:
        return "FeretDiameter";
        break;
      case BINARY_PRINCIPAL_MOMENTS:
        return "BinaryPrincipalMoments";
        break;
      case BINARY_PRINCIPAL_AXES:
        return "BinaryPrincipalAxes";
        break;
      case BINARY_ELONGATION:
        return "BinaryElongation";
        break;
      case PERIMETER:
        return "Perimeter";
        break;
      case ROUNDNESS:
        return "Roundness";
        break;
      }
      // can't recognize the namespace
      throw std::runtime_error("Unknown attribute.");
    }

  typedef ImageRegion< ImageDimension > RegionType;

  typedef typename itk::Point<double, ImageDimension> CentroidType;

  typedef Matrix< double, ImageDimension, ImageDimension >   MatrixType;

  typedef Vector< double, ImageDimension > VectorType;

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

//   itkGetConstMacro( FeretDiameter, double );
//   itkSetMacro( FeretDiameter, double );
  const double & GetFeretDiameter() const
    {
    return m_FeretDiameter;
    }

  void SetFeretDiameter( const double & v )
    {
    m_FeretDiameter = v;
    }

//   itkGetConstMacro( BinaryPrincipalMoments, VectorType );
//   itkSetMacro( BinaryPrincipalMoments, VectorType );
  const VectorType & GetBinaryPrincipalMoments() const
    {
    return m_BinaryPrincipalMoments;
    }

  void SetBinaryPrincipalMoments( const VectorType & v )
    {
    m_BinaryPrincipalMoments = v;
    }

//   itkGetConstMacro( BinaryPrincipalAxes, MatrixType );
//   itkSetMacro( BinaryPrincipalAxes, MatrixType );
  const MatrixType & GetBinaryPrincipalAxes() const
    {
    return m_BinaryPrincipalAxes;
    }

  void SetBinaryPrincipalAxes( const MatrixType & v )
    {
    m_BinaryPrincipalAxes = v;
    }

//   itkGetConstMacro( BinaryElongation, double );
//   itkSetMacro( BinaryElongation, double );
  const double & GetBinaryElongation() const
    {
    return m_BinaryElongation;
    }

  void SetBinaryElongation( const double & v )
    {
    m_BinaryElongation = v;
    }

//   itkGetConstMacro( Perimeter, double );
//   itkSetMacro( Perimeter, double );
  const double & GetPerimeter() const
    {
    return m_Perimeter;
    }

  void SetPerimeter( const double & v )
    {
    m_Perimeter = v;
    }

//   itkGetConstMacro( Roundness, double );
//   itkSetMacro( Roundness, double );
  const double & GetRoundness() const
    {
    return m_Roundness;
    }

  void SetRoundness( const double & v )
    {
    m_Roundness = v;
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
    m_FeretDiameter = src->m_FeretDiameter;
    m_BinaryPrincipalMoments = src->m_BinaryPrincipalMoments;
    m_BinaryPrincipalAxes = src->m_BinaryPrincipalAxes;
    m_BinaryElongation = src->m_BinaryElongation;
    m_Perimeter = src->m_Perimeter;
    m_Roundness = src->m_Roundness;
    }

protected:
  ShapeLabelObject()
    {
    m_Size = 0;
    m_PhysicalSize = 0;
    m_RegionElongation = 0;
    m_SizeRegionRatio = 0;
    m_SizeOnBorder = false;
    m_FeretDiameter = false;
    m_BinaryPrincipalMoments.Fill(0);
    m_BinaryPrincipalAxes.Fill(0);
    m_BinaryElongation = 0;
    m_Perimeter = 0;
    m_Roundness = 0;
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
    os << indent << "FeretDiameter: " << m_FeretDiameter << std::endl;
    os << indent << "BinaryPrincipalMoments: " << m_BinaryPrincipalMoments << std::endl;
    os << indent << "BinaryPrincipalAxes: " << m_BinaryPrincipalAxes << std::endl;
    os << indent << "BinaryElongation: " << m_BinaryElongation << std::endl;
    os << indent << "Perimeter: " << m_Perimeter << std::endl;
    os << indent << "Roundness: " << m_Roundness << std::endl;
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
  double m_FeretDiameter;
  VectorType m_BinaryPrincipalMoments;
  MatrixType m_BinaryPrincipalAxes;
  double m_BinaryElongation;
  double m_Perimeter;
  double m_Roundness;
  

};

} // end namespace itk

#endif
