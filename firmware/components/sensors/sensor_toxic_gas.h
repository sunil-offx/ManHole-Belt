/**
 * @file sensor_toxic_gas.h
 * @brief Toxic & combustible gas drivers (H2S, CO, CH4).
 */

#ifndef SENSOR_TOXIC_GAS_H
#define SENSOR_TOXIC_GAS_H

#include "sensor_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const sensor_driver_t sensor_driver_h2s;
extern const sensor_driver_t sensor_driver_co;
extern const sensor_driver_t sensor_driver_ch4;

void sensor_h2s_set_sensitivity(float mv_per_ppm, float zero_v);
void sensor_co_set_sensitivity(float mv_per_ppm, float zero_v);
void sensor_ch4_set_sensitivity(float mv_per_lel_pct, float zero_v);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_TOXIC_GAS_H */
