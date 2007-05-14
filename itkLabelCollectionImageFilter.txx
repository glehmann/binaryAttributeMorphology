/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkLabelCollectionImageFilter.txx,v $
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
#ifndef _itkLabelCollectionImageFilter_txx
#define _itkLabelCollectionImageFilter_txx
#include "itkLabelCollectionImageFilter.h"


namespace itk
{

/**
 *
 */
template <class TInputImage, class TOutputImage>
LabelCollectionImageFilter<TInputImage, TOutputImage>
::LabelCollectionImageFilter()
{
}

/**
 *
 */
template <class TInputImage, class TOutputImage>
LabelCollectionImageFilter<TInputImage, TOutputImage>
::~LabelCollectionImageFilter()
{
}


template <class TInputImage, class TOutputImage>
void 
LabelCollectionImageFilter<TInputImage, TOutputImage>
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


template <class TInputImage, class TOutputImage>
void 
LabelCollectionImageFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage>
void
LabelCollectionImageFilter<TInputImage, TOutputImage>
::GenerateData()
{

  // initialize the iterator
  m_LabelObjectIterator = this->GetLabelCollectionImage()->GetLabelObjectContainer().begin();

  // and the mutex
  m_LabelObjectIteratorLock = FastMutexLock::New();
  m_LabelObjectIteratorLock->Unlock();
  m_LabelObjectIteratorLock->Print( std::cout );

  // initialize the progress reporter
  // TODO: really report the progress!
  ProgressReporter progress( this, 0, this->GetLabelCollectionImage()->GetNumberOfObjects() );

  Superclass::GenerateData();
}


template <class TInputImage, class TOutputImage>
void
LabelCollectionImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData( const OutputImageRegionType&, int threadId )
{
  while( true )
    {
    // first lock the mutex
    m_LabelObjectIteratorLock->Lock();

    if( m_LabelObjectIterator == this->GetLabelCollectionImage()->GetLabelObjectContainer().end() )
      {
      // no more objects. Release the lock and return
      m_LabelObjectIteratorLock->Unlock();
      return;
      }

    // get the label object
    LabelObjectType * labelObject = m_LabelObjectIterator->second;
    // increment the iterator now, so it will not be invalidated if the object is destroyed
    m_LabelObjectIterator++;
    // pretend one more object is processed, even if it will be done later, to simplify the lock management
    // TODO: progress++
    // unlock the mutex, so the other threads can get an object
    m_LabelObjectIteratorLock->Unlock();
    // and run the user defined method for that object
    ThreadedGenerateData( labelObject );
    }
}


template <class TInputImage, class TOutputImage>
void
LabelCollectionImageFilter<TInputImage, TOutputImage>
::ThreadedGenerateData( LabelObjectType * labelObject )
{
  // do nothing
  // the subclass should override this method
}

} // end namespace itk

#endif
