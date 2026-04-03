#include "dev/serial.h"

void kernel_main(void) {
    SerialPort com1;

    serial_init(&com1, 0x3F8, SERIAL_BAUD_115200);
    serial_puts(&com1, "Hello World from serial!\n");

    for (;;)
        __asm__ volatile("hlt");
}
