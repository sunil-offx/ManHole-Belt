/**
 * @file hal_i2c.h
 * @brief I2C Hardware Abstraction Layer.
 */

#ifndef HAL_I2C_H
#define HAL_I2C_H

#include "hal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define PIN_I2C_SDA 8
#define PIN_I2C_SCL 9

esp_err_t hal_i2c_init(hal_i2c_port_t port, int sda_pin, int scl_pin, uint32_t clk_speed_hz);
esp_err_t hal_i2c_write(hal_i2c_port_t port, uint8_t dev_addr, const uint8_t *data, size_t len);
esp_err_t hal_i2c_read(hal_i2c_port_t port, uint8_t dev_addr, uint8_t *data, size_t len);
esp_err_t hal_i2c_write_read(hal_i2c_port_t port, uint8_t dev_addr, 
                             const uint8_t *w_data, size_t w_len, 
                             uint8_t *r_data, size_t r_len);

#ifdef __cplusplus
}
#endif

#endif /* HAL_I2C_H */
