#include "dev/dma.h"
#include "io.h"
#include "log.h"

static const uint16_t channel_addr_port[4] = {0x0, 0x2, 0x4, 0x6};
static const uint16_t channel_count_port[4] = {0x1, 0x3, 0x5, 0x7};

static const uint16_t channel_page_port[4] = {0x87, 0x83, 0x81, 0x82};

#define DMA1_MASK_PORT 0xa     /* single-channel mask register  */
#define DMA1_MODE_PORT 0xb     /* mode register                  */
#define DMA1_FLIPFLOP_PORT 0xc /* clear byte-pointer flip-flop   */

#define DMA_MASK_SET 0x4

#define DMA_MODE_SINGLE 0x40
#define DMA_MODE_XFER_WRITE_MEM 0x04 /* device -> memory  */
#define DMA_MODE_XFER_READ_MEM 0x08  /* memory -> device */

void dma_mask(uint8_t channel) {
    outb(DMA1_MASK_PORT, DMA_MASK_SET | (channel & 0x3));
}

void dma_unmask(uint8_t channel) {
    outb(DMA1_MASK_PORT, channel & 0x3);
}

void dma_prepare(uint8_t channel, uint32_t phys, uint32_t len, DmaDirection dir) {
    const uint32_t end_phys = phys + len - 1;

    assert(channel <= 3);
    assert(len >= 1 && len <= 65536);
    assert(end_phys < 0x1000000);
    assert((phys >> 16) == (end_phys >> 16));

    const uint8_t ch = channel & 0x3;

    const uint16_t addr_low = phys & 0xffffu;
    const uint8_t page = (phys >> 16) & 0xffu;

    const uint16_t count_reg = len - 1;

    dma_mask(ch);

    outb(DMA1_FLIPFLOP_PORT, 0);

    const uint8_t xfer = (dir == DMA_DIR_READ) ? DMA_MODE_XFER_WRITE_MEM : DMA_MODE_XFER_READ_MEM;
    outb(DMA1_MODE_PORT, DMA_MODE_SINGLE | xfer | ch);

    outb(channel_addr_port[ch], addr_low & 0xffu);
    outb(channel_addr_port[ch], (addr_low >> 8) & 0xffu);

    outb(channel_page_port[ch], page);
    outb(DMA1_FLIPFLOP_PORT, 0);

    outb(channel_count_port[ch], count_reg & 0xffu);
    outb(channel_count_port[ch], (count_reg >> 8) & 0xffu);

    dma_unmask(ch);
}
