/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkStatisticsRelabelLabelCollectionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2006/03/28 19:59:05 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkStatisticsRelabelLabelCollectionImageFilter_h
#define __itkStatisticsRelabelLabelCollectionImageFilter_h

#include "itkShapeRelabelLabelCollectionImageFilter.h"
#include "itkStatisticsLabelObject.h"

namespace itk {
/** \class StatisticsRelabelLabelCollectionImageFilter
 * \brief relabel objects according to their shape attributes
 *
 * StatisticsRelabelLabelCollectionImageFilter relabel a label collection image according to the statistics attributes of
 * the objects. The label produced are always consecutives.
 *
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa StatisticsLabelObject, RelabelComponentImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage >
class ITK_EXPORT StatisticsRelabelLabelCollectionImageFilter : 
    public ShapeRelabelLabelCollectionImageFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef StatisticsRelabelLabelCollectionImageFilter Self;
  typedef ShapeRelabelLabelCollectionImageFilter<TImage>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TImage ImageType;
  typedef typename ImageType::Pointer         ImagePointer;
  typedef typename ImageType::ConstPointer    ImageConstPointer;
  typedef typename ImageType::PixelType       PixelType;
  typedef typename ImageType::IndexType       IndexType;
  typedef typename ImageType::LabelObjectType LabelObjectType;
  
  typedef typename LabelObjectType::AttributeType AttributeType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);

  /** Runtime information support. */
  itkTypeMacro(StatisticsRelabelLabelCollectionImageFilter, 
               ShapeRelabelLabelCollectionImageFilter);

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

  /**
   * Set/Get the order of labeling of the objects. By default, the objects with
   * the highest attribute values are labeled first. Set ReverseOrdering to true
   * make the one with the smallest attributes be labeled first.
   */
  itkSetMacro( ReverseOrdering, bool );
  itkGetConstReferenceMacro( ReverseOrdering, bool );
  itkBooleanMacro( ReverseOrdering );

  /**
   * Set/Get the attribute to use. Default is "Mean".
   */
  itkGetConstMacro( Attribute, AttributeType );
  itkSetMacro( Attribute, AttributeType );
  void SetAttribute( const std::string & s )
    {
    this->SetAttribute( LabelObjectType::GetAttributeFromName( s ) );
    }


protected:
  StatisticsRelabelLabelCollectionImageFilter();
  ~StatisticsRelabelLabelCollectionImageFilter() {};

  void GenerateData();

  template <class TAttributeAccessor> void TemplatedGenerateData();
  
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  StatisticsRelabelLabelCollectionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_ReverseOrdering;
  AttributeType m_Attribute;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkStatisticsRelabelLabelCollectionImageFilter.txx"
#endif

#endif


