/**
 * @file calibration.h
 * @brief Calibration models, environmental cross-sensitivity compensation, and NVS storage.
 */

#ifndef CALIBRATION_H
#define CALIBRATION_H

#include "hal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float o2_v0;
    float o2_v_air;
    float h2s_v0;
    float h2s_mv_ppm;
    float co_v0;
    float co_mv_ppm;
    float ch4_v0;
    float ch4_mv_lel;
    float water_manhole_depth_cm;
    uint32_t calibration_timestamp;
} device_calibration_profile_t;

esp_err_t calibration_init(void);
esp_err_t calibration_get_profile(device_calibration_profile_t *profile);
esp_err_t calibration_set_profile(const device_calibration_profile_t *profile);

/**
 * @brief Temperature compensation for electrochemical gas sensors.
 * Electrolyte diffusion coefficients vary with temperature (~0.2% per deg C).
 */
float calibration_apply_temp_comp(float gas_raw, float temp_c, float reference_temp_c, float temp_coeff);

#ifdef __cplusplus
}
#endif

#endif /* CALIBRATION_H */
