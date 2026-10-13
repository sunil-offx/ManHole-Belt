/**
 * @file hal_gpio.c
 * @brief GPIO Hardware Abstraction implementation.
 */

#include "hal_gpio.h"

#ifdef ESP_PLATFORM
#include "driver/gpio.h"
#include "driver/ledc.h"

esp_err_t hal_gpio_init_output(int pin) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    return gpio_config(&io_conf);
}

esp_err_t hal_gpio_init_input(int pin, bool pull_up) {
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_INPUT,
        .pull_up_en = pull_up ? GPIO_PULLUP_ENABLE : GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    return gpio_config(&io_conf);
}

esp_err_t hal_gpio_set_level(int pin, hal_gpio_level_t level) {
    return gpio_set_level((gpio_num_t)pin, (uint32_t)level);
}

hal_gpio_level_t hal_gpio_get_level(int pin) {
    return (hal_gpio_level_t)gpio_get_level((gpio_num_t)pin);
}

esp_err_t hal_gpio_set_pwm_duty(int pin, uint32_t duty_percent) {
    /* Simple PWM using LEDC on ESP32 */
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel    = LEDC_CHANNEL_0,
        .timer_sel  = LEDC_TIMER_0,
        .intr_type  = LEDC_INTR_DISABLE,
        .gpio_num   = pin,
        .duty       = (duty_percent * 255) / 100,
        .hpoint     = 0
    };
    return ledc_channel_config(&ledc_channel);
}

#else
/* Native host mock implementation */
static hal_gpio_level_t mock_pins[64] = {HAL_GPIO_LOW};

esp_err_t hal_gpio_init_output(int pin) {
    if (pin < 0 || pin >= 64) return ESP_ERR_INVALID_ARG;
    mock_pins[pin] = HAL_GPIO_LOW;
    return ESP_OK;
}

esp_err_t hal_gpio_init_input(int pin, bool pull_up) {
    if (pin < 0 || pin >= 64) return ESP_ERR_INVALID_ARG;
    mock_pins[pin] = pull_up ? HAL_GPIO_HIGH : HAL_GPIO_LOW;
    return ESP_OK;
}

esp_err_t hal_gpio_set_level(int pin, hal_gpio_level_t level) {
    if (pin < 0 || pin >= 64) return ESP_ERR_INVALID_ARG;
    mock_pins[pin] = level;
    return ESP_OK;
}

hal_gpio_level_t hal_gpio_get_level(int pin) {
    if (pin < 0 || pin >= 64) return HAL_GPIO_LOW;
    return mock_pins[pin];
}

esp_err_t hal_gpio_set_pwm_duty(int pin, uint32_t duty_percent) {
    (void)pin;
    (void)duty_percent;
    return ESP_OK;
}
#endif
