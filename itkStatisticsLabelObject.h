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

template< class TLabelObject >
class ITK_EXPORT MinimumLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetMinimum();
    }
};

template< class TLabelObject >
class ITK_EXPORT MaximumLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetMaximum();
    }
};

template< class TLabelObject >
class ITK_EXPORT MeanLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetMean();
    }
};

template< class TLabelObject >
class ITK_EXPORT SumLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetSum();
    }
};

template< class TLabelObject >
class ITK_EXPORT SigmaLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetSigma();
    }
};

template< class TLabelObject >
class ITK_EXPORT VarianceLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetVariance();
    }
};

template< class TLabelObject >
class ITK_EXPORT MedianLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef double AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetMedian();
    }
};

template< class TLabelObject >
class ITK_EXPORT MaximumIndexLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef typename LabelObjectType::IndexType AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetMaximumIndex();
    }
};

template< class TLabelObject >
class ITK_EXPORT MinimumIndexLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef typename LabelObjectType::IndexType AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetMinimumIndex();
    }
};

template< class TLabelObject >
class ITK_EXPORT CenterOfGravityLabelObjectAccessor
{
public:
  typedef TLabelObject LabelObjectType;
  typedef typename LabelObjectType::PointType AttributeValueType;

  inline const AttributeValueType operator()( const LabelObjectType * labelObject )
    {
    return labelObject->GetCenterOfGravity();
    }
};


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

  typedef Point< double, ImageDimension > PointType;

  typedef TLabel LabelType;

  typedef typename Superclass::LineType LineType;

  typedef typename Superclass::LengthType LengthType;

  typedef typename Superclass::LineContainerType LineContainerType;

  typedef typename Superclass::AttributeType AttributeType;
  static const AttributeType MINIMUM=100;
  static const AttributeType MAXIMUM=101;
  static const AttributeType MEAN=102;
  static const AttributeType SUM=103;
  static const AttributeType SIGMA=104;
  static const AttributeType VARIANCE=105;
  static const AttributeType MEDIAN=106;
  static const AttributeType MAXIMUM_INDEX=107;
  static const AttributeType MINIMUM_INDEX=108;
  static const AttributeType CENTER_OF_GRAVITY=109;

  static AttributeType GetAttributeFromName( const std::string & s )
    {
    if( s == "Minimum" )
      {
      return MINIMUM;
      }
    else if( s == "Maximum" )
      {
      return MAXIMUM;
      }
    else if( s == "Mean" )
      {
      return MEAN;
      }
    else if( s == "Sum" )
      {
      return SUM;
       }
    else if( s == "Sigma" )
      {
      return SIGMA;
      }
    else if( s == "Variance" )
      {
      return VARIANCE;
      }
    else if( s == "Median" )
      {
      return MEDIAN;
      }
    else if( s == "MaximumIndex" )
      {
      return MAXIMUM_INDEX;
      }
    else if( s == "MinimumIndex" )
      {
      return MINIMUM_INDEX;
      }
    else if( s == "CenterOfGravity" )
      {
      return CENTER_OF_GRAVITY;
      }
    // can't recognize the namespace
    return Superclass::GetAttributeFromName( s );
    }

  static std::string GetNameFromAttribute( const AttributeType & a )
    {
    switch( a )
      {
      case MINIMUM:
        return "Minimum";
        break;
      case MAXIMUM:
        return "Maximum";
        break;
      case MEAN:
        return "Mean";
        break;
      case SUM:
        return "Sum";
        break;
      case SIGMA:
        return "Sigma";
        break;
      case VARIANCE:
        return "Variance";
        break;
      case MEDIAN:
        return "Median";
        break;
      case MAXIMUM_INDEX:
        return "MaximumIndex";
        break;
      case MINIMUM_INDEX:
        return "MinimumIndex";
        break;
      case CENTER_OF_GRAVITY:
        return "CenterOfGravity";
        break;
      }
      // can't recognize the namespace
      return Superclass::GetNameFromAttribute( a );
    }

  typedef ImageRegion< ImageDimension > RegionType;

  typedef typename Superclass::CentroidType CentroidType;


  virtual void CopyDataFrom( const Self * src )
    {
    Superclass::CopyDataFrom( src );

    m_Minimum = src->m_Minimum;
    m_Maximum = src->m_Maximum;
    m_Mean = src->m_Mean;
    m_Sum = src->m_Sum;
    m_Sigma = src->m_Sigma;
    m_Variance = src->m_Variance;
    m_Median = src->m_Median;
    m_MaximumIndex = src->m_MaximumIndex;
    m_MinimumIndex = src->m_MinimumIndex;
    m_CenterOfGravity = src->m_CenterOfGravity;
    }

//   itkGetConstMacro( Minimum, double );
//   itkSetMacro( Minimum, double );
  const double & GetMinimum() const
    {
    return m_Minimum;
    }

  void SetMinimum( const double & v )
    {
    m_Minimum = v;
    }

//   itkGetConstMacro( Maximum, double );
//   itkSetMacro( Maximum, double );
  const double & GetMaximum() const
    {
    return m_Maximum;
    }

  void SetMaximum( const double & v )
    {
    m_Maximum = v;
    }

//   itkGetConstMacro( Mean, double );
//   itkSetMacro( Mean, double );
  const double & GetMean() const
    {
    return m_Mean;
    }

  void SetMean( const double & v )
    {
    m_Mean = v;
    }

//   itkGetConstMacro( Sum, double );
//   itkSetMacro( Sum, double );
  const double & GetSum() const
    {
    return m_Sum;
    }

  void SetSum( const double & v )
    {
    m_Sum = v;
    }

//   itkGetConstMacro( Sigma, double );
//   itkSetMacro( Sigma, double );
  const double & GetSigma() const
    {
    return m_Sigma;
    }

  void SetSigma( const double & v )
    {
    m_Sigma = v;
    }

//   itkGetConstMacro( Variance, double );
//   itkSetMacro( Variance, double );
  const double & GetVariance() const
    {
    return m_Variance;
    }

  void SetVariance( const double & v )
    {
    m_Variance = v;
    }

//   itkGetConstMacro( Median, double );
//   itkSetMacro( Median, double );
  const double & GetMedian() const
    {
    return m_Median;
    }

  void SetMedian( const double & v )
    {
    m_Median = v;
    }

//   itkGetConstMacro( MaximumIndex, IndexType );
//   itkSetMacro( MaximumIndex, IndexType );
  const IndexType & GetMaximumIndex() const
    {
    return m_MaximumIndex;
    }

  void SetMaximumIndex( const IndexType & v )
    {
    m_MaximumIndex = v;
    }

//   itkGetConstMacro( MinimumIndex, IndexType );
//   itkSetMacro( MinimumIndex, IndexType );
  const IndexType & GetMinimumIndex() const
    {
    return m_MinimumIndex;
    }

  void SetMinimumIndex( const IndexType & v )
    {
    m_MinimumIndex = v;
    }

//   itkGetConstMacro( CenterOfGravity, PointType );
//   itkSetMacro( CenterOfGravity, PointType );
  const PointType & GetCenterOfGravity() const
    {
    return m_CenterOfGravity;
    }

  void SetCenterOfGravity( const PointType & v )
    {
    m_CenterOfGravity = v;
    }

protected:
  StatisticsLabelObject()
    {
    m_Minimum = 0;
    m_Maximum = 0;
    m_Mean = 0;
    m_Sum = 0;
    m_Sigma = 0;
    m_Variance = 0;
    m_Median = 0;
    m_MaximumIndex.Fill(0);
    m_MinimumIndex.Fill(0);
    m_CenterOfGravity.Fill(0);
    }
  

  void PrintSelf(std::ostream& os, Indent indent) const
    {
    Superclass::PrintSelf( os, indent );

    os << indent << "Minimum: " << m_Minimum << std::endl;
    os << indent << "Maximum: " << m_Maximum << std::endl;
    os << indent << "Mean: " << m_Mean << std::endl;
    os << indent << "Sum: " << m_Sum << std::endl;
    os << indent << "Sigma: " << m_Sigma << std::endl;
    os << indent << "Variance: " << m_Variance << std::endl;
    os << indent << "Median: " << m_Median << std::endl;
    os << indent << "MaximumIndex: " << m_MaximumIndex << std::endl;
    os << indent << "MinimumIndex: " << m_MinimumIndex << std::endl;
    os << indent << "CenterOfGravity: " << m_CenterOfGravity << std::endl;
    }

private:
  StatisticsLabelObject(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  double m_Minimum;
  double m_Maximum;
  double m_Mean;
  double m_Sum;
  double m_Sigma;
  double m_Variance;
  double m_Median;
  IndexType m_MaximumIndex;
  IndexType m_MinimumIndex;
  PointType m_CenterOfGravity;

};

} // end namespace itk

#endif
