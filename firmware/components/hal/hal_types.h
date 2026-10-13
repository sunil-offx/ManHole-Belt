/**
 * @file hal_types.h
 * @brief Hardware Abstraction Layer common types and definitions for SENTINEL-BELT.
 * Supports both ESP-IDF (ESP32-S3) and native host targets for unit testing.
 */

#ifndef HAL_TYPES_H
#define HAL_TYPES_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef ESP_PLATFORM
#include "esp_err.h"
#include "esp_log.h"
#else
/* Host simulation error types */
typedef int32_t esp_err_t;
#define ESP_OK          0
#define ESP_FAIL        -1
#define ESP_ERR_NO_MEM  0x101
#define ESP_ERR_INVALID_ARG 0x102
#define ESP_ERR_INVALID_STATE 0x103
#define ESP_ERR_TIMEOUT 0x107

#define ESP_LOGI(tag, fmt, ...) do {} while(0)
#define ESP_LOGW(tag, fmt, ...) do {} while(0)
#define ESP_LOGE(tag, fmt, ...) do {} while(0)
#endif

/* GPIO logic levels */
typedef enum {
    HAL_GPIO_LOW = 0,
    HAL_GPIO_HIGH = 1
} hal_gpio_level_t;

/* I2C Port IDs */
typedef enum {
    HAL_I2C_PORT_0 = 0,
    HAL_I2C_PORT_1 = 1
} hal_i2c_port_t;

/* ADC Channels */
typedef enum {
    HAL_ADC_CH_O2 = 0,
    HAL_ADC_CH_H2S = 1,
    HAL_ADC_CH_CO = 2,
    HAL_ADC_CH_CH4 = 3,
    HAL_ADC_CH_BATTERY = 4
} hal_adc_channel_t;

#endif /* HAL_TYPES_H */
