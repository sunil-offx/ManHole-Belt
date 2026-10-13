idf_component_register(
    SRCS "calibration.c" "filters.c" "feature_extractor.c"
    INCLUDE_DIRS "."
    REQUIRES hal sensors ai_inference
)
