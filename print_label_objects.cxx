#include "itkImageFileReader.h"
#include "itkStatisticsLabelObject.h"
#include "itkLabelCollectionImage.h"
#include "itkBinaryImageToLabelCollectionImageFilter.h"
#include "itkStatisticsLabelCollectionImageFilter.h"

int main(int argc, char * argv[])
{
  const int dim = 2;
  typedef unsigned char PixelType;
  typedef itk::Image< PixelType, dim >    ImageType;
  
  if( argc != 4)
    {
    std::cerr << "usage: " << argv[0] << " input featureImg foreground" << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  // read the input image
  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  
  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName( argv[2] );
  
  // define the object type. Here the StatisticsLabelObject type
  // is chosen in order to read some attribute related to the statistics
  // of the objects
  typedef unsigned long LabelType;
  typedef itk::StatisticsLabelObject< LabelType, dim > LabelObjectType;
  typedef itk::LabelCollectionImage< LabelObjectType > LabelCollectionType;

  // convert the image in a collection of objects
  typedef itk::BinaryImageToLabelCollectionImageFilter< ImageType, LabelCollectionType > ConverterType;
  ConverterType::Pointer converter = ConverterType::New();
  converter->SetInput( reader->GetOutput() );
  converter->SetForegroundValue( atoi(argv[3]) );
  converter->SetFullyConnected( true );

  // and valuate the attributes with the dedicated filter: StatisticsLabelCollectionImageFilter
  typedef itk::StatisticsLabelCollectionImageFilter< LabelCollectionType, ImageType > StatisticsFilterType;
  StatisticsFilterType::Pointer statistics = StatisticsFilterType::New();
  statistics->SetComputeFeretDiameter( false );
  statistics->SetInput( converter->GetOutput() );
  statistics->SetFeatureImage( reader2->GetOutput() );
  statistics->SetComputePerimeter( true );

  // update the statistics filter, so its output will be up to date
  statistics->Update();

  statistics->GetOutput()->PrintObjects();

  return 0;
}

