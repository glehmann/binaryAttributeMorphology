WRAP_CLASS("itk::LabelCollectionImageToBinaryImageFilter" POINTER)
  FOREACH(t ${WRAP_ITK_USIGN_INT})
    FOREACH(d ${WRAP_ITK_DIMS})
      WRAP_TEMPLATE("${ITKM_LI${d}}${ITKM_I${t}${d}}" "${ITKT_LI${d}}, ${ITKT_I${t}${d}}")
    ENDFOREACH(d)
  ENDFOREACH(t)
END_WRAP_CLASS()