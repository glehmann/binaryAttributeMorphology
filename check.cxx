#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkShapeLabelObject.h"
#include "itkLabelCollectionImage.h"
#include "itkLabelImageToLabelCollectionImageFilter.h"
#include "itkLabelCollectionImageToLabelImageFilter.h"
#include "itkShapeLabelCollectionImageFilter.h"
#include "itkSizeOpeningLabelCollectionImageFilter.h"


int main(int argc, char * argv[])
{

  if( argc != 3 )
    {
    std::cerr << "usage: " << argv[0] << " " << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  const int dim = 3;
  
  typedef itk::ShapeLabelObject< unsigned long, dim > LabelObjectType;
  typedef itk::LabelCollectionImage< LabelObjectType > LabelCollectionImageType;
  
  typedef itk::Image< unsigned char, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  
  typedef itk::LabelImageToLabelCollectionImageFilter< IType, LabelCollectionImageType> I2LType;
  I2LType::Pointer i2l = I2LType::New();
  i2l->SetInput( reader->GetOutput() );
  i2l->SetUseBackground( true );
  itk::SimpleFilterWatcher watcher(i2l, "filter");

  typedef itk::ShapeLabelCollectionImageFilter< LabelCollectionImageType > InPlaceType;
  InPlaceType::Pointer inplace = InPlaceType::New();
  inplace->SetInput( i2l->GetOutput() );
  inplace->SetInPlace( true );
  itk::SimpleFilterWatcher watcher3(inplace, "filter");

  typedef itk::SizeOpeningLabelCollectionImageFilter< LabelCollectionImageType > OpenType;
  OpenType::Pointer open = OpenType::New();
  open->SetInput( inplace->GetOutput() );
  open->SetInPlace( true );
  open->SetLambda( 3 );
  itk::SimpleFilterWatcher watcher4(inplace, "filter");

  typedef itk::LabelCollectionImageToLabelImageFilter< LabelCollectionImageType, IType > L2IType;
  L2IType::Pointer l2i = L2IType::New();
  l2i->SetInput( open->GetOutput() );
  itk::SimpleFilterWatcher watcher2(l2i, "filter");

  l2i->Update();


/*  typedef itk::BinaryImageToLabelCollectionImageFilter< IType, LabelCollectionImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput( reader->GetOutput() );

  itk::SimpleFilterWatcher watcher(filter, "filter");
*/

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( l2i->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();
  return 0;
}

