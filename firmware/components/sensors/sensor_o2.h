/**
 * @file sensor_o2.h
 * @brief Electrochemical Oxygen (O2) Sensor Driver.
 */

#ifndef SENSOR_O2_H
#define SENSOR_O2_H

#include "sensor_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const sensor_driver_t sensor_driver_o2;

void sensor_o2_set_calibration(float v0, float v_air);
void sensor_o2_get_calibration(float *v0, float *v_air);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_O2_H */
