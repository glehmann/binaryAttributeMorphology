WRAP_CLASS("itk::LabelCollectionImageFilter" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    # label collection -> label collection
    WRAP_TEMPLATE("${ITKM_LI${d}}${ITKM_LI${d}}" "${ITKT_LI${d}}, ${ITKT_LI${d}}")

    FOREACH(t ${WRAP_ITK_SCALAR})
      # label collection -> image
      WRAP_TEMPLATE("${ITKM_LI${d}}${ITKM_I${t}${d}}" "${ITKT_LI${d}}, ${ITKT_I${t}${d}}")
    ENDFOREACH(t)
  ENDFOREACH(d)
END_WRAP_CLASS()
