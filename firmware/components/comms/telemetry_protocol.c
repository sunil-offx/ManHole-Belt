/**
 * @file telemetry_protocol.c
 * @brief Telemetry protocol JSON formatting.
 */

#include "telemetry_protocol.h"
#include <stdio.h>
#include <string.h>

int telemetry_format_json(const telemetry_payload_t *p, char *out_json, size_t max_len) {
    if (!p || !out_json || max_len == 0) return -1;

    int written = snprintf(out_json, max_len,
        "{\n"
        "  \"device_id\": \"%s\",\n"
        "  \"timestamp\": %lu,\n"
        "  \"sensors\": {\n"
        "    \"O2\": %.2f,\n"
        "    \"H2S\": %.2f,\n"
        "    \"CO\": %.2f,\n"
        "    \"CH4\": %.2f,\n"
        "    \"temp\": %.2f,\n"
        "    \"hum\": %.2f,\n"
        "    \"water_level\": %.2f,\n"
        "    \"ax\": %.2f,\n"
        "    \"ay\": %.2f,\n"
        "    \"az\": %.2f\n"
        "  },\n"
        "  \"risk_score\": %.2f,\n"
        "  \"status\": \"%s\",\n"
        "  \"anomaly\": %s,\n"
        "  \"fall_detected\": %s\n"
        "}",
        p->device_id,
        (unsigned long)p->timestamp,
        p->sensors.o2_pct,
        p->sensors.h2s_ppm,
        p->sensors.co_ppm,
        p->sensors.ch4_lel_pct,
        p->sensors.temp_c,
        p->sensors.hum_pct,
        p->sensors.water_level_pct,
        p->sensors.acc_x,
        p->sensors.acc_y,
        p->sensors.acc_z,
        p->risk_score,
        p->status_str ? p->status_str : "SAFE",
        p->anomaly ? "true" : "false",
        p->fall_detected ? "true" : "false"
    );

    return (written > 0 && (size_t)written < max_len) ? written : -1;
}

int telemetry_format_alert_json(const char *device_id, uint32_t timestamp, const char *alert_type,
                                const char *severity, const char *msg, char *out_json, size_t max_len) {
    if (!out_json || max_len == 0) return -1;

    int written = snprintf(out_json, max_len,
        "{\n"
        "  \"device_id\": \"%s\",\n"
        "  \"timestamp\": %lu,\n"
        "  \"alert_type\": \"%s\",\n"
        "  \"severity\": \"%s\",\n"
        "  \"message\": \"%s\"\n"
        "}",
        device_id ? device_id : "UNKNOWN",
        (unsigned long)timestamp,
        alert_type ? alert_type : "GENERAL_HAZARD",
        severity ? severity : "CRITICAL",
        msg ? msg : "Immediate evacuation required!"
    );

    return (written > 0 && (size_t)written < max_len) ? written : -1;
}

void telemetry_get_topic(const char *device_id, const char *subtopic, char *out_topic, size_t max_len) {
    snprintf(out_topic, max_len, "sentinel/%s/%s", device_id ? device_id : "SB-001", subtopic ? subtopic : "telemetry");
}
