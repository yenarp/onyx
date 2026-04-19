#include "dev/pic.h"
#include "io.h"

#define PIC_M_CMD 0x20
#define PIC_M_DATA 0x21
#define PIC_S_CMD 0xa0
#define PIC_S_DATA 0xa1

#define ICW1_INIT 0x10
#define ICW1_ICW4 0x1
#define ICW4_8086 0x1
#define PIC_EOI 0x20

void pic_init(void) {
    outb(PIC_M_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();
    outb(PIC_S_CMD, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC_M_DATA, PIC_IRQ_BASE);
    io_wait();
    outb(PIC_S_DATA, PIC_IRQ_BASE + 8);
    io_wait();

    outb(PIC_M_DATA, 0x4);
    io_wait();
    outb(PIC_S_DATA, 0x2);
    io_wait();

    outb(PIC_M_DATA, ICW4_8086);
    io_wait();
    outb(PIC_S_DATA, ICW4_8086);
    io_wait();

    outb(PIC_M_DATA, 0xff);
    outb(PIC_S_DATA, 0xff);
}

void pic_disable(void) {
    outb(PIC_M_DATA, 0xff);
    outb(PIC_S_DATA, 0xff);
}

void pic_mask(uint8_t irq) {
    uint16_t port = (irq < 8) ? PIC_M_DATA : PIC_S_DATA;
    uint8_t bit = (irq < 8) ? irq : irq - 8;

    outb(port, inb(port) | (1u << bit));
}

void pic_unmask(uint8_t irq) {
    uint16_t port = (irq < 8) ? PIC_M_DATA : PIC_S_DATA;
    uint8_t bit = (irq < 8) ? irq : irq - 8;

    outb(port, inb(port) & ~(1u << bit));

    /* unmask the cascade line on the master */
    if (irq >= 8)
        outb(PIC_M_DATA, inb(PIC_M_DATA) & ~(1u << 2));
}

void pic_send_eoi(uint8_t irq) {
    if (irq >= 8)
        outb(PIC_S_CMD, PIC_EOI);

    outb(PIC_M_CMD, PIC_EOI);
}
