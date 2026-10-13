/**
 * @file filters.h
 * @brief Digital signal processing filters: Moving Average, Median, and 1D Kalman filter.
 */

#ifndef FILTERS_H
#define FILTERS_H

#include <stddef.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_FILTER_WINDOW 16

/* 1. Moving Average Filter */
typedef struct {
    float buffer[MAX_FILTER_WINDOW];
    size_t size;
    size_t index;
    size_t count;
    float sum;
} moving_avg_filter_t;

void moving_avg_init(moving_avg_filter_t *filter, size_t size);
float moving_avg_update(moving_avg_filter_t *filter, float sample);

/* 2. Median Filter for gas sensor spike rejection */
typedef struct {
    float buffer[MAX_FILTER_WINDOW];
    size_t size;
    size_t index;
    size_t count;
} median_filter_t;

void median_filter_init(median_filter_t *filter, size_t size);
float median_filter_update(median_filter_t *filter, float sample);

/* 3. 1D Kalman Filter */
typedef struct {
    float q; /* Process noise covariance */
    float r; /* Measurement noise covariance */
    float x; /* Value state estimate */
    float p; /* Estimation error covariance */
    float k; /* Kalman gain */
} kalman_filter_1d_t;

void kalman_filter_init(kalman_filter_1d_t *kf, float q, float r, float initial_value);
float kalman_filter_update(kalman_filter_1d_t *kf, float measurement);

#ifdef __cplusplus
}
#endif

#endif /* FILTERS_H */
