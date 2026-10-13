idf_component_register(
    SRCS "hal_gpio.c" "hal_i2c.c" "hal_adc.c" "hal_timer.c"
    INCLUDE_DIRS "."
    REQUIRES driver esp_timer esp_adc
)
