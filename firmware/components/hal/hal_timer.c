/**
 * @file hal_timer.c
 * @brief Platform timer and sleep abstraction implementation.
 */

#include "hal_timer.h"

#ifdef ESP_PLATFORM
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

uint32_t hal_get_time_ms(void) {
    return (uint32_t)(esp_timer_get_time() / 1000ULL);
}

uint64_t hal_get_time_us(void) {
    return (uint64_t)esp_timer_get_time();
}

void hal_delay_ms(uint32_t ms) {
    vTaskDelay(pdMS_TO_TICKS(ms));
}

void hal_delay_us(uint32_t us) {
    esp_rom_delay_us(us);
}

#else
/* Native host implementation */
#include <time.h>

#if defined(_WIN32)
#include <windows.h>

uint32_t hal_get_time_ms(void) {
    return (uint32_t)GetTickCount();
}

uint64_t hal_get_time_us(void) {
    LARGE_INTEGER freq, counter;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&counter);
    return (uint64_t)((counter.QuadPart * 1000000ULL) / freq.QuadPart);
}

void hal_delay_ms(uint32_t ms) {
    Sleep(ms);
}

void hal_delay_us(uint32_t us) {
    /* Rough microsecond sleep */
    LARGE_INTEGER freq, start, cur;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&start);
    uint64_t ticks_to_wait = (us * freq.QuadPart) / 1000000ULL;
    do {
        QueryPerformanceCounter(&cur);
    } while ((uint64_t)(cur.QuadPart - start.QuadPart) < ticks_to_wait);
}

#else
#include <unistd.h>

uint32_t hal_get_time_ms(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint32_t)(ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
}

uint64_t hal_get_time_us(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)(ts.tv_sec * 1000000ULL + ts.tv_nsec / 1000);
}

void hal_delay_ms(uint32_t ms) {
    usleep(ms * 1000);
}

void hal_delay_us(uint32_t us) {
    usleep(us);
}
#endif

#endif
