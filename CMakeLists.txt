#Change PROJECT_NAME to the name of your project
PROJECT(binaryAttributeMorphology)

# set the name of the input image used to run the test
SET(BINARY_IMAGE ${CMAKE_SOURCE_DIR}/images/2th_cthead1.png)
SET(LABEL_IMAGE ${CMAKE_SOURCE_DIR}/images/cthead1-label.png)

#include some macros from another file...
INCLUDE(IJMacros.txt)

#The following lines are required to use Dart
ENABLE_TESTING()
INCLUDE(Dart)
FIND_PROGRAM(ITK_TEST_DRIVER itkTestDriver)
SET(TEST_COMMAND ${ITK_TEST_DRIVER} --add-before-env PATH ${CMAKE_BINARY_DIR})

#Declare any external dependencies that your project may have here.
#examples include: ITK, VTK, JPEG, PNG, OpenGL, ZLIB, Perl, Java
#If you're not sure what name to use, look in the Modules directory of your
#cmake install and check that a file named Find(Package).cmake exists
SET(Required_Packages
  ITK
)

#this foreach loads all of the packages that you specified as required.
#It shouldn't need to be modified.
FOREACH(Package ${Required_Packages})
  LOADPACKAGE(${Package})
ENDFOREACH(Package)

#Set any libraries that your project depends on.
#examples: ITKCommon, VTKRendering, etc
SET(Libraries
  ITKCommon
  ITKBasicFilters
  ITKIO
  ITKStatistics
)


# install devel files
OPTION(INSTALL_DEVEL_FILES "Install C++ headers" ON)
IF(INSTALL_DEVEL_FILES)
FILE(GLOB develFiles *.h *.txx) 
FOREACH(f ${develFiles})
  INSTALL_FILES(/include/InsightToolkit/BasicFilters FILES ${f})
ENDFOREACH(f)
ENDIF(INSTALL_DEVEL_FILES)



# option for wrapping
OPTION(BUILD_WRAPPERS "Wrap library" OFF)
IF(BUILD_WRAPPERS)
   SUBDIRS(Wrapping)
ENDIF(BUILD_WRAPPERS)
   
   

#the following block of code is an example of how to build an executable in
#cmake.  Unmodified, it will add an executable called "MyExe" to the project.
#MyExe will be built using the files MyClass.h and MyClass.cxx, and it will
#be linked to all the libraries you specified above. 
#You can build more than one executable per project

IF(BUILD_TESTING)

# SET(CurrentExe "check")
# ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
# TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "binary_shape_opening")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "binary_shape_keep_n_objects")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "label_shape_opening")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "label_shape_keep_n_objects")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "shape_relabel")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})


SET(CurrentExe "binary_statistics_opening")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "binary_statistics_keep_n_objects")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "label_statistics_opening")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "label_statistics_keep_n_objects")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "statistics_relabel")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "mask")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "binary_reconstruction_by_dilation")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "binary_reconstruction_by_erosion")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "binary_closing_by_reconstruction")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "binary_opening_by_reconstruction")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})

SET(CurrentExe "binary_fillhole")
ADD_EXECUTABLE(${CurrentExe} ${CurrentExe}.cxx)
TARGET_LINK_LIBRARIES(${CurrentExe} ${Libraries})


ENDIF(BUILD_TESTING)

#the following line is an example of how to add a test to your project.
#Testname is the title for this particular test.  ExecutableToRun is the
#program which will be running this test.  It can either be a part of this
#project or an external executable.  After that list any args that are needed
#for this test.  Include as many tests as you like.  If your project doesn't have
#any tests you can comment out or delete the following line.
# ADD_TEST(Testname ExecutableToRun arg1 arg2 arg3)

FOREACH(fg 100 200)
  FOREACH(bg 0 150)
    FOREACH(order 0 1)
      FOREACH(connectivity 0 1)
        FOREACH(size 100 10000)
          ADD_TEST(BinarySizeOpening-${fg}-${bg}-${size}-${order}-${connectivity} ${TEST_COMMAND}
            binary_shape_opening
              ${BINARY_IMAGE} binary_size_opening-${fg}-${bg}-${size}-${order}-${connectivity}.png
              ${fg} ${bg} ${size} ${order} ${connectivity} Size
            --compare binary_size_opening-${fg}-${bg}-${size}-${order}-${connectivity}.png ${CMAKE_SOURCE_DIR}/images/binary_size_opening-${fg}-${bg}-${size}-${order}-${connectivity}.png
          )

          ADD_TEST(BinaryPhysicalSizeOpening-${fg}-${bg}-${size}-${order}-${connectivity} ${TEST_COMMAND}
            binary_shape_opening
              ${BINARY_IMAGE} binary_physical_size_opening-${fg}-${bg}-${size}-${order}-${connectivity}.png
              ${fg} ${bg} ${size} ${order} ${connectivity} PhysicalSize
            --compare binary_physical_size_opening-${fg}-${bg}-${size}-${order}-${connectivity}.png ${CMAKE_SOURCE_DIR}/images/binary_physical_size_opening-${fg}-${bg}-${size}-${order}-${connectivity}.png
          )
        ENDFOREACH(size)

        FOREACH(nb 3 10)
          ADD_TEST(BinarySizeKeepNObjects-${fg}-${bg}-${nb}-${order}-${connectivity} ${TEST_COMMAND}
            binary_shape_keep_n_objects
              ${BINARY_IMAGE} binary_size_keep_n_objects-${fg}-${bg}-${nb}-${order}-${connectivity}.png
              ${fg} ${bg} ${nb} ${order} ${connectivity} Size
            --compare binary_size_keep_n_objects-${fg}-${bg}-${nb}-${order}-${connectivity}.png ${CMAKE_SOURCE_DIR}/images/binary_size_keep_n_objects-${fg}-${bg}-${nb}-${order}-${connectivity}.png
          )
        ENDFOREACH(nb)
      ENDFOREACH(connectivity)
    ENDFOREACH(order)
  ENDFOREACH(bg)
ENDFOREACH(fg)

FOREACH(bg 0 176)
  FOREACH(order 0 1)
      FOREACH(size 3000 10000)
        ADD_TEST(LabelSizeOpening-${bg}-${size}-${order} ${TEST_COMMAND}
          label_shape_opening
            ${LABEL_IMAGE} label_size_opening-${bg}-${size}-${order}.png
            ${bg} ${size} ${order} Size
          --compare label_size_opening-${bg}-${size}-${order}.png ${CMAKE_SOURCE_DIR}/images/label_size_opening-${bg}-${size}-${order}.png
        )

        ADD_TEST(LabelPhysicalSizeOpening-${bg}-${size}-${order} ${TEST_COMMAND}
          label_shape_opening
            ${LABEL_IMAGE} label_physical_size_opening-${bg}-${size}-${order}.png
            ${bg} ${size} ${order} Size
          --compare label_physical_size_opening-${bg}-${size}-${order}.png ${CMAKE_SOURCE_DIR}/images/label_physical_size_opening-${bg}-${size}-${order}.png
        )
      ENDFOREACH(size)

      FOREACH(nb 1 3)
        ADD_TEST(LabelSizeKeepNObjects-${bg}-${nb}-${order} ${TEST_COMMAND}
          label_shape_keep_n_objects
            ${LABEL_IMAGE} label_size_keep_n_objects-${bg}-${nb}-${order}.png
            ${bg} ${nb} ${order} Size
          --compare label_size_keep_n_objects-${bg}-${nb}-${order}.png ${CMAKE_SOURCE_DIR}/images/label_size_keep_n_objects-${bg}-${nb}-${order}.png
        )
      ENDFOREACH(nb)

      FOREACH(useBg 0 1)
        ADD_TEST(SizeRelabel-${bg}-${useBg}-${order} ${TEST_COMMAND}
          shape_relabel
            ${LABEL_IMAGE} size_relabel-${bg}-${useBg}-${order}.png
            ${bg} ${useBg} ${order} Size
          --compare size_relabel-${bg}-${useBg}-${order}.png ${CMAKE_SOURCE_DIR}/images/size_relabel-${bg}-${useBg}-${order}.png
        )
      ENDFOREACH(useBg)
  ENDFOREACH(order)
ENDFOREACH(bg)
