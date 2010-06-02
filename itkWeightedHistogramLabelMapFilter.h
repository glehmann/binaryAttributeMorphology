/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkWeightedHistogramLabelMapFilter.h,v $
  Language:  C++
  Date:      $Date: 2009-08-08 01:21:23 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkWeightedHistogramLabelMapFilter_h
#define __itkWeightedHistogramLabelMapFilter_h

#include "itkInPlaceLabelMapFilter.h"
#include "itkStatisticsLabelObjectAccessors.h"

namespace itk {
/** \class WeightedHistogramLabelMapFilter
 * \brief The valuator class for the ShapeLabelObject
 *
 * StatisticsCollectionImageFilter can be used to set the attributes values
 * of the StatisticsLabelObject in a LabelMap.
 *
 * \author Gaetan Lehmann. Biologie du Developpement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * This implementation was taken from the Insight Journal paper:
 * http://hdl.handle.net/1926/584  or 
 * http://www.insight-journal.org/browse/publication/176
 *
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TImage, class TFeatureImage, class TWeightImage=TFeatureImage,
         class THistogramAccessor=typename Functor::HistogramLabelObjectAccessor<typename TImage::LabelObjectType> >
class ITK_EXPORT WeightedHistogramLabelMapFilter : 
    public InPlaceLabelMapFilter<TImage>
{
public:
  /** Standard class typedefs. */
  typedef WeightedHistogramLabelMapFilter    Self;
  typedef InPlaceLabelMapFilter<TImage> Superclass;
  typedef SmartPointer<Self>          Pointer;
  typedef SmartPointer<const Self>    ConstPointer;

  /** Some convenient typedefs. */
  typedef TImage                               ImageType;
  typedef typename ImageType::Pointer          ImagePointer;
  typedef typename ImageType::ConstPointer     ImageConstPointer;
  typedef typename ImageType::PixelType        PixelType;
  typedef typename ImageType::IndexType        IndexType;
  typedef typename ImageType::PointType        PointType;
  typedef typename ImageType::LabelObjectType  LabelObjectType;
  
  typedef TFeatureImage                              FeatureImageType;
  typedef typename FeatureImageType::Pointer         FeatureImagePointer;
  typedef typename FeatureImageType::ConstPointer    FeatureImageConstPointer;
  typedef typename FeatureImageType::PixelType       FeatureImagePixelType;
  
  typedef TWeightImage                              WeightImageType;
  typedef typename WeightImageType::Pointer         WeightImagePointer;
  typedef typename WeightImageType::ConstPointer    WeightImageConstPointer;
  typedef typename WeightImageType::PixelType       WeightImagePixelType;
  
  typedef THistogramAccessor                        HistogramAccessorType;

  /** ImageDimension constants */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(WeightedHistogramLabelMapFilter, 
               ShapeLabelMapFilter);

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

   /** Set the feature image */
  void SetFeatureImage(const TFeatureImage *input)
    {
    // Process object is not const-correct so the const casting is required.
    this->SetNthInput( 1, const_cast<TFeatureImage *>(input) );
    }

  /** Get the feature image */
  FeatureImageType * GetFeatureImage()
    {
    return static_cast<FeatureImageType*>(const_cast<DataObject *>(this->ProcessObject::GetInput(1)));
    }

   /** Set the weight image */
  void SetWeightImage(const TWeightImage *input)
    {
    // Process object is not const-correct so the const casting is required.
    this->SetNthInput( 2, const_cast<TWeightImage *>(input) );
    }

  /** Get the weight image */
  WeightImageType * GetWeightImage()
    {
    return static_cast<WeightImageType*>(const_cast<DataObject *>(this->ProcessObject::GetInput(2)));
    }

   /** Set the input image */
  void SetInput1(TImage *input)
    {
    this->SetInput( input );
    }

  /** Set the feature image */
  void SetInput2(const TFeatureImage *input)
    {
    this->SetFeatureImage( input );
    }

  /** Set the feature image */
  void SetInput3(const TWeightImage *input)
    {
    this->SetWeightImage( input );
    }

  /**
   * Set/Get the number of bins in the histogram. Note that the histogram is used
   * to compute the median value, and that this option may have an effect on the
   * value of the median.
   */
  itkSetMacro(NumberOfBins, unsigned int);
  itkGetConstReferenceMacro(NumberOfBins, unsigned int);


protected:
  WeightedHistogramLabelMapFilter();
  ~WeightedHistogramLabelMapFilter() {};

  virtual void ThreadedProcessLabelObject( LabelObjectType * labelObject );
  
  virtual void BeforeThreadedGenerateData();
  
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  WeightedHistogramLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  FeatureImagePixelType m_Minimum;
  FeatureImagePixelType m_Maximum;
  unsigned int          m_NumberOfBins;

}; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkWeightedHistogramLabelMapFilter.txx"
#endif

#endif
