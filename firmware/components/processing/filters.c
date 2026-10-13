/**
 * @file filters.c
 * @brief Implementations for Moving Average, Median, and 1D Kalman filters.
 */

#include "filters.h"
#include <string.h>

void moving_avg_init(moving_avg_filter_t *filter, size_t size) {
    if (!filter) return;
    if (size > MAX_FILTER_WINDOW) size = MAX_FILTER_WINDOW;
    if (size == 0) size = 1;
    filter->size = size;
    filter->index = 0;
    filter->count = 0;
    filter->sum = 0.0f;
    memset(filter->buffer, 0, sizeof(filter->buffer));
}

float moving_avg_update(moving_avg_filter_t *filter, float sample) {
    if (!filter || filter->size == 0) return sample;

    if (filter->count < filter->size) {
        filter->buffer[filter->index] = sample;
        filter->sum += sample;
        filter->count++;
    } else {
        filter->sum -= filter->buffer[filter->index];
        filter->buffer[filter->index] = sample;
        filter->sum += sample;
    }

    filter->index = (filter->index + 1) % filter->size;
    return filter->sum / (float)filter->count;
}

void median_filter_init(median_filter_t *filter, size_t size) {
    if (!filter) return;
    if (size > MAX_FILTER_WINDOW) size = MAX_FILTER_WINDOW;
    if (size == 0) size = 1;
    filter->size = size;
    filter->index = 0;
    filter->count = 0;
    memset(filter->buffer, 0, sizeof(filter->buffer));
}

static void insertion_sort(float arr[], size_t n) {
    for (size_t i = 1; i < n; i++) {
        float key = arr[i];
        int j = (int)i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

float median_filter_update(median_filter_t *filter, float sample) {
    if (!filter || filter->size == 0) return sample;

    filter->buffer[filter->index] = sample;
    filter->index = (filter->index + 1) % filter->size;
    if (filter->count < filter->size) filter->count++;

    float temp[MAX_FILTER_WINDOW];
    memcpy(temp, filter->buffer, filter->count * sizeof(float));
    insertion_sort(temp, filter->count);

    if (filter->count % 2 == 1) {
        return temp[filter->count / 2];
    } else {
        return 0.5f * (temp[(filter->count / 2) - 1] + temp[filter->count / 2]);
    }
}

void kalman_filter_init(kalman_filter_1d_t *kf, float q, float r, float initial_value) {
    if (!kf) return;
    kf->q = q;
    kf->r = r;
    kf->x = initial_value;
    kf->p = 1.0f;
    kf->k = 0.0f;
}

float kalman_filter_update(kalman_filter_1d_t *kf, float measurement) {
    if (!kf) return measurement;

    /* Time update (prediction) */
    kf->p = kf->p + kf->q;

    /* Measurement update (correction) */
    kf->k = kf->p / (kf->p + kf->r);
    kf->x = kf->x + kf->k * (measurement - kf->x);
    kf->p = (1.0f - kf->k) * kf->p;

    return kf->x;
}
