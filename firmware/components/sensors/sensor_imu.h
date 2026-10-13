/**
 * @file sensor_imu.h
 * @brief IMU 6-axis accelerometer driver (MPU6050 / BMA400).
 */

#ifndef SENSOR_IMU_H
#define SENSOR_IMU_H

#include "sensor_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const sensor_driver_t sensor_driver_imu_ax;
extern const sensor_driver_t sensor_driver_imu_ay;
extern const sensor_driver_t sensor_driver_imu_az;

esp_err_t sensor_imu_read_accel(float *ax, float *ay, float *az);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_IMU_H */
