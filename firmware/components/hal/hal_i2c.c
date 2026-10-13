/**
 * @file hal_i2c.c
 * @brief I2C Hardware Abstraction implementation.
 */

#include "hal_i2c.h"
#include <string.h>

#ifdef ESP_PLATFORM
#include "driver/i2c.h"

esp_err_t hal_i2c_init(hal_i2c_port_t port, int sda_pin, int scl_pin, uint32_t clk_speed_hz) {
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = sda_pin,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_io_num = scl_pin,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = clk_speed_hz,
    };
    esp_err_t err = i2c_param_config((i2c_port_t)port, &conf);
    if (err != ESP_OK) return err;
    return i2c_driver_install((i2c_port_t)port, conf.mode, 0, 0, 0);
}

esp_err_t hal_i2c_write(hal_i2c_port_t port, uint8_t dev_addr, const uint8_t *data, size_t len) {
    return i2c_master_write_to_device((i2c_port_t)port, dev_addr, data, len, 1000 / portTICK_PERIOD_MS);
}

esp_err_t hal_i2c_read(hal_i2c_port_t port, uint8_t dev_addr, uint8_t *data, size_t len) {
    return i2c_master_read_from_device((i2c_port_t)port, dev_addr, data, len, 1000 / portTICK_PERIOD_MS);
}

esp_err_t hal_i2c_write_read(hal_i2c_port_t port, uint8_t dev_addr, 
                             const uint8_t *w_data, size_t w_len, 
                             uint8_t *r_data, size_t r_len) {
    return i2c_master_write_read_device((i2c_port_t)port, dev_addr, w_data, w_len, r_data, r_len, 1000 / portTICK_PERIOD_MS);
}

#else
/* Native host mock implementation */
esp_err_t hal_i2c_init(hal_i2c_port_t port, int sda_pin, int scl_pin, uint32_t clk_speed_hz) {
    (void)port; (void)sda_pin; (void)scl_pin; (void)clk_speed_hz;
    return ESP_OK;
}

esp_err_t hal_i2c_write(hal_i2c_port_t port, uint8_t dev_addr, const uint8_t *data, size_t len) {
    (void)port; (void)dev_addr; (void)data; (void)len;
    return ESP_OK;
}

esp_err_t hal_i2c_read(hal_i2c_port_t port, uint8_t dev_addr, uint8_t *data, size_t len) {
    (void)port; (void)dev_addr;
    if (data && len > 0) {
        memset(data, 0, len);
    }
    return ESP_OK;
}

esp_err_t hal_i2c_write_read(hal_i2c_port_t port, uint8_t dev_addr, 
                             const uint8_t *w_data, size_t w_len, 
                             uint8_t *r_data, size_t r_len) {
    (void)port; (void)dev_addr; (void)w_data; (void)w_len;
    if (r_data && r_len > 0) {
        memset(r_data, 0, r_len);
    }
    return ESP_OK;
}
#endif
