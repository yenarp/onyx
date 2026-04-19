#include "log.h"
#include "dev/serial.h"

static SerialPort *g_log;

void log_str(const char *s) {
    if (g_log)
        serial_puts(g_log, s);
}

void log_u32(uint32_t v) {
    if (v == 0) {
        log_str("0");
        return;
    }

    char buf[11];
    int i = 10;
    buf[i--] = '\0';

    while (v > 0) {
        buf[i--] = (v % 10) + '0';
        v /= 10;
    }

    log_str(&buf[i + 1]);
}

void log_hex32(uint32_t v) {
    static constexpr char hex[] = "0123456789abcdef";

    char buf[11] = "0x00000000";
    for (int i = 0; i < 8; i++) {
        buf[9 - i] = hex[v & 0xf];
        v >>= 4;
    }

    log_str(buf);
}

void log_set_sinker(SerialPort *port) {
    g_log = port;
}

void log_assert_fail(const char *expr, const char *file, uint32_t line) {
    log_str("\r\n!!! ASSERTION FAILED: ");
    log_str(expr);
    log_str("\r\n!!! Location: ");
    log_str(file);
    log_str(":");
    log_u32(line);
    log_str("\r\n");

    while (1)
        __asm__ volatile("cli; hlt");
}