#ifndef STAGE2_DEV_PIC_H
#define STAGE2_DEV_PIC_H

#include <stdint.h>

#define PIC_IRQ_BASE 0x20
#define PIC_IRQ_COUNT 16

void pic_mask(uint8_t irq);
void pic_unmask(uint8_t irq);
void pic_send_eoi(uint8_t irq);

#endif /* STAGE2_DEV_PIC_H */
