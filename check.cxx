#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkShapeLabelObject.h"
#include "itkLabelCollectionImage.h"
#include "itkBinaryImageToLabelCollectionImageFilter.h"
#include "itkLabelCollectionImageToBinaryImageFilter.h"
#include "itkLabelCollectionImageToLabelImageFilter.h"
#include "itkShapeLabelCollectionImageFilter.h"
#include "itkSizeOpeningLabelCollectionImageFilter.h"
#include "itkSizeRelabelLabelCollectionImageFilter.h"
#include "itkLabelSizeOpeningImageFilter.h"
#include "itkSizeRelabelImageFilter.h"


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
  
//   typedef itk::BinaryImageToLabelCollectionImageFilter< IType, LabelCollectionImageType> I2LType;
//   I2LType::Pointer i2l = I2LType::New();
//   i2l->SetInput( reader->GetOutput() );
//   i2l->SetUseBackground( true );
//   itk::SimpleFilterWatcher watcher(i2l, "filter");
// 
//   typedef itk::ShapeLabelCollectionImageFilter< LabelCollectionImageType > InPlaceType;
//   InPlaceType::Pointer inplace = InPlaceType::New();
//   inplace->SetInput( i2l->GetOutput() );
//   inplace->SetInPlace( true );
//   itk::SimpleFilterWatcher watcher3(inplace, "filter");
// 
//   typedef itk::SizeRelabelLabelCollectionImageFilter< LabelCollectionImageType > RelabelType;
//   RelabelType::Pointer relabel = RelabelType::New();
//   relabel->SetInput( inplace->GetOutput() );
//   relabel->SetReverseOrdering( true );
//   itk::SimpleFilterWatcher watcher5(relabel, "filter");

//   typedef itk::SizeOpeningLabelCollectionImageFilter< LabelCollectionImageType > OpenType;
//   OpenType::Pointer open = OpenType::New();
//   open->SetInput( inplace->GetOutput() );
//   open->SetInPlace( true );
//   open->SetLambda( 3 );
//   itk::SimpleFilterWatcher watcher4(inplace, "filter");

//   typedef itk::LabelCollectionImageToBinaryImageFilter< LabelCollectionImageType, IType > L2IType;
//   L2IType::Pointer l2i = L2IType::New();
//   l2i->SetInput( open->GetOutput() );
//   itk::SimpleFilterWatcher watcher2(l2i, "filter");

//   typedef itk::LabelCollectionImageToLabelImageFilter< LabelCollectionImageType, IType > L2IType;
//   L2IType::Pointer l2i = L2IType::New();
//   l2i->SetInput( relabel->GetOutput() );
//   itk::SimpleFilterWatcher watcher2(l2i, "filter");

//   typedef itk::LabelSizeOpeningImageFilter< IType > BinaryOpeningType;
//   BinaryOpeningType::Pointer opening = BinaryOpeningType::New();
//   opening->SetInput( reader->GetOutput() );

  typedef itk::SizeRelabelImageFilter< IType > RelabelType;
  RelabelType::Pointer relabel = RelabelType::New();
  relabel->SetInput( reader->GetOutput() );


  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( relabel->GetOutput() );
//   writer->SetInput( l2i->GetOutput() );
//   writer->SetInput( opening->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();
  return 0;
}

