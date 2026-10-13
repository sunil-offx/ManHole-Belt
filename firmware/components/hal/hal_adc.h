/**
 * @file hal_adc.h
 * @brief ADC Hardware Abstraction Layer for analog gas sensors and battery monitor.
 */

#ifndef HAL_ADC_H
#define HAL_ADC_H

#include "hal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t hal_adc_init(void);
esp_err_t hal_adc_read_voltage(hal_adc_channel_t channel, float *voltage_volts);
esp_err_t hal_adc_read_raw(hal_adc_channel_t channel, int *raw_value);

#ifdef __cplusplus
}
#endif

#endif /* HAL_ADC_H */
