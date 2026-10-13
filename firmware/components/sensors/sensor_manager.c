/**
 * @file sensor_manager.c
 * @brief Multi-sensor coordinator implementation.
 */

#include "sensor_manager.h"
#include "sensor_o2.h"
#include "sensor_toxic_gas.h"
#include "sensor_env.h"
#include "sensor_imu.h"
#include "sensor_water.h"
#include "sensor_spo2.h"
#include "hal_timer.h"
#include <string.h>

typedef struct {
    const sensor_driver_t *driver;
    uint32_t last_poll_ms;
    sensor_reading_t state;
} managed_sensor_entry_t;

static managed_sensor_entry_t s_sensors[SENSOR_COUNT];
static multi_sensor_snapshot_t s_ring_buffer[SENSOR_RING_BUFFER_CAPACITY];
static size_t s_head = 0;
static size_t s_count = 0;
static multi_sensor_snapshot_t s_latest_snapshot;

esp_err_t sensor_manager_init(void) {
    memset(s_sensors, 0, sizeof(s_sensors));
    memset(s_ring_buffer, 0, sizeof(s_ring_buffer));
    memset(&s_latest_snapshot, 0, sizeof(s_latest_snapshot));

    s_sensors[SENSOR_ID_O2].driver = &sensor_driver_o2;
    s_sensors[SENSOR_ID_H2S].driver = &sensor_driver_h2s;
    s_sensors[SENSOR_ID_CO].driver = &sensor_driver_co;
    s_sensors[SENSOR_ID_CH4].driver = &sensor_driver_ch4;
    s_sensors[SENSOR_ID_TEMP].driver = &sensor_driver_temp;
    s_sensors[SENSOR_ID_HUMIDITY].driver = &sensor_driver_humidity;
    s_sensors[SENSOR_ID_IMU_AX].driver = &sensor_driver_imu_ax;
    s_sensors[SENSOR_ID_IMU_AY].driver = &sensor_driver_imu_ay;
    s_sensors[SENSOR_ID_IMU_AZ].driver = &sensor_driver_imu_az;
    s_sensors[SENSOR_ID_WATER_LEVEL].driver = &sensor_driver_water;
    s_sensors[SENSOR_ID_SPO2].driver = &sensor_driver_spo2;

    for (int i = 0; i < SENSOR_COUNT; i++) {
        if (s_sensors[i].driver && s_sensors[i].driver->init) {
            s_sensors[i].driver->init();
            s_sensors[i].state.health = SENSOR_STATUS_OK;
        }
    }
    return ESP_OK;
}

esp_err_t sensor_manager_poll_all(void) {
    uint32_t now = hal_get_time_ms();
    multi_sensor_snapshot_t snap;
    memset(&snap, 0, sizeof(snap));
    snap.timestamp_ms = now;

    for (int i = 0; i < SENSOR_COUNT; i++) {
        managed_sensor_entry_t *entry = &s_sensors[i];
        if (!entry->driver || !entry->driver->read) continue;

        if (now - entry->last_poll_ms >= entry->driver->sample_period_ms) {
            float val = 0.0f;
            esp_err_t err = entry->driver->read(&val);
            entry->last_poll_ms = now;

            if (err == ESP_OK) {
                /* Range validation check */
                if (val < entry->driver->min_value || val > entry->driver->max_value) {
                    entry->state.health = SENSOR_STATUS_OUT_OF_RANGE;
                    entry->state.error_count++;
                } else {
                    entry->state.health = SENSOR_STATUS_OK;
                }
                entry->state.raw_value = val;
                entry->state.filtered_value = val;
                entry->state.timestamp_ms = now;
            } else if (err == ESP_ERR_TIMEOUT) {
                entry->state.health = SENSOR_STATUS_TIMEOUT;
                entry->state.error_count++;
            } else {
                entry->state.health = SENSOR_STATUS_DEGRADED;
                entry->state.error_count++;
            }
        }
    }

    snap.o2_pct          = s_sensors[SENSOR_ID_O2].state.filtered_value;
    snap.h2s_ppm         = s_sensors[SENSOR_ID_H2S].state.filtered_value;
    snap.co_ppm          = s_sensors[SENSOR_ID_CO].state.filtered_value;
    snap.ch4_lel_pct     = s_sensors[SENSOR_ID_CH4].state.filtered_value;
    snap.temp_c          = s_sensors[SENSOR_ID_TEMP].state.filtered_value;
    snap.hum_pct         = s_sensors[SENSOR_ID_HUMIDITY].state.filtered_value;
    snap.water_level_pct = s_sensors[SENSOR_ID_WATER_LEVEL].state.filtered_value;
    snap.acc_x           = s_sensors[SENSOR_ID_IMU_AX].state.filtered_value;
    snap.acc_y           = s_sensors[SENSOR_ID_IMU_AY].state.filtered_value;
    snap.acc_z           = s_sensors[SENSOR_ID_IMU_AZ].state.filtered_value;
    snap.spo2_pct        = s_sensors[SENSOR_ID_SPO2].state.filtered_value;

    /* Push snapshot to thread-safe ring buffer */
    s_ring_buffer[s_head] = snap;
    s_head = (s_head + 1) % SENSOR_RING_BUFFER_CAPACITY;
    if (s_count < SENSOR_RING_BUFFER_CAPACITY) s_count++;
    s_latest_snapshot = snap;

    return ESP_OK;
}

esp_err_t sensor_manager_get_latest_snapshot(multi_sensor_snapshot_t *snapshot) {
    if (!snapshot) return ESP_ERR_INVALID_ARG;
    *snapshot = s_latest_snapshot;
    return ESP_OK;
}

esp_err_t sensor_manager_get_driver_health(sensor_id_t id, sensor_health_t *health) {
    if (id < 0 || id >= SENSOR_COUNT || !health) return ESP_ERR_INVALID_ARG;
    *health = s_sensors[id].state.health;
    return ESP_OK;
}

bool sensor_manager_has_critical_sensor_fault(void) {
    /* If O2 or H2S sensor fails, mark critical fault */
    if (s_sensors[SENSOR_ID_O2].state.health != SENSOR_STATUS_OK ||
        s_sensors[SENSOR_ID_H2S].state.health != SENSOR_STATUS_OK) {
        return true;
    }
    return false;
}
