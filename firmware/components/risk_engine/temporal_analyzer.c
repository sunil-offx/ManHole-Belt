/**
 * @file temporal_analyzer.c
 * @brief Temporal Hazard Analysis implementation with sliding window and time-to-threshold estimation.
 */

#include "temporal_analyzer.h"
#include <math.h>
#include <string.h>

#ifndef INFINITY
#define INFINITY (1e9f)
#endif

typedef struct {
    float values[TEMPORAL_WINDOW_SIZE];
    uint32_t timestamps[TEMPORAL_WINDOW_SIZE];
    size_t count;
    size_t head;
} temporal_history_t;

static temporal_history_t s_hist_o2;
static temporal_history_t s_hist_h2s;
static temporal_history_t s_hist_co;
static temporal_history_t s_hist_ch4;

static temporal_config_t s_cfg = {
    .threshold_o2_min = 19.5f,
    .threshold_h2s_max = 10.0f,
    .threshold_co_max = 25.0f,
    .threshold_ch4_max = 10.0f,
    .early_warning_sec = 120.0f
};

static void push_history(temporal_history_t *h, float val, uint32_t ts) {
    h->values[h->head] = val;
    h->timestamps[h->head] = ts;
    h->head = (h->head + 1) % TEMPORAL_WINDOW_SIZE;
    if (h->count < TEMPORAL_WINDOW_SIZE) h->count++;
}

static float calc_rate(const temporal_history_t *h) {
    if (h->count < 2) return 0.0f;
    size_t latest_idx = (h->head + TEMPORAL_WINDOW_SIZE - 1) % TEMPORAL_WINDOW_SIZE;
    size_t oldest_idx = (h->count < TEMPORAL_WINDOW_SIZE) ? 0 : h->head;

    float dt = (float)(h->timestamps[latest_idx] - h->timestamps[oldest_idx]) / 1000.0f;
    if (dt < 0.1f) return 0.0f;
    return (h->values[latest_idx] - h->values[oldest_idx]) / dt;
}

void temporal_analyzer_init(void) {
    memset(&s_hist_o2, 0, sizeof(s_hist_o2));
    memset(&s_hist_h2s, 0, sizeof(s_hist_h2s));
    memset(&s_hist_co, 0, sizeof(s_hist_co));
    memset(&s_hist_ch4, 0, sizeof(s_hist_ch4));
}

void temporal_analyzer_set_config(const temporal_config_t *config) {
    if (config) s_cfg = *config;
}

void temporal_analyzer_update(const multi_sensor_snapshot_t *sample, temporal_prediction_t *out) {
    if (!sample || !out) return;

    push_history(&s_hist_o2, sample->o2_pct, sample->timestamp_ms);
    push_history(&s_hist_h2s, sample->h2s_ppm, sample->timestamp_ms);
    push_history(&s_hist_co, sample->co_ppm, sample->timestamp_ms);
    push_history(&s_hist_ch4, sample->ch4_lel_pct, sample->timestamp_ms);

    float r_o2 = calc_rate(&s_hist_o2);
    float r_h2s = calc_rate(&s_hist_h2s);
    float r_co = calc_rate(&s_hist_co);
    float r_ch4 = calc_rate(&s_hist_ch4);

    out->rate_o2 = r_o2;
    out->rate_h2s = r_h2s;
    out->rate_co = r_co;
    out->rate_ch4 = r_ch4;

    /* Time to threshold calculations */
    /* For O2: hazard occurs when O2 is DECREASING (r_o2 < 0) below threshold_o2_min */
    if (r_o2 < -0.001f && sample->o2_pct > s_cfg.threshold_o2_min) {
        out->t_to_threshold_o2 = (sample->o2_pct - s_cfg.threshold_o2_min) / (-r_o2);
    } else {
        out->t_to_threshold_o2 = INFINITY;
    }

    /* For toxic gases: hazard occurs when gas is INCREASING (rate > 0) */
    if (r_h2s > 0.001f && sample->h2s_ppm < s_cfg.threshold_h2s_max) {
        out->t_to_threshold_h2s = (s_cfg.threshold_h2s_max - sample->h2s_ppm) / r_h2s;
    } else {
        out->t_to_threshold_h2s = INFINITY;
    }

    if (r_co > 0.001f && sample->co_ppm < s_cfg.threshold_co_max) {
        out->t_to_threshold_co = (s_cfg.threshold_co_max - sample->co_ppm) / r_co;
    } else {
        out->t_to_threshold_co = INFINITY;
    }

    if (r_ch4 > 0.001f && sample->ch4_lel_pct < s_cfg.threshold_ch4_max) {
        out->t_to_threshold_ch4 = (s_cfg.threshold_ch4_max - sample->ch4_lel_pct) / r_ch4;
    } else {
        out->t_to_threshold_ch4 = INFINITY;
    }

    /* Check pre-warning triggers */
    out->pre_warning_active = false;
    out->leading_hazard = "None";

    if (out->t_to_threshold_o2 < s_cfg.early_warning_sec) {
        out->pre_warning_active = true;
        out->leading_hazard = "Rapid Oxygen Depletion";
    } else if (out->t_to_threshold_h2s < s_cfg.early_warning_sec) {
        out->pre_warning_active = true;
        out->leading_hazard = "Rising H2S Surge";
    } else if (out->t_to_threshold_co < s_cfg.early_warning_sec) {
        out->pre_warning_active = true;
        out->leading_hazard = "Rising CO Surge";
    } else if (out->t_to_threshold_ch4 < s_cfg.early_warning_sec) {
        out->pre_warning_active = true;
        out->leading_hazard = "Combustible Gas Buildup";
    }
}
