#include "dev/serial.h"
#include "io.h"

#define REG_DATA 0 /* RX/TX buffer */
#define REG_IER 1  /* Interrupt enable */
#define REG_FCR 2  /* FIFO control */
#define REG_LCR 3  /* Line control */
#define REG_MCR 4  /* Modem control */
#define REG_LSR 5  /* Line status */

#define LSR_DR 0x1    /* Data ready */
#define LSR_THRE 0x20 /* Transmit holding register empty */

/* LCR */
#define LCR_DLAB 0x80
#define LCR_8N1 0x3

/* enable FIFOs, clear both, 14-byte threshold */
#define FCR_INIT 0xc7

/* DTR + RTS + OUT2 */
#define MCR_NORMAL 0xf
#define MCR_LOOP 0x1e

#define LOOP_TEST_BYTE 0xae

static void set_baud_divisor(uint16_t base, SerialBaud baud) {
    uint8_t lcr = inb(base + REG_LCR);
    outb(base + REG_LCR, lcr | LCR_DLAB);

    outb(base + REG_DATA, baud & 0xff);
    outb(base + REG_IER, (baud >> 8) & 0xff);

    outb(base + REG_LCR, lcr & ~LCR_DLAB);
}

static int loopback_test(uint16_t base) {
    outb(base + REG_MCR, MCR_LOOP);
    outb(base + REG_DATA, LOOP_TEST_BYTE);

    return inb(base + REG_DATA) == LOOP_TEST_BYTE ? 0 : 1;
}

int serial_init(SerialPort *port, uint16_t base, SerialBaud baud) {
    port->base = base;
    port->baud = baud;

    outb(base + REG_IER, 0x00);
    set_baud_divisor(base, baud);
    outb(base + REG_LCR, LCR_8N1);
    outb(base + REG_FCR, FCR_INIT);
    outb(base + REG_MCR, 0x0B);

    if (loopback_test(base))
        return 1;

    outb(base + REG_MCR, MCR_NORMAL);
    return 0;
}

int serial_set_baud(SerialPort *port, SerialBaud baud) {
    set_baud_divisor(port->base, baud);
    port->baud = baud;

    return 0;
}

void serial_putc(SerialPort *port, char c) {
    while (!(inb(port->base + REG_LSR) & LSR_THRE))
        ;

    outb(port->base + REG_DATA, c);
}

void serial_puts(SerialPort *port, const char *s) {
    while (*s)
        serial_putc(port, *s++);
}

int serial_data_ready(SerialPort *port) {
    return inb(port->base + REG_LSR) & LSR_DR;
}

char serial_getc(SerialPort *port) {
    while (!serial_data_ready(port))
        ;

    return inb(port->base + REG_DATA);
}

int serial_trygetc(SerialPort *port) {
    if (!serial_data_ready(port))
        return -1;

    return inb(port->base + REG_DATA);
}
