/**
 * @file telemetry_protocol.h
 * @brief MQTT and HTTP Telemetry JSON serialization and parsing matching SENTINEL-BELT standard.
 */

#ifndef TELEMETRY_PROTOCOL_H
#define TELEMETRY_PROTOCOL_H

#include "sensor_types.h"
#include "risk_engine.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_TELEMETRY_JSON_LEN 512
#define MAX_TOPIC_LEN          64

typedef struct {
    char device_id[16];
    uint32_t timestamp;
    multi_sensor_snapshot_t sensors;
    float risk_score;
    const char *status_str;
    bool anomaly;
    bool fall_detected;
} telemetry_payload_t;

int telemetry_format_json(const telemetry_payload_t *payload, char *out_json, size_t max_len);
int telemetry_format_alert_json(const char *device_id, uint32_t timestamp, const char *alert_type,
                                const char *severity, const char *msg, char *out_json, size_t max_len);

void telemetry_get_topic(const char *device_id, const char *subtopic, char *out_topic, size_t max_len);

#ifdef __cplusplus
}
#endif

#endif /* TELEMETRY_PROTOCOL_H */
