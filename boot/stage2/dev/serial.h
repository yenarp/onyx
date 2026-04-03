#ifndef STAGE2_DEV_SERIAL_H
#define STAGE2_DEV_SERIAL_H

#include <stdint.h>

typedef enum {
    SERIAL_BAUD_115200 = 1,
    SERIAL_BAUD_57600 = 2,
    SERIAL_BAUD_38400 = 3,
    SERIAL_BAUD_19200 = 6,
    SERIAL_BAUD_9600 = 12,
} SerialBaud;

typedef struct {
    uint16_t base;
    SerialBaud baud;
} SerialPort;

int serial_init(SerialPort *port, uint16_t base, SerialBaud baud);
int serial_set_baud(SerialPort *port, SerialBaud baud);

void serial_putc(SerialPort *port, char c);
void serial_puts(SerialPort *port, const char *s);

int serial_data_ready(SerialPort *port);

char serial_getc(SerialPort *port);
int serial_trygetc(SerialPort *port);

#endif /* STAGE2_DEV_SERIAL_H */
