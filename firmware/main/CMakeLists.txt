idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES 
        nvs_flash 
        esp_timer 
        esp_wifi 
        mqtt 
        hal 
        sensors 
        processing 
        risk_engine 
        motion 
        ai_inference 
        alerts 
        comms 
        power
)
