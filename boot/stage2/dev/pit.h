#ifndef STAGE2_DEV_PIT_H
#define STAGE2_DEV_PIT_H

#include "isr.h"
#include <stdint.h>

#define PIT_INPUT_HZ 1193182u

void pit_init(uint32_t hz);

uint32_t pit_ticks(void);

void pit_sleep_ms(uint32_t ms);
void pit_oneshot_us(uint32_t us);

void pit_irq_handler(InterruptFrame *frame);

#endif /* STAGE2_DEV_PIT_H */
