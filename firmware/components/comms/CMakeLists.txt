idf_component_register(
    SRCS "comm_manager.c" "telemetry_protocol.c"
    INCLUDE_DIRS "."
    REQUIRES hal sensors risk_engine mqtt esp_wifi
)
