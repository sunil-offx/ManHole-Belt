/**
 * @file sensor_water.c
 * @brief Ultrasonic water level sensor driver implementation.
 * Distance d = (v * t) / 2
 * Water level = D - d
 * Water % = (WaterLevel / D) * 100
 */

#include "sensor_water.h"
#include "hal_gpio.h"
#include "hal_timer.h"

static float s_total_depth_cm = 200.0f; /* 2.0 meters standard manhole depth */
static float s_ambient_temp_c = 25.0f;
static float s_mock_water_pct = 5.0f;

static esp_err_t water_init(void) {
    hal_gpio_init_output(PIN_ULTRASONIC_TRIG);
    hal_gpio_init_input(PIN_ULTRASONIC_ECHO, false);
    hal_gpio_set_level(PIN_ULTRASONIC_TRIG, HAL_GPIO_LOW);
    return ESP_OK;
}

esp_err_t sensor_water_measure_distance_cm(float *distance_cm) {
    if (!distance_cm) return ESP_ERR_INVALID_ARG;

#ifdef ESP_PLATFORM
    /* Trigger 10us pulse */
    hal_gpio_set_level(PIN_ULTRASONIC_TRIG, HAL_GPIO_LOW);
    hal_delay_us(2);
    hal_gpio_set_level(PIN_ULTRASONIC_TRIG, HAL_GPIO_HIGH);
    hal_delay_us(10);
    hal_gpio_set_level(PIN_ULTRASONIC_TRIG, HAL_GPIO_LOW);

    /* Measure echo pulse width */
    uint64_t start_us = hal_get_time_us();
    uint64_t timeout_us = start_us + 30000; /* ~5 meters max timeout */
    
    while (hal_gpio_get_level(PIN_ULTRASONIC_ECHO) == HAL_GPIO_LOW) {
        if (hal_get_time_us() > timeout_us) return ESP_ERR_TIMEOUT;
    }
    
    uint64_t echo_start = hal_get_time_us();
    timeout_us = echo_start + 30000;
    while (hal_gpio_get_level(PIN_ULTRASONIC_ECHO) == HAL_GPIO_HIGH) {
        if (hal_get_time_us() > timeout_us) return ESP_ERR_TIMEOUT;
    }
    uint64_t echo_end = hal_get_time_us();
    
    float pulse_time_s = (float)(echo_end - echo_start) / 1000000.0f;
    /* Temperature compensation for speed of sound in air (m/s) */
    float speed_of_sound_m_s = 331.3f + (0.606f * s_ambient_temp_c);
    float dist_meters = (speed_of_sound_m_s * pulse_time_s) / 2.0f;
    *distance_cm = dist_meters * 100.0f;
    return ESP_OK;
#else
    /* Mock implementation for host simulation */
    /* If water is mock_pct, distance is total_depth * (1 - mock_pct/100) */
    *distance_cm = s_total_depth_cm * (1.0f - (s_mock_water_pct / 100.0f));
    return ESP_OK;
#endif
}

static esp_err_t water_read(float *value) {
    if (!value) return ESP_ERR_INVALID_ARG;
    float dist_cm = 0.0f;
    esp_err_t err = sensor_water_measure_distance_cm(&dist_cm);
    if (err != ESP_OK) return err;

    float water_level = s_total_depth_cm - dist_cm;
    if (water_level < 0.0f) water_level = 0.0f;
    if (water_level > s_total_depth_cm) water_level = s_total_depth_cm;

    float pct = (water_level / s_total_depth_cm) * 100.0f;
    *value = pct;
    return ESP_OK;
}

static esp_err_t water_calibrate(void) {
    /* Set current distance as empty manhole depth (0% water baseline) */
    float dist_cm = 0.0f;
    esp_err_t err = sensor_water_measure_distance_cm(&dist_cm);
    if (err == ESP_OK && dist_cm > 20.0f) {
        s_total_depth_cm = dist_cm;
    }
    return err;
}

void sensor_water_set_total_depth(float depth_cm) {
    if (depth_cm > 10.0f) s_total_depth_cm = depth_cm;
}

float sensor_water_get_total_depth(void) {
    return s_total_depth_cm;
}

void sensor_water_set_ambient_temp(float temp_c) {
    s_ambient_temp_c = temp_c;
}

#ifndef ESP_PLATFORM
void sensor_water_mock_set_level_pct(float pct) {
    s_mock_water_pct = pct;
}
#endif

const sensor_driver_t sensor_driver_water = {
    .init = water_init,
    .read = water_read,
    .calibrate = water_calibrate,
    .name = "Ultrasonic_Water_Level",
    .min_value = 0.0f,
    .max_value = 100.0f,
    .sample_period_ms = 1000
};
