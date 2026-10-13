idf_component_register(
    SRCS "alert_manager.c" "oled_display.c"
    INCLUDE_DIRS "."
    REQUIRES hal sensors risk_engine
)
