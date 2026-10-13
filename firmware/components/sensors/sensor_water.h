/**
 * @file sensor_water.h
 * @brief Ultrasonic water level sensor driver (HC-SR04 / JSN-SR04T).
 */

#ifndef SENSOR_WATER_H
#define SENSOR_WATER_H

#include "sensor_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const sensor_driver_t sensor_driver_water;

void sensor_water_set_total_depth(float depth_cm);
float sensor_water_get_total_depth(void);
void sensor_water_set_ambient_temp(float temp_c);
esp_err_t sensor_water_measure_distance_cm(float *distance_cm);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_WATER_H */
