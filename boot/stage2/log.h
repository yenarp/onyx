#ifndef STAGE2_LOG_H
#define STAGE2_LOG_H

#include "dev/serial.h"

void log_set_sinker(SerialPort *port);

void log_str(const char *s);
void log_hex32(uint32_t v);

#endif /* STAGE2_LOG_H */
