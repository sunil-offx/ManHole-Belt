/**
 * @file sensor_toxic_gas.c
 * @brief Implementations for H2S, CO, and CH4 gas sensors.
 */

#include "sensor_toxic_gas.h"
#include "hal_adc.h"

/* H2S: typically 100 ppm full scale; sensitivity e.g., 25 mV/ppm */
static float s_h2s_v0 = 0.05f;
static float s_h2s_gain = 0.025f; /* V per ppm */

/* CO: typically 500 ppm full scale; sensitivity e.g., 5 mV/ppm */
static float s_co_v0 = 0.10f;
static float s_co_gain = 0.005f; /* V per ppm */

/* CH4: 0 to 100% LEL; sensitivity e.g., 25 mV per % LEL */
static float s_ch4_v0 = 0.02f;
static float s_ch4_gain = 0.025f; /* V per % LEL */

static esp_err_t h2s_init(void) { return ESP_OK; }
static esp_err_t co_init(void)  { return ESP_OK; }
static esp_err_t ch4_init(void) { return ESP_OK; }

static esp_err_t h2s_read(float *value) {
    if (!value) return ESP_ERR_INVALID_ARG;
    float v = 0.0f;
    esp_err_t err = hal_adc_read_voltage(HAL_ADC_CH_H2S, &v);
    if (err != ESP_OK) return err;

    float ppm = (v - s_h2s_v0) / s_h2s_gain;
    if (ppm < 0.0f) ppm = 0.0f;
    if (ppm > 200.0f) ppm = 200.0f;
    *value = ppm;
    return ESP_OK;
}

static esp_err_t h2s_calibrate(void) {
    /* Zero calibrate in fresh air */
    float v = 0.0f;
    esp_err_t err = hal_adc_read_voltage(HAL_ADC_CH_H2S, &v);
    if (err == ESP_OK) s_h2s_v0 = v;
    return err;
}

static esp_err_t co_read(float *value) {
    if (!value) return ESP_ERR_INVALID_ARG;
    float v = 0.0f;
    esp_err_t err = hal_adc_read_voltage(HAL_ADC_CH_CO, &v);
    if (err != ESP_OK) return err;

    float ppm = (v - s_co_v0) / s_co_gain;
    if (ppm < 0.0f) ppm = 0.0f;
    if (ppm > 1000.0f) ppm = 1000.0f;
    *value = ppm;
    return ESP_OK;
}

static esp_err_t co_calibrate(void) {
    float v = 0.0f;
    esp_err_t err = hal_adc_read_voltage(HAL_ADC_CH_CO, &v);
    if (err == ESP_OK) s_co_v0 = v;
    return err;
}

static esp_err_t ch4_read(float *value) {
    if (!value) return ESP_ERR_INVALID_ARG;
    float v = 0.0f;
    esp_err_t err = hal_adc_read_voltage(HAL_ADC_CH_CH4, &v);
    if (err != ESP_OK) return err;

    float lel_pct = (v - s_ch4_v0) / s_ch4_gain;
    if (lel_pct < 0.0f) lel_pct = 0.0f;
    if (lel_pct > 100.0f) lel_pct = 100.0f;
    *value = lel_pct;
    return ESP_OK;
}

static esp_err_t ch4_calibrate(void) {
    float v = 0.0f;
    esp_err_t err = hal_adc_read_voltage(HAL_ADC_CH_CH4, &v);
    if (err == ESP_OK) s_ch4_v0 = v;
    return err;
}

void sensor_h2s_set_sensitivity(float mv_per_ppm, float zero_v) {
    s_h2s_gain = mv_per_ppm / 1000.0f;
    s_h2s_v0 = zero_v;
}

void sensor_co_set_sensitivity(float mv_per_ppm, float zero_v) {
    s_co_gain = mv_per_ppm / 1000.0f;
    s_co_v0 = zero_v;
}

void sensor_ch4_set_sensitivity(float mv_per_lel_pct, float zero_v) {
    s_ch4_gain = mv_per_lel_pct / 1000.0f;
    s_ch4_v0 = zero_v;
}

const sensor_driver_t sensor_driver_h2s = {
    .init = h2s_init,
    .read = h2s_read,
    .calibrate = h2s_calibrate,
    .name = "Electrochemical_H2S",
    .min_value = 0.0f,
    .max_value = 100.0f,
    .sample_period_ms = 500
};

const sensor_driver_t sensor_driver_co = {
    .init = co_init,
    .read = co_read,
    .calibrate = co_calibrate,
    .name = "Electrochemical_CO",
    .min_value = 0.0f,
    .max_value = 500.0f,
    .sample_period_ms = 500
};

const sensor_driver_t sensor_driver_ch4 = {
    .init = ch4_init,
    .read = ch4_read,
    .calibrate = ch4_calibrate,
    .name = "Combustible_CH4",
    .min_value = 0.0f,
    .max_value = 100.0f,
    .sample_period_ms = 500
};
