#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkLabelObject.h"
#include "itkLabelCollectionImage.h"
//#include "itkBinaryImageToLabelCollectionImageFilter.h"


int main(int argc, char * argv[])
{

  if( argc != 2 )
    {
    std::cerr << "usage: " << argv[0] << " " << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  const int dim = 2;
  
  typedef itk::LabelObject< unsigned long, dim > LabelObjectType;
  typedef itk::LabelCollectionImage< LabelObjectType > LabelCollectionImageType;
  
  typedef itk::Image< unsigned char, 3 > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();
  
  LabelCollectionImageType::Pointer labelCollection = LabelCollectionImageType::New();
//   labelCollection->SetRegions( reader->GetOutput()->GetLargestPossibleRegion() );
  LabelCollectionImageType::IndexType idx;
  idx.Fill( 0 );
//   std::cout << labelCollection->HasLabel( 1 ) << std::endl;
//  labelCollection->SetPixel( idx, 1 );
  LabelObjectType::Pointer lo = LabelObjectType::New();
  lo->SetLabel( 1 );
  
  std::cout << "lo: " << lo << std::endl;
  labelCollection->AddLabelObject( lo );
  
  labelCollection->SetPixel( idx, 2 );
  
  LabelObjectType::Pointer lo1 = labelCollection->GetLabelObject( 1 );
  std::cout << "lo1: " << lo1 << std::endl;

  LabelObjectType::Pointer lo2 = labelCollection->GetLabelObject( 2 );
  std::cout << "lo2: " << lo2 << std::endl;

/*  typedef itk::BinaryImageToLabelCollectionImageFilter< IType, LabelCollectionImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );

  itk::SimpleFilterWatcher watcher(filter, "filter");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();
*/
  return 0;
}

