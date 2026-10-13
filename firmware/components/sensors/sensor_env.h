/**
 * @file sensor_env.h
 * @brief Environmental temperature and humidity sensor driver (SHT31 / digital I2C).
 */

#ifndef SENSOR_ENV_H
#define SENSOR_ENV_H

#include "sensor_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const sensor_driver_t sensor_driver_temp;
extern const sensor_driver_t sensor_driver_humidity;

esp_err_t sensor_env_read_both(float *temp_c, float *hum_pct);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_ENV_H */
