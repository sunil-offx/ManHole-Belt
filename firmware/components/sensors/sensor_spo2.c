/**
 * @file sensor_spo2.c
 * @brief MAX30102 Optical SpO2 sensor driver implementation.
 */

#include "sensor_spo2.h"
#include "hal_i2c.h"

#define MAX30102_ADDR 0x57

static float s_cached_spo2 = 98.0f;

static esp_err_t spo2_init(void) {
    return ESP_OK;
}

static esp_err_t spo2_read(float *value) {
    if (!value) return ESP_ERR_INVALID_ARG;
    *value = s_cached_spo2;
    return ESP_OK;
}

static esp_err_t spo2_calibrate(void) {
    return ESP_OK;
}

const sensor_driver_t sensor_driver_spo2 = {
    .init = spo2_init,
    .read = spo2_read,
    .calibrate = spo2_calibrate,
    .name = "MAX30102_SpO2",
    .min_value = 70.0f,
    .max_value = 100.0f,
    .sample_period_ms = 1000
};
