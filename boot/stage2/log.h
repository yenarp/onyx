#ifndef STAGE2_LOG_H
#define STAGE2_LOG_H

#include "dev/serial.h"

void log_set_sinker(SerialPort *port);

void log_str(const char *s);
void log_u32(uint32_t v);
void log_hex32(uint32_t v);

#define assert(cond)                                                                               \
    do {                                                                                           \
        if (!(cond))                                                                               \
            log_assert_fail(#cond, __FILE__, __LINE__);                                            \
    } while (0)

void log_assert_fail(const char *expr, const char *file, uint32_t line);

#endif /* STAGE2_LOG_H */
