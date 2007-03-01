WRAP_INCLUDE("itkShapeLabelObject.h")
WRAP_CLASS("itk::LabelCollectionImage" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("SLO${ITKM_UL}${d}" "itk::ShapeLabelObject< ${ITKT_UL}, ${d} >")
  ENDFOREACH(d)
END_WRAP_CLASS()