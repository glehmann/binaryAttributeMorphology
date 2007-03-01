WRAP_CLASS("itk::LabelImageToLabelCollectionImageFilter" POINTER)
  FOREACH(t ${WRAP_ITK_USIGN_INT})
    FOREACH(d ${WRAP_ITK_DIMS})
      # image -> label collection image
      WRAP_TEMPLATE("${ITKM_I${t}${d}}${ITKM_LI${d}}" "${ITKT_I${t}${d}}, ${ITKT_LI${d}}")
    ENDFOREACH(d)
  ENDFOREACH(t)
END_WRAP_CLASS()
