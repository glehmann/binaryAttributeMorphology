#include "itkImageFileReader.h"
#include "itkStatisticsLabelObject.h"
#include "itkLabelMap.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "itkWeightedHistogramLabelMapFilter.h"
#include "itkSimpleFilterWatcher.h"

int main(int argc, char * argv[])
{
  const int dim = 3;
  typedef unsigned char PixelType;
  typedef itk::Image< PixelType, dim >    ImageType;
  
  if( argc != 4)
    {
    std::cerr << "usage: " << argv[0] << " labelImage featureImage weightImage" << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader1 = ReaderType::New();
  reader1->SetFileName( argv[1] );
  
  ReaderType::Pointer reader2 = ReaderType::New();
  reader2->SetFileName( argv[2] );
  
  ReaderType::Pointer reader3 = ReaderType::New();
  reader3->SetFileName( argv[3] );
  
  typedef itk::StatisticsLabelObject< unsigned long, dim > LabelObjectType;
  typedef itk::LabelMap< LabelObjectType > LabelMapType;
  typedef itk::LabelImageToLabelMapFilter< ImageType, LabelMapType > ConverterType;
  ConverterType::Pointer converter = ConverterType::New();
  converter->SetInput( reader1->GetOutput() );

  typedef itk::WeightedHistogramLabelMapFilter< LabelMapType, ImageType, ImageType > HistogramCalculatorType;
  HistogramCalculatorType::Pointer calc = HistogramCalculatorType::New();
  calc->SetInput( converter->GetOutput() );
  calc->SetFeatureImage( reader2->GetOutput() );
  calc->SetWeightImage( reader3->GetOutput() );
  calc->SetNumberOfBins( 10 );
  itk::SimpleFilterWatcher watcher(calc, "filter");

  calc->Update();
  LabelObjectType * lo = calc->GetOutput()->GetNthLabelObject(0);
  const LabelObjectType::HistogramType * histogram = lo->GetHistogram();
  histogram->Print(std::cout);

  for( unsigned long i=0; i<histogram->Size(); i++)
    {
    std::cout << histogram->GetMeasurementVector( i )[0] << ": " << histogram->GetFrequency( i ) << std::endl;
    }
  return 0;
}