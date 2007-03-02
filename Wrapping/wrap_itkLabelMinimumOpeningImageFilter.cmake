WRAP_CLASS("itk::LabelMinimumOpeningImageFilter" POINTER_WITH_SUPERCLASS)
  FOREACH(d ${WRAP_ITK_DIMS})
    FOREACH(t2 ${WRAP_ITK_SCALAR})
      FOREACH(t1 ${WRAP_ITK_USIGN_INT})
        WRAP_TEMPLATE("${ITKM_I${t1}${d}}${ITKM_I${t2}${d}}"  "${ITKT_I${t1}${d}},${ITKT_I${t2}${d}}")
      ENDFOREACH(t1)
    ENDFOREACH(t2)
  ENDFOREACH(d)
END_WRAP_CLASS()
