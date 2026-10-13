/**
 * @file fall_detector.h
 * @brief 50 Hz IMU Fall Detection and Worker Inactivity Supervisor.
 */

#ifndef FALL_DETECTOR_H
#define FALL_DETECTOR_H

#include "hal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IMU_HISTORY_CAPACITY 250 /* 5 seconds at 50 Hz */

typedef struct {
    float ax, ay, az;
    float magnitude;
    float orientation_angle_deg;
    uint32_t timestamp;
} imu_sample_t;

typedef enum {
    MOTION_STATE_NORMAL = 0,
    MOTION_STATE_FREEFALL_SUSPECTED,
    MOTION_STATE_IMPACT_DETECTED,
    MOTION_STATE_FALL_CONFIRMED,
    MOTION_STATE_PROLONGED_INACTIVITY
} motion_state_t;

typedef struct {
    motion_state_t state;
    bool fall_detected;
    float inactivity_duration_sec;
    float peak_impact_g;
    float orientation_shift_deg;
    uint32_t last_movement_timestamp_ms;
} fall_status_t;

void fall_detector_init(void);
void fall_detector_push_sample(float ax, float ay, float az, uint32_t timestamp_ms);
void fall_detector_update(fall_status_t *out_status);
void fall_detector_reset(void);

#ifdef __cplusplus
}
#endif

#endif /* FALL_DETECTOR_H */
