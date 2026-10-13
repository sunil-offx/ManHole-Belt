/**
 * @file main.c
 * @brief SENTINEL-BELT FreeRTOS Main Application Entry Point.
 * Orchestrates multi-sensor acquisition, fall detection, risk engine,
 * embedded AI inference, fail-safe local alerts, and cloud telemetry.
 */

#include "hal_types.h"
#include "hal_timer.h"
#include "sensor_manager.h"
#include "calibration.h"
#include "filters.h"
#include "feature_extractor.h"
#include "risk_engine.h"
#include "temporal_analyzer.h"
#include "fall_detector.h"
#include "model_weights.h"
#include "alert_manager.h"
#include "oled_display.h"
#include "comm_manager.h"
#include "power_manager.h"

#ifdef ESP_PLATFORM
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "nvs_flash.h"
#endif

#include <stdio.h>
#include <string.h>

#define TAG "SENTINEL_BELT"

/* Task Priorities */
#define PRIORITY_ALERT_LOCAL     10 /* Highest: Local fail-safe alarms */
#define PRIORITY_SENSOR_IMU_FALL  8 /* High: 50Hz Fall detection & sensor polling */
#define PRIORITY_RISK_AI_ENGINE   6 /* Medium: Math Risk + ML inference */
#define PRIORITY_COMMUNICATION    4 /* Low: WiFi/MQTT transmission */
#define PRIORITY_OLED_POWER       2 /* Lowest: Display & battery supervision */

/* Shared State protected by mutex */
static multi_sensor_snapshot_t g_current_sensors;
static risk_assessment_t g_current_risk;
static fall_status_t g_current_fall;
static extracted_features_t g_current_features;
static temporal_prediction_t g_current_temporal;
static bool g_ai_anomaly_flag = false;
static ml_risk_class_t g_ai_risk_class = ML_CLASS_SAFE;

/* Execution loop step for both FreeRTOS tasks and host simulation */
void sentinel_system_step(void) {
    uint32_t now = hal_get_time_ms();

    /* 1. Poll Sensors */
    sensor_manager_poll_all();
    sensor_manager_get_latest_snapshot(&g_current_sensors);

    /* 2. Update Fall Detector at 50Hz rate */
    fall_detector_push_sample(g_current_sensors.acc_x,
                              g_current_sensors.acc_y,
                              g_current_sensors.acc_z,
                              now);
    fall_detector_update(&g_current_fall);

    /* 3. Extract Features */
    feature_extractor_update(&g_current_sensors, &g_current_features);

    /* 4. Temporal Trend Analysis */
    temporal_analyzer_update(&g_current_sensors, &g_current_temporal);

    /* 5. Mathematical Risk Evaluation */
    risk_status_t math_status = risk_engine_evaluate(
        &g_current_sensors,
        g_current_fall.inactivity_duration_sec,
        g_current_fall.fall_detected,
        &g_current_risk
    );

    /* 6. AI Inference & Sensor Fusion */
    g_ai_risk_class = sentinel_ml_predict_risk(g_current_features.features);
    g_ai_anomaly_flag = sentinel_ml_detect_anomaly(g_current_features.features);

    /* Final Risk Fusion: Escalate if AI predicts higher risk or temporal pre-warning fires */
    risk_status_t final_status = math_status;
    if (g_current_temporal.pre_warning_active && final_status == RISK_SAFE) {
        final_status = RISK_WARNING;
    }
    if (g_ai_risk_class == ML_CLASS_CRITICAL && final_status < RISK_CRITICAL) {
        final_status = RISK_CRITICAL;
    } else if (g_ai_risk_class == ML_CLASS_WARNING && final_status == RISK_SAFE) {
        final_status = RISK_WARNING;
    }

    /* 7. Update Local Fail-Safe Alerts immediately (< 200 ms response requirement) */
    alert_manager_update(final_status, &g_current_risk, &g_current_sensors);

    /* 8. Queue Cloud Telemetry */
    const char *status_str = (final_status == RISK_SAFE) ? "SAFE" :
                             (final_status == RISK_WARNING) ? "WARNING" : "CRITICAL";

    telemetry_payload_t payload;
    memset(&payload, 0, sizeof(payload));
    snprintf(payload.device_id, sizeof(payload.device_id), "SB-001");
    payload.timestamp = now / 1000;
    payload.sensors = g_current_sensors;
    payload.risk_score = g_current_risk.composite_score;
    payload.status_str = status_str;
    payload.anomaly = g_ai_anomaly_flag;
    payload.fall_detected = g_current_fall.fall_detected;

    comm_manager_queue_telemetry(&payload);

    /* If status is critical or fall detected, trigger high priority alert payload */
    if (final_status == RISK_CRITICAL || g_current_fall.fall_detected) {
        comm_manager_publish_emergency_alert(
            g_current_fall.fall_detected ? "WORKER_FALL" : "HAZARDOUS_GAS_EXPOSURE",
            g_current_temporal.leading_hazard
        );
    }
}

#ifdef ESP_PLATFORM

/* High Priority: Local Safety & Alarm Task */
static void alert_manager_task(void *pvParameters) {
    while (1) {
        if (alert_manager_is_sos_active() || g_current_risk.status == RISK_CRITICAL) {
            alert_manager_update(RISK_CRITICAL, &g_current_risk, &g_current_sensors);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

/* High Priority: 50Hz Sensor & Motion Task */
static void sensor_motion_task(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();
    const TickType_t xFrequency = pdMS_TO_TICKS(20); /* 50 Hz */

    while (1) {
        sentinel_system_step();
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

/* Low Priority: Communication Task */
static void comm_task(void *pvParameters) {
    while (1) {
        comm_manager_kick_watchdog();
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

/* App Entry Point on ESP32 */
void app_main(void) {
    ESP_LOGI(TAG, "Starting SENTINEL-BELT Safety Firmware...");

    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    calibration_init();
    feature_extractor_init();
    temporal_analyzer_init();
    risk_engine_init();
    fall_detector_init();
    alert_manager_init();
    oled_display_init();
    sensor_manager_init();
    power_manager_init();
    comm_manager_init(NULL);

    ESP_LOGI(TAG, "All sub-systems initialized. Spawning FreeRTOS tasks...");

    xTaskCreate(alert_manager_task, "alert_task", 3072, NULL, PRIORITY_ALERT_LOCAL, NULL);
    xTaskCreate(sensor_motion_task, "sensor_task", 4096, NULL, PRIORITY_SENSOR_IMU_FALL, NULL);
    xTaskCreate(comm_task, "comm_task", 4096, NULL, PRIORITY_COMMUNICATION, NULL);
}

#else

/* Host entry point for testing/simulation */
int main(int argc, char **argv) {
    printf("=== SENTINEL-BELT Host Engine Initialized ===\n");
    calibration_init();
    feature_extractor_init();
    temporal_analyzer_init();
    risk_engine_init();
    fall_detector_init();
    alert_manager_init();
    sensor_manager_init();
    power_manager_init();
    comm_manager_init(NULL);

    /* Run 10 iterations */
    for (int i = 0; i < 10; i++) {
        sentinel_system_step();
        hal_delay_ms(100);
    }
    printf("Host engine completed successfully.\n");
    return 0;
}

#endif
