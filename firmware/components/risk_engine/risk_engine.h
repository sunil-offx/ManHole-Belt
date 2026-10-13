/**
 * @file risk_engine.h
 * @brief Mathematical Risk Engine for multi-gas, water, and biomechanical hazards.
 */

#ifndef RISK_ENGINE_H
#define RISK_ENGINE_H

#include "sensor_types.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    RISK_SAFE = 0,
    RISK_WARNING = 1,
    RISK_CRITICAL = 2,
    RISK_EMERGENCY_SOS = 3
} risk_status_t;

typedef struct {
    float weight_o2;   /* Default: 0.30 */
    float weight_h2s;  /* Default: 0.25 */
    float weight_co;   /* Default: 0.15 */
    float weight_ch4;  /* Default: 0.15 */
    float weight_water;/* Default: 0.10 */
    float weight_motion;/* Default: 0.05 */
    
    float threshold_safe_max;    /* Default: 30.0 */
    float threshold_warning_max; /* Default: 60.0 */
    float o2_override_threshold; /* Default: 19.5 */
} risk_engine_config_t;

typedef struct {
    float s_o2;
    float s_h2s;
    float s_co;
    float s_ch4;
    float s_water;
    float s_motion;
    float composite_score; /* 0.0 to 100.0 */
    risk_status_t status;
    bool emergency_override_active;
} risk_assessment_t;

void risk_engine_init(void);
void risk_engine_set_config(const risk_engine_config_t *config);
void risk_engine_get_config(risk_engine_config_t *config);

float risk_calc_oxygen(float o2_pct);
float risk_calc_h2s(float h2s_ppm);
float risk_calc_co(float co_ppm);
float risk_calc_ch4(float ch4_lel);
float risk_calc_water(float water_pct);
float risk_calc_motion(float inactivity_sec, bool fall_detected);

risk_status_t risk_engine_evaluate(const multi_sensor_snapshot_t *sensors,
                                  float inactivity_sec,
                                  bool fall_detected,
                                  risk_assessment_t *out_assessment);

#ifdef __cplusplus
}
#endif

#endif /* RISK_ENGINE_H */
