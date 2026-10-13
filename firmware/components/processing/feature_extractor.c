/**
 * @file feature_extractor.c
 * @brief Implement feature extraction logic.
 */

#include "feature_extractor.h"
#include <math.h>
#include <string.h>

static multi_sensor_snapshot_t s_prev_snapshot;
static bool s_has_prev = false;

void feature_extractor_init(void) {
    memset(&s_prev_snapshot, 0, sizeof(s_prev_snapshot));
    s_has_prev = false;
}

esp_err_t feature_extractor_update(const multi_sensor_snapshot_t *current, extracted_features_t *out) {
    if (!current || !out) return ESP_ERR_INVALID_ARG;

    float dt_sec = 1.0f;
    if (s_has_prev && current->timestamp_ms > s_prev_snapshot.timestamp_ms) {
        dt_sec = (float)(current->timestamp_ms - s_prev_snapshot.timestamp_ms) / 1000.0f;
        if (dt_sec < 0.05f) dt_sec = 0.05f; /* Avoid division by zero */
    }

    float do2_dt = s_has_prev ? (current->o2_pct - s_prev_snapshot.o2_pct) / dt_sec : 0.0f;
    float dh2s_dt = s_has_prev ? (current->h2s_ppm - s_prev_snapshot.h2s_ppm) / dt_sec : 0.0f;
    float dco_dt = s_has_prev ? (current->co_ppm - s_prev_snapshot.co_ppm) / dt_sec : 0.0f;
    float dch4_dt = s_has_prev ? (current->ch4_lel_pct - s_prev_snapshot.ch4_lel_pct) / dt_sec : 0.0f;

    float acc_mag = sqrtf(current->acc_x * current->acc_x + 
                          current->acc_y * current->acc_y + 
                          current->acc_z * current->acc_z);

    out->accel_magnitude = acc_mag;
    out->do2_dt = do2_dt;
    out->dh2s_dt = dh2s_dt;
    out->dco_dt = dco_dt;
    out->dch4_dt = dch4_dt;
    out->timestamp_ms = current->timestamp_ms;

    /* Build 14-element feature vector matching ML model specification */
    out->features[FEAT_O2]       = current->o2_pct;
    out->features[FEAT_H2S]      = current->h2s_ppm;
    out->features[FEAT_CO]       = current->co_ppm;
    out->features[FEAT_CH4]      = current->ch4_lel_pct;
    out->features[FEAT_TEMP]     = current->temp_c;
    out->features[FEAT_HUM]      = current->hum_pct;
    out->features[FEAT_WATER]    = current->water_level_pct;
    out->features[FEAT_AX]       = current->acc_x;
    out->features[FEAT_AY]       = current->acc_y;
    out->features[FEAT_AZ]       = current->acc_z;
    out->features[FEAT_DO2_DT]   = do2_dt;
    out->features[FEAT_DH2S_DT]  = dh2s_dt;
    out->features[FEAT_DCO_DT]   = dco_dt;
    out->features[FEAT_DCH4_DT]  = dch4_dt;

    s_prev_snapshot = *current;
    s_has_prev = true;
    return ESP_OK;
}
