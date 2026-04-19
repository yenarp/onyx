#ifndef STAGE2_ISR_H
#define STAGE2_ISR_H

#include <stdint.h>

typedef struct {
    uint32_t edi, esi, ebp, esp_saved, ebx, edx, ecx, eax;
    uint32_t vector, error_code;
    uint32_t eip, cs, eflags;
} InterruptFrame;

typedef void (*isr_handler_t)(InterruptFrame *frame);

void isr_dispatch(InterruptFrame *frame);

#endif /* STAGE2_ISR_H */
