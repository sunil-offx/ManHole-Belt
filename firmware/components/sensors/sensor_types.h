/**
 * @file sensor_types.h
 * @brief Unified Sensor Abstraction Layer types and interfaces.
 */

#ifndef SENSOR_TYPES_H
#define SENSOR_TYPES_H

#include "hal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    SENSOR_ID_O2 = 0,
    SENSOR_ID_H2S,
    SENSOR_ID_CO,
    SENSOR_ID_CH4,
    SENSOR_ID_TEMP,
    SENSOR_ID_HUMIDITY,
    SENSOR_ID_IMU_AX,
    SENSOR_ID_IMU_AY,
    SENSOR_ID_IMU_AZ,
    SENSOR_ID_WATER_LEVEL,
    SENSOR_ID_SPO2,
    SENSOR_COUNT
} sensor_id_t;

typedef enum {
    SENSOR_STATUS_OK = 0,
    SENSOR_STATUS_UNINITIALIZED,
    SENSOR_STATUS_OUT_OF_RANGE,
    SENSOR_STATUS_TIMEOUT,
    SENSOR_STATUS_CRC_ERROR,
    SENSOR_STATUS_DEGRADED
} sensor_health_t;

/**
 * @brief Authoritative Sensor Driver Interface
 */
typedef struct {
    esp_err_t (*init)(void);
    esp_err_t (*read)(float *value);
    esp_err_t (*calibrate)(void);
    const char* name;
    float min_value;
    float max_value;
    uint32_t sample_period_ms;
} sensor_driver_t;

typedef struct {
    float raw_value;
    float filtered_value;
    uint32_t timestamp_ms;
    sensor_health_t health;
    uint32_t error_count;
} sensor_reading_t;

typedef struct {
    float o2_pct;           /* % Oxygen (Normal: 20.9%) */
    float h2s_ppm;          /* ppm H2S (PEL: 10ppm, IDLH: 100ppm) */
    float co_ppm;           /* ppm CO (PEL: 35ppm, IDLH: 1200ppm) */
    float ch4_lel_pct;      /* % LEL Methane (LEL is 5% vol) */
    float temp_c;           /* Celsius */
    float hum_pct;          /* % Relative Humidity */
    float water_level_pct;  /* % Water Level (0 to 100%) */
    float acc_x;            /* g-force X */
    float acc_y;            /* g-force Y */
    float acc_z;            /* g-force Z */
    float spo2_pct;         /* % SpO2 (Optional) */
    uint32_t timestamp_ms;
} multi_sensor_snapshot_t;

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_TYPES_H */
