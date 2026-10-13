/**
 * @file sensor_manager.h
 * @brief Multi-sensor polling coordinator, ring buffer, and fault supervisor.
 */

#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "sensor_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SENSOR_RING_BUFFER_CAPACITY 64

esp_err_t sensor_manager_init(void);
esp_err_t sensor_manager_poll_all(void);
esp_err_t sensor_manager_get_latest_snapshot(multi_sensor_snapshot_t *snapshot);
esp_err_t sensor_manager_get_driver_health(sensor_id_t id, sensor_health_t *health);
bool sensor_manager_has_critical_sensor_fault(void);

#ifdef __cplusplus
}
#endif

#endif /* SENSOR_MANAGER_H */
