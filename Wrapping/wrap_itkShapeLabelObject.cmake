WRAP_CLASS("itk::ShapeLabelObject" POINTER_WITH_SUPERCLASS)
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("${ITKM_UL}${d}" "${ITKT_UL},${d}")
  ENDFOREACH(d)
END_WRAP_CLASS()