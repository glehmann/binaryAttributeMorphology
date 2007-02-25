/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkInPlaceLabelCollectionImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2004/07/11 14:56:39 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

  Portions of this code are covered under the VTK copyright.
  See VTKCopyright.txt or http://www.kitware.com/VTKCopyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkInPlaceLabelCollectionImageFilter_txx
#define _itkInPlaceLabelCollectionImageFilter_txx
#include "itkInPlaceLabelCollectionImageFilter.h"


namespace itk
{

/**
 *
 */
template <class TInputImage>
InPlaceLabelCollectionImageFilter<TInputImage>
::InPlaceLabelCollectionImageFilter()
  : m_InPlace(true)
{
}

/**
 *
 */
template <class TInputImage>
InPlaceLabelCollectionImageFilter<TInputImage>
::~InPlaceLabelCollectionImageFilter()
{
}
  


template<class TInputImage>
void 
InPlaceLabelCollectionImageFilter<TInputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  os << indent << "InPlace: " << (m_InPlace ? "On" : "Off") << std::endl;
  if ( this->CanRunInPlace())
    {
    os << indent << "The input and output to this filter are the same type. The filter can be run in place." << std::endl;
    }
  else
    {
    os << indent << "The input and output to this filter are different types. The filter cannot be run in place." << std::endl;
    }
}

template<class TInputImage>
void 
InPlaceLabelCollectionImageFilter<TInputImage>
::AllocateOutputs()
{
  // if told to run in place and the types support it, 
  if( m_InPlace && this->CanRunInPlace() )
    {
    // Graft this first input to the output.  Later, we'll need to
    // remove the input's hold on the bulk data.
    //
    OutputImagePointer inputAsOutput
      = dynamic_cast<TOutputImage *>(const_cast<TInputImage *>(this->GetInput()));
    if (inputAsOutput)
      {
      this->GraftOutput( inputAsOutput );
      }
    }
  else
    {
    Superclass::AllocateOutputs();
    // copy the content of the input image to the output image
    const TInputImage * input = this->GetInput();
    TOutputImage * output = this->GetOutput();
    
    output->SetUseBackground( input->GetUseBackground() );
    output->SetBackgroundValue( input->GetBackgroundValue() );

    typedef typename InputImageType::LabelObjectType LabelObjectType;

    typename InputImageType::LabelObjectContainerType::const_iterator it;
    const typename InputImageType::LabelObjectContainerType & labelObjectContainer = input->GetLabelObjectContainer();
    for( it = labelObjectContainer.begin(); it != labelObjectContainer.end(); it++ )
      {
      const LabelObjectType * labeObject = it->second;

      typename LabelObjectType::Pointer newLabelObject = LabelObjectType::New();
      newLabelObject->CopyDataFrom( labeObject );
      
      output->AddLabelObject( newLabelObject );
      }

    }
}

template<class TInputImage>
void 
InPlaceLabelCollectionImageFilter<TInputImage>
::ReleaseInputs()
{
  // if told to run in place and the types support it, 
  if (m_InPlace && (typeid(TInputImage) == typeid(TOutputImage)))
    {
    // Release any input where the ReleaseData flag has been set
    ProcessObject::ReleaseInputs();
    
    // Release input 0 by default since we overwrote it
    TInputImage * ptr = const_cast<TInputImage*>( this->GetInput() );
    if( ptr )
      {
      ptr->ReleaseData();
      }
    }
  else
    {
    Superclass::ReleaseInputs();
    }
}


template <class TInputImage>
void 
InPlaceLabelCollectionImageFilter<TInputImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  
  if ( !input )
    { return; }

  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
}


template <class TInputImage>
void 
InPlaceLabelCollectionImageFilter<TInputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


} // end namespace itk

#endif
