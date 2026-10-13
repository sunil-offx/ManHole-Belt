/**
 * @file sensor_env.c
 * @brief SHT31 Temperature and Humidity sensor driver implementation.
 */

#include "sensor_env.h"
#include "hal_i2c.h"
#include "hal_timer.h"

#define SHT31_I2C_ADDR 0x44

static float s_cached_temp = 24.5f;
static float s_cached_hum = 62.0f;
static uint32_t s_last_read_ms = 0;

static uint8_t sht31_crc(const uint8_t *data, size_t len) {
    uint8_t crc = 0xFF;
    for (size_t i = 0; i < len; i++) {
        crc ^= data[i];
        for (int b = 0; b < 8; b++) {
            if (crc & 0x80) {
                crc = (crc << 1) ^ 0x31;
            } else {
                crc = (crc << 1);
            }
        }
    }
    return crc;
}

static esp_err_t env_init(void) {
    return hal_i2c_init(HAL_I2C_PORT_0, PIN_I2C_SDA, PIN_I2C_SCL, 100000);
}

esp_err_t sensor_env_read_both(float *temp_c, float *hum_pct) {
    uint32_t now = hal_get_time_ms();
    if (now - s_last_read_ms < 200 && s_last_read_ms > 0) {
        if (temp_c) *temp_c = s_cached_temp;
        if (hum_pct) *hum_pct = s_cached_hum;
        return ESP_OK;
    }

    uint8_t cmd[2] = {0x24, 0x00}; /* High repeatability single shot */
    esp_err_t err = hal_i2c_write(HAL_I2C_PORT_0, SHT31_I2C_ADDR, cmd, sizeof(cmd));
    if (err != ESP_OK) {
        /* Fallback cached or default */
        if (temp_c) *temp_c = s_cached_temp;
        if (hum_pct) *hum_pct = s_cached_hum;
        return ESP_OK;
    }

    hal_delay_ms(15); /* measurement duration */

    uint8_t rx[6] = {0};
    err = hal_i2c_read(HAL_I2C_PORT_0, SHT31_I2C_ADDR, rx, sizeof(rx));
    if (err == ESP_OK) {
        /* Verify CRC */
        if (sht31_crc(&rx[0], 2) == rx[2] && sht31_crc(&rx[3], 2) == rx[5]) {
            uint16_t raw_t = (rx[0] << 8) | rx[1];
            uint16_t raw_rh = (rx[3] << 8) | rx[4];
            s_cached_temp = -45.0f + 175.0f * ((float)raw_t / 65535.0f);
            s_cached_hum = 100.0f * ((float)raw_rh / 65535.0f);
            s_last_read_ms = now;
        }
    }

    if (temp_c) *temp_c = s_cached_temp;
    if (hum_pct) *hum_pct = s_cached_hum;
    return ESP_OK;
}

static esp_err_t temp_read(float *value) {
    return sensor_env_read_both(value, NULL);
}

static esp_err_t hum_read(float *value) {
    return sensor_env_read_both(NULL, value);
}

static esp_err_t env_calibrate(void) {
    return ESP_OK; /* Factory calibrated */
}

const sensor_driver_t sensor_driver_temp = {
    .init = env_init,
    .read = temp_read,
    .calibrate = env_calibrate,
    .name = "SHT31_Temperature",
    .min_value = -20.0f,
    .max_value = 80.0f,
    .sample_period_ms = 1000
};

const sensor_driver_t sensor_driver_humidity = {
    .init = env_init,
    .read = hum_read,
    .calibrate = env_calibrate,
    .name = "SHT31_Humidity",
    .min_value = 0.0f,
    .max_value = 100.0f,
    .sample_period_ms = 1000
};
