WRAP_CLASS("itk::ImageSource" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("${ITKM_LM${d}}" "${ITKT_LM${d}}")
  ENDFOREACH(d)
END_WRAP_CLASS()

WRAP_CLASS("itk::ImageToImageFilter" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    # label collection -> label collection
    WRAP_TEMPLATE("${ITKM_LM${d}}${ITKM_LM${d}}" "${ITKT_LM${d}}, ${ITKT_LM${d}}")

    FOREACH(t ${WRAP_ITK_SCALAR})
      # image -> label collection image
      WRAP_TEMPLATE("${ITKM_I${t}${d}}${ITKM_LM${d}}" "${ITKT_I${t}${d}}, ${ITKT_LM${d}}")
      # label collection -> image
      WRAP_TEMPLATE("${ITKM_LM${d}}${ITKM_I${t}${d}}" "${ITKT_LM${d}}, ${ITKT_I${t}${d}}")
    ENDFOREACH(t)
  ENDFOREACH(d)
END_WRAP_CLASS()
