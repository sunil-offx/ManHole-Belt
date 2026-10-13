/**
 * @file oled_display.c
 * @brief SSD1306 OLED UI renderer implementation.
 */

#include "oled_display.h"
#include "hal_i2c.h"
#include <stdio.h>
#include <string.h>

#define OLED_I2C_ADDR 0x3C

static bool s_is_asleep = false;

esp_err_t oled_display_init(void) {
    /* Initialize SSD1306 over I2C */
    uint8_t init_cmds[] = {
        0x00, /* Command stream */
        0xAE, /* Display OFF */
        0xD5, 0x80, /* Set osc division */
        0xA8, 0x3F, /* Multiplex ratio 1/64 */
        0xD3, 0x00, /* Display offset 0 */
        0x40, /* Start line 0 */
        0x8D, 0x14, /* Charge pump ON */
        0x20, 0x00, /* Memory mode horizontal */
        0xA1, /* Segment remap */
        0xC8, /* COM scan direction */
        0xDA, 0x12, /* COM pins hardware config */
        0x81, 0xCF, /* Contrast */
        0xD9, 0xF1, /* Pre-charge */
        0xDB, 0x40, /* VCOMH */
        0xA4, /* Resume RAM */
        0xA6, /* Normal display */
        0xAF  /* Display ON */
    };
    return hal_i2c_write(HAL_I2C_PORT_0, OLED_I2C_ADDR, init_cmds, sizeof(init_cmds));
}

void oled_show_safe(float risk_score, const multi_sensor_snapshot_t *sensors) {
    if (s_is_asleep || !sensors) return;
    /* In real hardware, format buffer & blast to SSD1306 RAM */
    /* Lines displayed:
       [STATUS: SAFE  R: 12%]
       O2: 20.9%  H2S: 0.1ppm
       CO: 1ppm   CH4: 0%LEL
       WTR: 5%    TMP: 24.5C
    */
}

void oled_show_warning(float risk_score, const multi_sensor_snapshot_t *sensors) {
    if (s_is_asleep || !sensors) return;
    /* Flashing WARNING banner and hazard gas highlight */
}

void oled_show_critical(float risk_score, const multi_sensor_snapshot_t *sensors) {
    if (!sensors) return;
    s_is_asleep = false; /* Always wake OLED on critical hazard */
    /* High contrast inverted CRITICAL EVACUATE prompt */
}

void oled_show_battery(float bat_pct, float voltage) {
    (void)bat_pct;
    (void)voltage;
}

void oled_set_sleep(bool sleep) {
    s_is_asleep = sleep;
    uint8_t cmd[2] = {0x00, sleep ? 0xAE : 0xAF};
    hal_i2c_write(HAL_I2C_PORT_0, OLED_I2C_ADDR, cmd, sizeof(cmd));
}
