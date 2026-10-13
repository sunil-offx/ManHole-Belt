/**
 * @file feature_extractor.h
 * @brief Derives dynamic features: rates of change, vector magnitudes, and 14-D ML feature vectors.
 */

#ifndef FEATURE_EXTRACTOR_H
#define FEATURE_EXTRACTOR_H

#include "sensor_types.h"
#include "model_weights.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    float features[ML_FEATURE_COUNT];
    float accel_magnitude;
    float do2_dt;
    float dh2s_dt;
    float dco_dt;
    float dch4_dt;
    uint32_t timestamp_ms;
} extracted_features_t;

void feature_extractor_init(void);
esp_err_t feature_extractor_update(const multi_sensor_snapshot_t *current, extracted_features_t *out_features);

#ifdef __cplusplus
}
#endif

#endif /* FEATURE_EXTRACTOR_H */
