/**
 * @file comm_manager.h
 * @brief Fail-Safe Communication Manager (WiFi, MQTT, BLE, LoRa, Watchdog).
 */

#ifndef COMM_MANAGER_H
#define COMM_MANAGER_H

#include "hal_types.h"
#include "telemetry_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    COMM_STATE_DISCONNECTED = 0,
    COMM_STATE_CONNECTING_WIFI,
    COMM_STATE_CONNECTING_MQTT,
    COMM_STATE_ONLINE,
    COMM_STATE_ERROR_RETRYING
} comm_state_t;

typedef struct {
    char wifi_ssid[32];
    char wifi_password[64];
    char mqtt_broker_uri[128];
    uint16_t mqtt_port;
    char device_id[16];
    uint32_t publish_interval_safe_ms;    /* e.g., 5000 ms */
    uint32_t publish_interval_warning_ms; /* e.g., 2000 ms */
    uint32_t publish_interval_critical_ms;/* e.g., 500 ms */
} comm_config_t;

esp_err_t comm_manager_init(const comm_config_t *config);
esp_err_t comm_manager_queue_telemetry(const telemetry_payload_t *payload);
esp_err_t comm_manager_publish_emergency_alert(const char *alert_type, const char *msg);
comm_state_t comm_manager_get_state(void);
void comm_manager_kick_watchdog(void);
bool comm_manager_is_connected(void);

#ifdef __cplusplus
}
#endif

#endif /* COMM_MANAGER_H */
