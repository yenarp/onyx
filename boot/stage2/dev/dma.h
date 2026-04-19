#ifndef STAGE2_DEV_DMA_H
#define STAGE2_DEV_DMA_H

#include <stddef.h>
#include <stdint.h>

typedef enum {
    DMA_DIR_READ,
    DMA_DIR_WRITE,
} DmaDirection;

void dma_prepare(uint8_t channel, uint32_t phys, uint32_t len, DmaDirection dir);

void dma_mask(uint8_t channel);
void dma_unmask(uint8_t channel);

#endif /* STAGE2_DEV_DMA_H */
