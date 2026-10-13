/**
 * @file temporal_analyzer.h
 * @brief Temporal Hazard Analysis: rate-of-change and time-to-threshold early warning.
 */

#ifndef TEMPORAL_ANALYZER_H
#define TEMPORAL_ANALYZER_H

#include "sensor_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TEMPORAL_WINDOW_SIZE 10

typedef struct {
    float threshold_o2_min;  /* e.g., 19.5% */
    float threshold_h2s_max; /* e.g., 10.0 ppm */
    float threshold_co_max;  /* e.g., 25.0 ppm */
    float threshold_ch4_max; /* e.g., 10.0 % LEL */
    float early_warning_sec; /* e.g., 120.0 seconds (2 mins) */
} temporal_config_t;

typedef struct {
    float rate_o2;           /* % / sec */
    float rate_h2s;          /* ppm / sec */
    float rate_co;           /* ppm / sec */
    float rate_ch4;          /* % LEL / sec */
    float t_to_threshold_o2; /* seconds */
    float t_to_threshold_h2s;/* seconds */
    float t_to_threshold_co; /* seconds */
    float t_to_threshold_ch4;/* seconds */
    bool pre_warning_active;
    const char *leading_hazard;
} temporal_prediction_t;

void temporal_analyzer_init(void);
void temporal_analyzer_set_config(const temporal_config_t *config);
void temporal_analyzer_update(const multi_sensor_snapshot_t *sample, temporal_prediction_t *out_pred);

#ifdef __cplusplus
}
#endif

#endif /* TEMPORAL_ANALYZER_H */
