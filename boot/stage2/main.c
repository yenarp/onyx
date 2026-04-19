#include "dev/pit.h"
#include "log.h"

void stage2_main(void) {
    SerialPort com1;

    serial_init(&com1, 0x3f8, SERIAL_BAUD_115200);
    log_set_sinker(&com1);

    log_str("Hello World!\n");
    pit_init(1000);

    pit_sleep_ms(2000);
    serial_puts(&com1, "2s via pit_sleep_ms\n");

    pit_oneshot_us(50000);
    serial_puts(&com1, "50ms via pit_oneshot_us\n");

    for (;;)
        __asm__ volatile("hlt");
}
