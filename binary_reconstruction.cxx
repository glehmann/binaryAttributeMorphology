#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkLabelObject.h"
#include "itkLabelCollectionImage.h"
#include "itkBinaryImageToLabelCollectionImageFilter.h"
#include "itkLabelCollectionImageToBinaryImageFilter.h"
#include "itkReconstructionLabelCollectionImageFilter.h"


int main(int argc, char * argv[])
{

  if( argc != 7 )
    {
    std::cerr << "usage: " << argv[0] << " input mask output conn fg bg" << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  const int dim = 3;
  
  typedef itk::LabelObject< unsigned long, dim > LabelObjectType;
  typedef itk::LabelCollectionImage< LabelObjectType > LabelCollectionImageType;
  
  typedef itk::Image< unsigned char, dim > IType;

  typedef itk::ImageFileReader< IType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  reader->Update();
  
  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName( argv[2] );
  reader2->Update();
  
 typedef itk::BinaryImageToLabelCollectionImageFilter< IType, LabelCollectionImageType> I2LType;
  I2LType::Pointer i2l = I2LType::New();
  i2l->SetInput( reader->GetOutput() );
  i2l->SetUseBackground( true );
  i2l->SetFullyConnected( atoi(argv[4]) );
  i2l->SetForegroundValue( atoi(argv[5]) );
  i2l->SetBackgroundValue( atoi(argv[6]) );
//   itk::SimpleFilterWatcher watcher(i2l, "filter");

  typedef itk::ReconstructionLabelCollectionImageFilter< LabelCollectionImageType, IType > ReconstructionType;
  ReconstructionType::Pointer reconstruction = ReconstructionType::New();
  reconstruction->SetInput( i2l->GetOutput() );
  reconstruction->SetMaskImage( reader2->GetOutput() );
  reconstruction->SetInPlace( true );
  reconstruction->SetForegroundValue( atoi(argv[5]) );
  itk::SimpleFilterWatcher watcher3(reconstruction, "filter");

  typedef itk::LabelCollectionImageToBinaryImageFilter< LabelCollectionImageType, IType > L2IType;
  L2IType::Pointer l2i = L2IType::New();
  l2i->SetInput( reconstruction->GetOutput() );
  l2i->SetForegroundValue( atoi(argv[5]) );
  l2i->SetBackgroundValue( atoi(argv[6]) );
  l2i->SetBackgroundImage( reader->GetOutput() );
//   itk::SimpleFilterWatcher watcher2(l2i, "filter");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( l2i->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();
  return 0;
}

