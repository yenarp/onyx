#include "log.h"
#include "dev/serial.h"

static SerialPort *g_log;

void log_str(const char *s) {
    if (g_log)
        serial_puts(g_log, s);
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
