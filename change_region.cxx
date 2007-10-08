#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkSimpleFilterWatcher.h"

#include "itkLabelObject.h"
#include "itkLabelMap.h"
#include "itkLabelImageToLabelMapFilter.h"
#include "itkChangeRegionLabelMapFilter.h"
#include "itkLabelMapToLabelImageFilter.h"


int main(int argc, char * argv[])
{

  if( argc != 7 )
    {
    std::cerr << "usage: " << argv[0] << " input output idx0 idx1 size0 size1" << std::endl;
    // std::cerr << "  : " << std::endl;
    exit(1);
    }

  // the filters are able to work in any dimension. Lets choose 3, so the program can be tested on
  // 2D and 2D image.
  const int dim = 2;
  
  // declare the input image type
  typedef itk::Image< unsigned char, dim > ImageType;

  // and the label object type to use. The input image is a label image, so the 
  // type of the label can be the same type than the pixel type. itk::LabelObject is
  // chosen, because only the change feature is tested here, so we don't need any
  // attribute.
  typedef itk::LabelObject< unsigned char, dim > LabelObjectType;
  typedef itk::LabelMap< LabelObjectType > LabelMapType;
  
  // read the label image and the input image to be changeed.
  typedef itk::ImageFileReader< ImageType > ReaderType;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName( argv[1] );
  
  // convert the label image to a label collection image.
  typedef itk::LabelImageToLabelMapFilter< ImageType, LabelMapType> I2LType;
  I2LType::Pointer i2l = I2LType::New();
  i2l->SetInput( reader->GetOutput() );
//  i2l->SetUseBackground( true );

  // then change the image. Two inputs are required (the label collection image, and
  // the image to be changeed). The label used to change the image is passed with the 
  // SetLabel() method. The background in the output image, where the image is changeed,
  // is passed with SetBackground(). The user can choose to change the image outside the
  // label object (that's the default behavior), or inside the label object with the
  // chosen label, by calling SetNegated(). Finally, the image can be cropped to the
  // changeed region, by calling SetCrop( true ), or to a region padded by a border, by
  // calling both SetCrop() and SetCropBorder(). The crop border defaults to 0, and the
  // image is not cropped by default.
  typedef itk::ChangeRegionLabelMapFilter< LabelMapType > ChangeType;
  ChangeType::Pointer change = ChangeType::New();
  change->SetInput( i2l->GetOutput() );
  ChangeType::IndexType idx;
  idx[0] = atoi( argv[3] );
  idx[1] = atoi( argv[4] );
  ChangeType::SizeType size;
  size[0] = atoi( argv[5] );
  size[1] = atoi( argv[5] );
  ChangeType::RegionType region;
  region.SetSize( size );
  region.SetIndex( idx );
  change->SetRegion( region );
  itk::SimpleFilterWatcher watcher6(change, "filter");

  typedef itk::LabelMapToLabelImageFilter< LabelMapType, ImageType> L2IType;
  L2IType::Pointer l2i = L2IType::New();
  l2i->SetInput( change->GetOutput() );

  // Finally, save the output image.
  typedef itk::ImageFileWriter< ImageType > WriterType;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( l2i->GetOutput() );
  writer->SetFileName( argv[2] );
  writer->Update();

  return 0;
}

