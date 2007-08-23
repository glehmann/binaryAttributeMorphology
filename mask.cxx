#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkLabelObject.h"
#include "itkLabelCollectionImage.h"
#include "itkLabelImageToLabelCollectionImageFilter.h"
#include "itkLabelCollectionImageToMaskImageFilter.h"


int main(int argc, char * argv[])
{

  if( argc != 8 )
    {
    std::cerr << "usage: " << argv[0] << " input input2 output label bg neg crop" << std::endl;
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
  
  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName( argv[2] );
  
  typedef itk::LabelImageToLabelCollectionImageFilter< IType, LabelCollectionImageType> I2LType;
  I2LType::Pointer i2l = I2LType::New();
  i2l->SetInput( reader->GetOutput() );
  i2l->SetUseBackground( true );

  typedef itk::LabelCollectionImageToMaskImageFilter< LabelCollectionImageType, IType > MaskType;
  MaskType::Pointer mask = MaskType::New();
  mask->SetInput( i2l->GetOutput() );
  mask->SetFeatureImage( reader2->GetOutput() );
  mask->SetLabel( atoi(argv[4]) );
  mask->SetBackgroundValue( atoi(argv[5]) );
  mask->SetNegated( atoi(argv[6]) );
  mask->SetCrop( atoi(argv[7]) );
  itk::SimpleFilterWatcher watcher6(mask, "filter");

  typedef itk::ImageFileWriter< IType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( mask->GetOutput() );
  writer->SetFileName( argv[3] );
  writer->Update();

  return 0;
}

