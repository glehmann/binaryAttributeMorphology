WRAP_CLASS("itk::XXXKeepNObjectsLabelCollectionImageFilter" POINTER_WITH_SUPERCLASS)
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("${ITKM_LI${d}}" "${ITKT_LI${d}}")
  ENDFOREACH(d)
END_WRAP_CLASS()