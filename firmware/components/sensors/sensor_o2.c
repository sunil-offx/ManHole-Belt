/**
 * @file sensor_o2.c
 * @brief Electrochemical Oxygen (O2) Sensor Driver implementation.
 * Formula: O2 = 20.9 * (V - V0) / (V_air - V0)
 */

#include "sensor_o2.h"
#include "hal_adc.h"
#include <math.h>

static float s_v0 = 0.05f;       /* Sensor zero-point voltage in zero-gas */
static float s_v_air = 1.65f;    /* Voltage in ambient clean air (20.9% O2) */
static bool s_initialized = false;

static esp_err_t o2_init(void) {
    hal_adc_init();
    s_initialized = true;
    return ESP_OK;
}

static esp_err_t o2_read(float *value) {
    if (!s_initialized || !value) return ESP_ERR_INVALID_STATE;
    
    float voltage = 0.0f;
    esp_err_t err = hal_adc_read_voltage(HAL_ADC_CH_O2, &voltage);
    if (err != ESP_OK) return err;

    if (fabsf(s_v_air - s_v0) < 0.001f) {
        return ESP_ERR_INVALID_STATE;
    }

    /* Linear electrochemical calibration model */
    float o2_pct = 20.9f * (voltage - s_v0) / (s_v_air - s_v0);
    
    /* Hardware sanity check: negative or extreme reading detection */
    if (o2_pct < 0.0f) o2_pct = 0.0f;
    if (o2_pct > 30.0f) o2_pct = 30.0f;

    *value = o2_pct;
    return ESP_OK;
}

static esp_err_t o2_calibrate(void) {
    /* Auto-span calibration in fresh air (assumes worker initiates in 20.9% air) */
    float voltage = 0.0f;
    esp_err_t err = hal_adc_read_voltage(HAL_ADC_CH_O2, &voltage);
    if (err != ESP_OK) return err;

    s_v_air = voltage;
    return ESP_OK;
}

void sensor_o2_set_calibration(float v0, float v_air) {
    s_v0 = v0;
    s_v_air = v_air;
}

void sensor_o2_get_calibration(float *v0, float *v_air) {
    if (v0) *v0 = s_v0;
    if (v_air) *v_air = s_v_air;
}

const sensor_driver_t sensor_driver_o2 = {
    .init = o2_init,
    .read = o2_read,
    .calibrate = o2_calibrate,
    .name = "Electrochemical_O2",
    .min_value = 0.0f,
    .max_value = 25.0f,
    .sample_period_ms = 500  /* Poll gas sensors every 500ms to 1s */
};
