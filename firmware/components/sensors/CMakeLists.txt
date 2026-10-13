idf_component_register(
    SRCS "sensor_o2.c" "sensor_toxic_gas.c" "sensor_env.c" "sensor_imu.c" "sensor_water.c" "sensor_spo2.c" "sensor_manager.c"
    INCLUDE_DIRS "."
    REQUIRES hal
)
