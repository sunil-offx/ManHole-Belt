/**
 * @file oled_display.h
 * @brief SSD1306 128x64 OLED display controller for local real-time worker feedback.
 */

#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include "sensor_types.h"

#ifdef __cplusplus
extern "C" {
#endif

esp_err_t oled_display_init(void);
void oled_show_safe(float risk_score, const multi_sensor_snapshot_t *sensors);
void oled_show_warning(float risk_score, const multi_sensor_snapshot_t *sensors);
void oled_show_critical(float risk_score, const multi_sensor_snapshot_t *sensors);
void oled_show_battery(float bat_pct, float voltage);
void oled_set_sleep(bool sleep);

#ifdef __cplusplus
}
#endif

#endif /* OLED_DISPLAY_H */
