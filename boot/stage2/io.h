#ifndef STAGE2_IO_H
#define STAGE2_IO_H

#include <stdint.h>

static inline void outb(uint16_t port, uint8_t val) {
    __asm__ volatile("out dx, al" : : "a"(val), "d"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t val;
    __asm__ volatile("in al, dx" : "=a"(val) : "d"(port));
    return val;
}

static inline void outw(uint16_t port, uint16_t val) {
    __asm__ volatile("out dx, ax" : : "a"(val), "d"(port));
}

static inline uint16_t inw(uint16_t port) {
    uint16_t val;
    __asm__ volatile("in ax, dx" : "=a"(val) : "d"(port));
    return val;
}

static inline void outl(uint16_t port, uint32_t val) {
    __asm__ volatile("out dx, eax" : : "a"(val), "d"(port));
}

static inline uint32_t inl(uint16_t port) {
    uint32_t val;
    __asm__ volatile("in eax, dx" : "=a"(val) : "d"(port));
    return val;
}

static inline void io_wait(void) {
    outb(0x80, 0);
}

#endif /* STAGE2_IO_H */
