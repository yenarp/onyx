#ifndef STAGE2_MEM_LOWBUF_H
#define STAGE2_MEM_LOWBUF_H

#include <stddef.h>
#include <stdint.h>

#define LOWBUF_SIZE 9216u

void *lowbuf_ptr(void);
uint32_t lowbuf_phys(void);

#endif /* STAGE2_MEM_LOWBUF_H */
