FIND_PACKAGE(WrapITK REQUIRED)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR})

# include new declaration of types
INCLUDE(WrapITKTypes.cmake)

BEGIN_WRAPPER_LIBRARY("${PROJECT_NAME}")
SET(WRAPPER_LIBRARY_DEPENDS Base BaseTransforms)

SET(WRAPPER_LIBRARY_GROUPS
  itkLabelObject
  itkShapeLabelObject
  itkStatisticsLabelObject
  itkLabelCollectionImage
  ITKLabelCollectionImageBase
  itkLabelCollectionImageFilter
  itkInPlaceLabelCollectionImageFilter
)

SET(WRAPPER_LIBRARY_LINK_LIBRARIES ITKCommon ITKStatistics)

WRAPPER_LIBRARY_CREATE_WRAP_FILES()
WRAPPER_LIBRARY_CREATE_LIBRARY()

