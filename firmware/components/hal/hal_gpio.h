/**
 * @file hal_gpio.h
 * @brief GPIO Hardware Abstraction Layer.
 */

#ifndef HAL_GPIO_H
#define HAL_GPIO_H

#include "hal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Standard Sentinel-Belt GPIO Pin Map */
#define PIN_BUZZER_PWM     18
#define PIN_VIBRATION      19
#define PIN_LED_RED        21
#define PIN_LED_GREEN      22
#define PIN_LED_BLUE       23
#define PIN_ULTRASONIC_TRIG 4
#define PIN_ULTRASONIC_ECHO 5
#define PIN_SOS_BUTTON     0

esp_err_t hal_gpio_init_output(int pin);
esp_err_t hal_gpio_init_input(int pin, bool pull_up);
esp_err_t hal_gpio_set_level(int pin, hal_gpio_level_t level);
hal_gpio_level_t hal_gpio_get_level(int pin);
esp_err_t hal_gpio_set_pwm_duty(int pin, uint32_t duty_percent);

#ifdef __cplusplus
}
#endif

#endif /* HAL_GPIO_H */
