WRAP_CLASS("itk::ImageSource" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    WRAP_TEMPLATE("${ITKM_LI${d}}" "${ITKT_LI${d}}")
  ENDFOREACH(d)
END_WRAP_CLASS()

WRAP_CLASS("itk::ImageToImageFilter" POINTER)
  FOREACH(d ${WRAP_ITK_DIMS})
    # label collection -> label collection
    WRAP_TEMPLATE("${ITKM_LI${d}}${ITKM_LI${d}}" "${ITKT_LI${d}}, ${ITKT_LI${d}}")

    FOREACH(t ${WRAP_ITK_USIGN_INT})
      # image -> label collection image
      WRAP_TEMPLATE("${ITKM_I${t}${d}}${ITKM_LI${d}}" "${ITKT_I${t}${d}}, ${ITKT_LI${d}}")
      # label collection -> image
      WRAP_TEMPLATE("${ITKM_LI${d}}${ITKM_I${t}${d}}" "${ITKT_LI${d}}, ${ITKT_I${t}${d}}")
    ENDFOREACH(t)
  ENDFOREACH(d)
END_WRAP_CLASS()
