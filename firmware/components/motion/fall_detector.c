/**
 * @file fall_detector.c
 * @brief Fall detection algorithm implementation.
 */

#include "fall_detector.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

static imu_sample_t s_buffer[IMU_HISTORY_CAPACITY];
static size_t s_head = 0;
static size_t s_count = 0;

static motion_state_t s_state = MOTION_STATE_NORMAL;
static uint32_t s_freefall_start_ms = 0;
static uint32_t s_impact_ms = 0;
static float s_pre_impact_az = 1.0f;
static float s_peak_impact_g = 1.0f;
static float s_orientation_shift_deg = 0.0f;
static uint32_t s_last_active_ms = 0;
static bool s_fall_confirmed = false;

void fall_detector_init(void) {
    memset(s_buffer, 0, sizeof(s_buffer));
    s_head = 0;
    s_count = 0;
    s_state = MOTION_STATE_NORMAL;
    s_freefall_start_ms = 0;
    s_impact_ms = 0;
    s_pre_impact_az = 1.0f;
    s_peak_impact_g = 1.0f;
    s_orientation_shift_deg = 0.0f;
    s_last_active_ms = 0;
    s_fall_confirmed = false;
}

void fall_detector_reset(void) {
    fall_detector_init();
}

void fall_detector_push_sample(float ax, float ay, float az, uint32_t timestamp_ms) {
    float mag = sqrtf(ax * ax + ay * ay + az * az);
    
    imu_sample_t sample = {
        .ax = ax,
        .ay = ay,
        .az = az,
        .magnitude = mag,
        .orientation_angle_deg = acosf(fmaxf(-1.0f, fminf(1.0f, az / fmaxf(mag, 0.001f)))) * (180.0f / (float)M_PI),
        .timestamp = timestamp_ms
    };

    s_buffer[s_head] = sample;
    s_head = (s_head + 1) % IMU_HISTORY_CAPACITY;
    if (s_count < IMU_HISTORY_CAPACITY) s_count++;

    /* Track gross activity (dynamic deviation from 1g) */
    if (fabsf(mag - 1.0f) > 0.15f) {
        s_last_active_ms = timestamp_ms;
    }
}

void fall_detector_update(fall_status_t *out_status) {
    if (!out_status || s_count == 0) return;

    size_t latest_idx = (s_head + IMU_HISTORY_CAPACITY - 1) % IMU_HISTORY_CAPACITY;
    imu_sample_t cur = s_buffer[latest_idx];
    uint32_t now = cur.timestamp;

    switch (s_state) {
        case MOTION_STATE_NORMAL:
            /* Detect free-fall: A < 0.5g */
            if (cur.magnitude < 0.5f) {
                s_state = MOTION_STATE_FREEFALL_SUSPECTED;
                s_freefall_start_ms = now;
                s_pre_impact_az = cur.az;
            } else if (cur.magnitude > 2.5f) {
                /* Sudden direct impact without detected freefall phase (slip/trip) */
                s_state = MOTION_STATE_IMPACT_DETECTED;
                s_impact_ms = now;
                s_peak_impact_g = cur.magnitude;
            }
            break;

        case MOTION_STATE_FREEFALL_SUSPECTED:
            if (cur.magnitude > 2.5f) {
                /* Freefall succeeded by heavy impact */
                s_state = MOTION_STATE_IMPACT_DETECTED;
                s_impact_ms = now;
                s_peak_impact_g = cur.magnitude;
            } else if (now - s_freefall_start_ms > 1000) {
                /* Freefall timed out without impact (e.g. sensor dip) */
                s_state = MOTION_STATE_NORMAL;
            }
            break;

        case MOTION_STATE_IMPACT_DETECTED:
            if (cur.magnitude > s_peak_impact_g) {
                s_peak_impact_g = cur.magnitude;
            }
            /* Step 4: Check post-impact stabilization time (> 3 seconds of stillness) */
            if (now - s_impact_ms >= 3000) {
                /* Verify stillness around ~1g during post-impact period */
                if (cur.magnitude >= 0.8f && cur.magnitude <= 1.25f) {
                    /* Orientation vector change */
                    s_orientation_shift_deg = fabsf(cur.az - s_pre_impact_az) * 90.0f;
                    if (s_orientation_shift_deg > 30.0f || s_peak_impact_g > 2.8f) {
                        s_state = MOTION_STATE_FALL_CONFIRMED;
                        s_fall_confirmed = true;
                    } else {
                        s_state = MOTION_STATE_NORMAL;
                    }
                } else {
                    /* Worker still moving / struggling to get up, reset impact state */
                    s_state = MOTION_STATE_NORMAL;
                }
            }
            break;

        case MOTION_STATE_FALL_CONFIRMED:
            /* Remains confirmed until reset or supervisor acknowledgment */
            break;

        case MOTION_STATE_PROLONGED_INACTIVITY:
            break;
    }

    float inact_sec = 0.0f;
    if (s_last_active_ms > 0 && now >= s_last_active_ms) {
        inact_sec = (float)(now - s_last_active_ms) / 1000.0f;
    }

    out_status->state = s_state;
    out_status->fall_detected = s_fall_confirmed;
    out_status->inactivity_duration_sec = inact_sec;
    out_status->peak_impact_g = s_peak_impact_g;
    out_status->orientation_shift_deg = s_orientation_shift_deg;
    out_status->last_movement_timestamp_ms = s_last_active_ms;
}
