#include "dev/pit.h"
#include "dev/pic.h"
#include "io.h"

#define PIT_CH0 0x40
#define PIT_CH2 0x42
#define PIT_CMD 0x43
#define SYSCTL_B 0x61 /* bit 0 = ch2 gate, bit 1 = speaker, bit 5 = OUT2 */

static volatile uint32_t tick_count;
static uint32_t tick_hz;

void pit_init(uint32_t hz) {
    if (hz == 0)
        hz = 1000;

    tick_hz = hz;

    uint32_t div = PIT_INPUT_HZ / hz;
    if (div == 0)
        div = 1;

    if (div > 0xffff)
        div = 0xffff;

    outb(PIT_CMD, 0x36);
    outb(PIT_CH0, div & 0xff);
    outb(PIT_CH0, (div >> 8) & 0xff);

    tick_count = 0;
    pic_unmask(0);
}

uint32_t pit_ticks(void) {
    return tick_count;
}

void pit_sleep_ms(uint32_t ms) {
    uint32_t target = pit_ticks() + (ms * tick_hz + 999) / 1000;
    while (pit_ticks() < target)
        __asm__ volatile("hlt");
}

void pit_oneshot_us(uint32_t us) {
    uint32_t div = ((us * PIT_INPUT_HZ) / 1000000u);
    if (div == 0)
        div = 1;

    if (div > 0xFFFF)
        div = 0xFFFF;

    uint8_t base = inb(SYSCTL_B) & 0xfc;
    outb(SYSCTL_B, base);

    outb(PIT_CMD, 0xb0);
    outb(PIT_CH2, div & 0xff);
    outb(PIT_CH2, (div >> 8) & 0xff);

    outb(SYSCTL_B, base | 0x1);

    while ((inb(SYSCTL_B) & 0x20) == 0)
        __asm__ volatile("pause");

    outb(SYSCTL_B, base);
}

void pit_irq_handler(InterruptFrame *frame) {
    (void)frame;
    tick_count++;
}
