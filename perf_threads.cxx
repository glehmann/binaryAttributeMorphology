#include "itkImageFileReader.h"
#include "itkShapeLabelObject.h"
#include "itkLabelCollectionImage.h"
#include "itkBinaryImageToLabelCollectionImageFilter.h"
#include "itkShapeLabelCollectionImageFilter.h"
#include "itkTimeProbe.h"

#include <iomanip>

int main(int, char * argv[])
{
  const int dim = 3;
  typedef unsigned char PixelType;
  typedef itk::Image< PixelType, dim >    ImageType;
  
  // read the input image
  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  
  typedef unsigned long LabelType;
  typedef itk::ShapeLabelObject< LabelType, dim > LabelObjectType;
  typedef itk::LabelCollectionImage< LabelObjectType > LabelCollectionType;

  typedef itk::BinaryImageToLabelCollectionImageFilter< ImageType, LabelCollectionType > ConverterType;
  ConverterType::Pointer converter = ConverterType::New();
  converter->SetInput( reader->GetOutput() );
  converter->SetForegroundValue( 200 );
  converter->Update();

  typedef itk::ShapeLabelCollectionImageFilter< LabelCollectionType > ShapeFilterType;
  ShapeFilterType::Pointer shape = ShapeFilterType::New();
  shape->SetInPlace( false );  // to avoid running the converter each time
  shape->SetInput( converter->GetOutput() );
  
  std::cout << "#nb" << "\t" 
            << "shape" << std::endl;

  for( int t=1; t<=10; t++ )
    {
    itk::TimeProbe shapeTime;
  
    shape->SetNumberOfThreads( t );
    
    for( int i=0; i<50; i++ )
      {
      shapeTime.Start();
      shape->Update();
      shapeTime.Stop();
      
      shape->Modified();
      }
      
    std::cout << std::setprecision(3) << t << "\t" 
              << shapeTime.GetMeanTime() << std::endl;
    }
  
  
  return 0;
}

