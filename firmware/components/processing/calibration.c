/**
 * @file calibration.c
 * @brief Calibration models implementation.
 */

#include "calibration.h"
#include "sensor_o2.h"
#include "sensor_toxic_gas.h"
#include "sensor_water.h"
#include <string.h>

static device_calibration_profile_t s_cal_profile = {
    .o2_v0 = 0.05f,
    .o2_v_air = 1.65f,
    .h2s_v0 = 0.05f,
    .h2s_mv_ppm = 25.0f,
    .co_v0 = 0.10f,
    .co_mv_ppm = 5.0f,
    .ch4_v0 = 0.02f,
    .ch4_mv_lel = 25.0f,
    .water_manhole_depth_cm = 200.0f,
    .calibration_timestamp = 1718000000
};

esp_err_t calibration_init(void) {
    sensor_o2_set_calibration(s_cal_profile.o2_v0, s_cal_profile.o2_v_air);
    sensor_h2s_set_sensitivity(s_cal_profile.h2s_mv_ppm, s_cal_profile.h2s_v0);
    sensor_co_set_sensitivity(s_cal_profile.co_mv_ppm, s_cal_profile.co_v0);
    sensor_ch4_set_sensitivity(s_cal_profile.ch4_mv_lel, s_cal_profile.ch4_v0);
    sensor_water_set_total_depth(s_cal_profile.water_manhole_depth_cm);
    return ESP_OK;
}

esp_err_t calibration_get_profile(device_calibration_profile_t *profile) {
    if (!profile) return ESP_ERR_INVALID_ARG;
    *profile = s_cal_profile;
    return ESP_OK;
}

esp_err_t calibration_set_profile(const device_calibration_profile_t *profile) {
    if (!profile) return ESP_ERR_INVALID_ARG;
    s_cal_profile = *profile;
    return calibration_init();
}

float calibration_apply_temp_comp(float gas_raw, float temp_c, float reference_temp_c, float temp_coeff) {
    /* Linear temperature compensation formula */
    float delta_t = temp_c - reference_temp_c;
    float factor = 1.0f + (temp_coeff * delta_t);
    if (factor < 0.1f) factor = 0.1f;
    return gas_raw / factor;
}
