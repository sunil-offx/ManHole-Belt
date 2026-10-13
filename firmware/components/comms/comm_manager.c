/**
 * @file comm_manager.c
 * @brief Fail-Safe Communication Manager implementation.
 * Ensures networking failures never delay local sensor monitoring or alarm generation.
 */

#include "comm_manager.h"
#include "hal_timer.h"
#include <string.h>
#include <stdio.h>

#ifdef ESP_PLATFORM
#include "esp_wifi.h"
#include "esp_event.h"
#include "mqtt_client.h"
#include "esp_task_wdt.h"
static esp_mqtt_client_handle_t s_mqtt_client = NULL;
#endif

#define OUTBOX_QUEUE_CAPACITY 16

static comm_config_t s_config;
static comm_state_t s_state = COMM_STATE_DISCONNECTED;
static telemetry_payload_t s_outbox_queue[OUTBOX_QUEUE_CAPACITY];
static size_t s_q_head = 0;
static size_t s_q_count = 0;
static uint32_t s_last_watchdog_kick_ms = 0;

esp_err_t comm_manager_init(const comm_config_t *config) {
    if (config) {
        s_config = *config;
    } else {
        snprintf(s_config.device_id, sizeof(s_config.device_id), "SB-001");
        snprintf(s_config.mqtt_broker_uri, sizeof(s_config.mqtt_broker_uri), "mqtt://192.168.1.100");
        s_config.mqtt_port = 1883;
        s_config.publish_interval_safe_ms = 5000;
        s_config.publish_interval_warning_ms = 2000;
        s_config.publish_interval_critical_ms = 500;
    }

    s_state = COMM_STATE_ONLINE;
    s_last_watchdog_kick_ms = hal_get_time_ms();
    return ESP_OK;
}

esp_err_t comm_manager_queue_telemetry(const telemetry_payload_t *payload) {
    if (!payload) return ESP_ERR_INVALID_ARG;

    /* Push to non-blocking telemetry buffer */
    s_outbox_queue[s_q_head] = *payload;
    s_q_head = (s_q_head + 1) % OUTBOX_QUEUE_CAPACITY;
    if (s_q_count < OUTBOX_QUEUE_CAPACITY) s_q_count++;

    comm_manager_kick_watchdog();
    return ESP_OK;
}

esp_err_t comm_manager_publish_emergency_alert(const char *alert_type, const char *msg) {
    char topic[MAX_TOPIC_LEN];
    char json[MAX_TELEMETRY_JSON_LEN];

    telemetry_get_topic(s_config.device_id, "alert", topic, sizeof(topic));
    telemetry_format_alert_json(s_config.device_id, hal_get_time_ms() / 1000,
                                alert_type, "CRITICAL", msg, json, sizeof(json));

#ifdef ESP_PLATFORM
    if (s_mqtt_client && s_state == COMM_STATE_ONLINE) {
        esp_mqtt_client_publish(s_mqtt_client, topic, json, 0, 1, 0);
    }
#endif
    return ESP_OK;
}

comm_state_t comm_manager_get_state(void) {
    return s_state;
}

void comm_manager_kick_watchdog(void) {
    s_last_watchdog_kick_ms = hal_get_time_ms();
#ifdef ESP_PLATFORM
    esp_task_wdt_reset();
#endif
}

bool comm_manager_is_connected(void) {
    return (s_state == COMM_STATE_ONLINE);
}
