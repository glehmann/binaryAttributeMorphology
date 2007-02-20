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

  itkGetConstMacro( Region, RegionType );
  itkSetMacro( Region, RegionType );

  itkGetConstMacro( PhysicalSize, double );
  itkSetMacro( PhysicalSize, double );

  itkGetConstMacro( Size, unsigned long );
  itkSetMacro( Size, unsigned long );

  itkGetConstMacro( Centroid, CentroidType );
  itkSetMacro( Centroid, CentroidType );

  virtual void CopyDataFrom( const Self * src )
    {
    Superclass::CopyDataFrom( src );

    m_Region = src->m_Region;
    m_Size = src->m_Size;
    m_PhysicalSize = src->m_PhysicalSize;
    m_Centroid = src->m_Centroid;
    }

protected:
  ShapeLabelObject()
    {
    m_Size = 0;
    m_PhysicalSize = 0;
    }
  

  void PrintSelf(std::ostream& os, Indent indent) const
    {
    Superclass::PrintSelf( os, indent );
    os << indent << "Centroid: " << m_Centroid << std::endl;
    os << indent << "Region: ";
    m_Region.Print( os, indent );
    os << indent << "PhysicalSize: " << m_PhysicalSize << std::endl;
    os << indent << "Size: " << m_Size << std::endl;
    }

private:
  ShapeLabelObject(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  RegionType m_Region;
  unsigned long m_Size;
  double m_PhysicalSize;
  CentroidType m_Centroid;
  

};

} // end namespace itk

#endif
