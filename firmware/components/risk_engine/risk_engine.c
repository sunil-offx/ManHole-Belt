/**
 * @file risk_engine.c
 * @brief Mathematical Risk Engine implementation with piecewise hazard mappings and emergency override.
 */

#include "risk_engine.h"
#include <string.h>

static risk_engine_config_t s_config = {
    .weight_o2 = 0.30f,
    .weight_h2s = 0.25f,
    .weight_co = 0.15f,
    .weight_ch4 = 0.15f,
    .weight_water = 0.10f,
    .weight_motion = 0.05f,
    .threshold_safe_max = 30.0f,
    .threshold_warning_max = 60.0f,
    .o2_override_threshold = 19.5f
};

void risk_engine_init(void) {
    /* Baseline configuration loaded */
}

void risk_engine_set_config(const risk_engine_config_t *config) {
    if (config) {
        s_config = *config;
    }
}

void risk_engine_get_config(risk_engine_config_t *config) {
    if (config) {
        *config = s_config;
    }
}

float risk_calc_oxygen(float o2) {
    /* 
     * Safe: >= 19.5%
     * Warning: 16.0% to 19.5%
     * Critical: <= 16.0% (asphyxiation danger)
     */
    if (o2 >= 19.5f) return 0.0f;
    else if (o2 > 16.0f) return (19.5f - o2) / (19.5f - 16.0f);
    else return 1.0f;
}

float risk_calc_h2s(float h2s) {
    /* Thresholds: 0ppm = 0, 10ppm = 0.5 (PEL), 20ppm = 1.0 (Critical) */
    if (h2s <= 0.0f) return 0.0f;
    else if (h2s < 10.0f) return h2s / 20.0f;
    else if (h2s < 20.0f) return 0.5f + (h2s - 10.0f) / 20.0f;
    else return 1.0f;
}

float risk_calc_co(float co) {
    /* Thresholds: 0ppm = 0, 25ppm = 0.5 (OSHA action), 50ppm = 1.0 */
    if (co <= 0.0f) return 0.0f;
    else if (co < 25.0f) return co / 50.0f;
    else if (co < 50.0f) return 0.5f + (co - 25.0f) / 50.0f;
    else return 1.0f;
}

float risk_calc_ch4(float ch4_lel) {
    /* % LEL: 0% = 0, 10% = 0.5, 25% = 1.0 */
    if (ch4_lel <= 0.0f) return 0.0f;
    else if (ch4_lel < 10.0f) return ch4_lel / 20.0f;
    else if (ch4_lel < 25.0f) return 0.5f + (ch4_lel - 10.0f) / 30.0f;
    else return 1.0f;
}

float risk_calc_water(float water_pct) {
    /* Rising water hazard: 0% = 0, 30% = 0.5, 60% = 1.0 */
    if (water_pct <= 0.0f) return 0.0f;
    else if (water_pct < 30.0f) return water_pct / 60.0f;
    else if (water_pct < 60.0f) return 0.5f + (water_pct - 30.0f) / 60.0f;
    else return 1.0f;
}

float risk_calc_motion(float inactivity_sec, bool fall_detected) {
    if (fall_detected) return 1.0f;
    if (inactivity_sec > 60.0f) return 1.0f;
    if (inactivity_sec > 30.0f) return 0.5f;
    return 0.0f;
}

risk_status_t risk_engine_evaluate(const multi_sensor_snapshot_t *sensors,
                                  float inactivity_sec,
                                  bool fall_detected,
                                  risk_assessment_t *out_assessment) {
    if (!sensors) return RISK_SAFE;

    float s_o2    = risk_calc_oxygen(sensors->o2_pct);
    float s_h2s   = risk_calc_h2s(sensors->h2s_ppm);
    float s_co    = risk_calc_co(sensors->co_ppm);
    float s_ch4   = risk_calc_ch4(sensors->ch4_lel_pct);
    float s_water = risk_calc_water(sensors->water_level_pct);
    float s_motion = risk_calc_motion(inactivity_sec, fall_detected);

    float overall_risk = 100.0f * (
        s_config.weight_o2    * s_o2 +
        s_config.weight_h2s   * s_h2s +
        s_config.weight_co    * s_co +
        s_config.weight_ch4   * s_ch4 +
        s_config.weight_water * s_water +
        s_config.weight_motion* s_motion
    );

    risk_status_t status;
    bool emergency_override = false;

    /* Emergency Override Rule: immediate Critical if O2 is below safe minimum */
    if (sensors->o2_pct < s_config.o2_override_threshold) {
        status = RISK_CRITICAL;
        emergency_override = true;
    } else if (fall_detected) {
        status = RISK_CRITICAL;
        emergency_override = true;
    } else if (overall_risk < s_config.threshold_safe_max) {
        status = RISK_SAFE;
    } else if (overall_risk < s_config.threshold_warning_max) {
        status = RISK_WARNING;
    } else {
        status = RISK_CRITICAL;
    }

    if (out_assessment) {
        out_assessment->s_o2 = s_o2;
        out_assessment->s_h2s = s_h2s;
        out_assessment->s_co = s_co;
        out_assessment->s_ch4 = s_ch4;
        out_assessment->s_water = s_water;
        out_assessment->s_motion = s_motion;
        out_assessment->composite_score = overall_risk;
        out_assessment->status = status;
        out_assessment->emergency_override_active = emergency_override;
    }

    return status;
}
