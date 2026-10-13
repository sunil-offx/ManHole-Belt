/**
 * @file alert_manager.h
 * @brief Fail-Safe Local Alert System Manager (Buzzer PWM, Vibration, RGB LED, SOS).
 */

#ifndef ALERT_MANAGER_H
#define ALERT_MANAGER_H

#include "risk_engine.h"
#include "sensor_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    LED_COLOR_OFF = 0,
    LED_COLOR_GREEN,
    LED_COLOR_YELLOW,
    LED_COLOR_RED,
    LED_COLOR_BLUE_SOS
} led_color_t;

typedef enum {
    BUZZER_SILENT = 0,
    BUZZER_PATTERN_WARNING_BEEP,
    BUZZER_CONTINUOUS_CRITICAL,
    BUZZER_SOS_SIREN
} buzzer_mode_t;

typedef enum {
    VIB_OFF = 0,
    VIB_INTERMITTENT_PULSE,
    VIB_CONTINUOUS_MAX
} vibration_mode_t;

esp_err_t alert_manager_init(void);
void alert_manager_trigger_sos(void);
bool alert_manager_is_sos_active(void);
void alert_manager_clear_sos(void);

void alert_manager_update(risk_status_t status,
                          const risk_assessment_t *assessment,
                          const multi_sensor_snapshot_t *sensors);

#ifdef __cplusplus
}
#endif

#endif /* ALERT_MANAGER_H */
