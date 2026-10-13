/**
 * @file power_manager.h
 * @brief Power management, battery ADC monitoring, and light sleep duty cycling.
 */

#ifndef POWER_MANAGER_H
#define POWER_MANAGER_H

#include "hal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float battery_voltage;
    float battery_percent;
    bool low_battery_alert;
    bool critical_shutdown_needed;
    uint32_t estimated_runtime_hours;
} battery_status_t;

esp_err_t power_manager_init(void);
esp_err_t power_manager_update(battery_status_t *out_status);
void power_manager_enter_light_sleep_ms(uint32_t ms);

#ifdef __cplusplus
}
#endif

#endif /* POWER_MANAGER_H */
