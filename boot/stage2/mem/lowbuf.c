#include "mem/lowbuf.h"

extern uint8_t lowbuf_storage[LOWBUF_SIZE];

void *lowbuf_ptr(void) {
    return lowbuf_storage;
}

uint32_t lowbuf_phys(void) {
    return (uintptr_t)lowbuf_storage;
}
