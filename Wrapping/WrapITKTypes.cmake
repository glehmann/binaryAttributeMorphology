
# define a new type

WRAP_TYPE("itk::LabelCollectionImage" "LI")
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("${d}" "itk::StatisticsLabelObject< ${ITKT_UL}, ${d} >")
  ENDFOREACH(d)
END_WRAP_TYPE()
SET(itk_Wrap_LabelCollectionImage ${WRAPPER_TEMPLATES})

#add corresponding header in default files

SET(WRAPPER_DEFAULT_INCLUDE
  ${WRAPPER_DEFAULT_INCLUDE}
  "itkStatisticsLabelObject.h"
  "itkLabelCollectionImage.h"
)
