/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkAttributeUniqueLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkAttributeUniqueLabelMapFilter_txx
#define __itkAttributeUniqueLabelMapFilter_txx

#include "itkAttributeUniqueLabelMapFilter.h"
#include "itkProgressReporter.h"
#include <queue>


namespace itk {

template <class TImage, class TAttributeAccessor>
AttributeUniqueLabelMapFilter<TImage, TAttributeAccessor>
::AttributeUniqueLabelMapFilter()
{
  m_ReverseOrdering = false;
}


template <class TImage, class TAttributeAccessor>
void
AttributeUniqueLabelMapFilter<TImage, TAttributeAccessor>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  // the priority queue to store all the lines of all the objects sorted
  typedef typename std::priority_queue< Line, typename std::vector<Line>, LineComparator > PriorityQueueType;
  PriorityQueueType pq;

  ProgressReporter progress( this, 0, 1 );
  // TODO: really report the progress
  
  typedef typename ImageType::LabelObjectContainerType LabelObjectContainerType;

  const LabelObjectContainerType & labelObjects = this->GetLabelMap()->GetLabelObjectContainer();
  for( typename LabelObjectContainerType::const_iterator it2 = labelObjects.begin();
    it2 != labelObjects.end();
    it2++ )
    {
    LabelObjectType * lo = it2->second;
    
    // may reduce the number of lines to proceed
    lo->Optimize();
    
    typename LabelObjectType::LineContainerType::const_iterator lit;
    typename LabelObjectType::LineContainerType & lineContainer = lo->GetLineContainer();
  
    for( lit = lineContainer.begin(); lit != lineContainer.end(); lit++ )
      {
      pq.push( Line( *lit, lo ) );
      }
    
    // clear the lines to readd them later
    lineContainer.clear();
    
    // go to the next label
    // progress.CompletedPixel();
    }
    
  if( pq.empty() )
    {
    // nothing to do
    return;
    }
    
  AttributeAccessorType accessor;

  typedef typename std::deque<Line> LinesType;
  LinesType lines;

  lines.push_back( pq.top() );
  Line prev = lines.back();
  IndexType prevIdx = prev.line.GetIndex();
  pq.pop();
  
  while( !pq.empty() )
    {
    Line l = pq.top();
    IndexType idx = l.line.GetIndex();
    pq.pop();
    
    bool newMainLine = false;
    // don't check dim 0!
    for( int i=1; i<ImageDimension; i++ )
      {
      if( idx[i] != prevIdx[i] )
        {
        newMainLine = true;
        }
      }
    
    if( newMainLine )
      {
      // just push the line
      lines.push_back( l );      
      }
    else
      {
      unsigned long prevLength = prev.line.GetLength();
      unsigned long length = l.line.GetLength();
      
      if( prevIdx[0] + (long)prevLength >= idx[0] )
        {
        // the lines are overlapping. We need to choose which line to keep.
        // TODO: need to find a stable way to handle equal attributes
        if( ( !m_ReverseOrdering && accessor( l.labelObject ) > accessor( prev.labelObject ) )
            || ( m_ReverseOrdering && accessor( l.labelObject ) < accessor( prev.labelObject ) ) )
          {
          // keep the current one. We must truncate the previous one to remove the
          // overlap, and take care of the end of the previous line if it extends
          // after the current one.
          if( prevIdx[0] + prevLength > idx[0] + length )
            {
            // the previous line is longer than the current one. Lets take its tail and
            // add it to the priority queue
            IndexType newIdx = idx;
            newIdx[0] = idx[0] + length;
            unsigned long newLength = prevIdx[0] + prevLength - newIdx[0];
            pq.push( Line( LineType( newIdx, newLength ), prev.labelObject ) );
            }
          // truncate the previous line to let some place for the current one
          prevLength = idx[0] - prevIdx[0];
          if( prevLength != 0 )
            {
            lines.back().line.SetLength( idx[0] - prevIdx[0] );
            }
          else
            {
            // length is 0 - no need to keep that line
            lines.pop_back();
            }
          // and push the current one
          lines.push_back( l );
          }
        else
          {
          // keep the previous one. If the previous line fully overlap the current one,
          // the current one is fully discarded.
          if( prevIdx[0] + prevLength > idx[0] + length )
            {
            // discarding the current line - just do nothing
            }
          else
            {
            IndexType newIdx = idx;
            newIdx[0] = prevIdx[0] + prevLength;
            unsigned long newLength = idx[0] + length - newIdx[0];
            l.line.SetIndex( newIdx );
            l.line.SetLength( newLength );
            lines.push_back( l );
            }
          
          }
        }
      else
        {
        // no overlap - things are just fine already
        lines.push_back( l );
        }
      }
    
    // store the current line as the previous one, and go to the next one.
    prev = lines.back();
    prevIdx = prev.line.GetIndex();
    }
  
  // put the lines in their object
  for( unsigned int i=0; i<lines.size(); i++ )
    {
    Line & l = lines[i];
    l.labelObject->AddLine( l.line );
    }

  // remove objects without lines
  typename LabelObjectContainerType::const_iterator it = labelObjects.begin();
  while( it != labelObjects.end() )
    {
    typename LabelObjectType::LabelType label = it->first;
    LabelObjectType * labelObject = it->second;

    if( labelObject->Empty() )
      {
      // must increment the iterator before removing the object to avoid invalidating the iterator
      it++;
      this->GetLabelMap()->RemoveLabel( label );
      }
    else
      {
      it++;
      }

    }
}


template <class TImage, class TAttributeAccessor>
void
AttributeUniqueLabelMapFilter<TImage, TAttributeAccessor>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "ReverseOrdering: "  << m_ReverseOrdering << std::endl;
}

}// end namespace itk
#endif
