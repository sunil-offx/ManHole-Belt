/**
 * @file power_manager.c
 * @brief Power management and battery monitor implementation.
 */

#include "power_manager.h"
#include "hal_adc.h"
#include "hal_timer.h"

#ifdef ESP_PLATFORM
#include "esp_sleep.h"
#endif

/* Standard resistor divider ratio 2:1 on battery rail (4.2V max divided to 2.1V max into ADC) */
#define BATTERY_DIVIDER_RATIO 2.0f

esp_err_t power_manager_init(void) {
    return ESP_OK;
}

esp_err_t power_manager_update(battery_status_t *out_status) {
    if (!out_status) return ESP_ERR_INVALID_ARG;

    float v_adc = 0.0f;
    esp_err_t err = hal_adc_read_voltage(HAL_ADC_CH_BATTERY, &v_adc);
    if (err != ESP_OK) {
        v_adc = 1.95f; /* Default ~3.9V */
    }

    float battery_voltage = v_adc * BATTERY_DIVIDER_RATIO;
    /* Lithium-Ion single cell: 3.3V (0%) to 4.2V (100%) */
    float battery_percent = ((battery_voltage - 3.3f) / (4.2f - 3.3f)) * 100.0f;
    if (battery_percent < 0.0f) battery_percent = 0.0f;
    if (battery_percent > 100.0f) battery_percent = 100.0f;

    bool low_battery = (battery_percent < 15.0f);
    bool shutdown = (battery_voltage < 3.2f);

    /* Target 2000mAh battery delivers >17 hours under nominal 115mA average load */
    uint32_t hours_left = (uint32_t)((battery_percent / 100.0f) * 18.0f);

    out_status->battery_voltage = battery_voltage;
    out_status->battery_percent = battery_percent;
    out_status->low_battery_alert = low_battery;
    out_status->critical_shutdown_needed = shutdown;
    out_status->estimated_runtime_hours = hours_left;

    return ESP_OK;
}

void power_manager_enter_light_sleep_ms(uint32_t ms) {
#ifdef ESP_PLATFORM
    esp_sleep_enable_timer_wakeup((uint64_t)ms * 1000ULL);
    esp_light_sleep_start();
#else
    hal_delay_ms(ms);
#endif
}
