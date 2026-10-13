/**
 * @file hal_timer.h
 * @brief Platform timer and sleep abstraction.
 */

#ifndef HAL_TIMER_H
#define HAL_TIMER_H

#include "hal_types.h"

#ifdef __cplusplus
extern "C" {
#endif

uint32_t hal_get_time_ms(void);
uint64_t hal_get_time_us(void);
void hal_delay_ms(uint32_t ms);
void hal_delay_us(uint32_t us);

#ifdef __cplusplus
}
#endif

#endif /* HAL_TIMER_H */
