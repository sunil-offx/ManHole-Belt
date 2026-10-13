/**
 * @file model_weights.h
 * @brief Auto-generated C Header for SENTINEL-BELT AI Inference Engine.
 * Generated from Random Forest model trained on confined space safety dataset.
 */

#ifndef MODEL_WEIGHTS_H
#define MODEL_WEIGHTS_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ML_FEATURE_COUNT 14

/* Feature index mapping */
#define FEAT_O2          0
#define FEAT_H2S         1
#define FEAT_CO          2
#define FEAT_CH4         3
#define FEAT_TEMP        4
#define FEAT_HUM         5
#define FEAT_WATER       6
#define FEAT_AX          7
#define FEAT_AY          8
#define FEAT_AZ          9
#define FEAT_DO2_DT     10
#define FEAT_DH2S_DT    11
#define FEAT_DCO_DT     12
#define FEAT_DCH4_DT    13

typedef enum {
    ML_CLASS_SAFE = 0,
    ML_CLASS_WARNING = 1,
    ML_CLASS_CRITICAL = 2
} ml_risk_class_t;

/**
 * @brief Evaluates Random Forest classifier on 14-element sensor feature vector.
 * @param features Array of 14 float sensor readings and derivatives.
 * @return ml_risk_class_t predicted class (SAFE, WARNING, CRITICAL)
 */
ml_risk_class_t sentinel_ml_predict_risk(const float features[ML_FEATURE_COUNT]);

/**
 * @brief Fast decision-boundary anomaly detector.
 * @param features Array of 14 float sensor readings and derivatives.
 * @return bool true if anomalous pattern detected, false if normal.
 */
bool sentinel_ml_detect_anomaly(const float features[ML_FEATURE_COUNT]);

#ifdef __cplusplus
}
#endif

#endif /* MODEL_WEIGHTS_H */
