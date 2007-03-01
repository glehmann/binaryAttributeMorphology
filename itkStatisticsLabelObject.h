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
#ifndef __itkStatisticsLabelObject_h
#define __itkStatisticsLabelObject_h

#include "itkShapeLabelObject.h"

namespace itk
{


namespace Functor {


}


/** \class StatisticsLabelObject
 *  \brief StatisticsLabelObject class
 *
 * \ingroup DataRepresentation 
 */
template < class TLabel, unsigned int VImageDimension >
class ITK_EXPORT StatisticsLabelObject : public ShapeLabelObject< TLabel, VImageDimension >
{
public:
  /** Standard class typedefs */
  typedef StatisticsLabelObject         Self;
  typedef ShapeLabelObject< TLabel, VImageDimension > Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  typedef WeakPointer<const Self>  ConstWeakPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(StatisticsLabelObject, LabelObject);

  typedef LabelCollectionImage< Self > LabelCollectionImageType;

  itkStaticConstMacro(ImageDimension, unsigned int, VImageDimension);

  typedef typename Superclass::IndexType IndexType;

  typedef TLabel LabelType;

  typedef typename Superclass::LineType LineType;

  typedef typename Superclass::LengthType LengthType;

  typedef typename Superclass::LineContainerType LineContainerType;

  typedef ImageRegion< ImageDimension > RegionType;

  typedef typename itk::FixedArray<double, ImageDimension> CentroidType;


  virtual void CopyDataFrom( const Self * src )
    {
    Superclass::CopyDataFrom( src );

    }

protected:
  StatisticsLabelObject()
    {
    }
  

  void PrintSelf(std::ostream& os, Indent indent) const
    {
    Superclass::PrintSelf( os, indent );
    }

private:
  StatisticsLabelObject(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

};

} // end namespace itk

#endif
