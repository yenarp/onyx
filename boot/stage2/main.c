#define VGA_ADDR  0xb8000
#define VGA_COLS  80
#define VGA_ROWS  25

static unsigned short *const vga = (unsigned short *)VGA_ADDR;

#define vga_entry(c, col) ((c) | ((col) << 8))

static void clear_screen(unsigned char color) {
    for (unsigned i = 0; i < VGA_COLS * VGA_ROWS; i++)
        vga[i] = vga_entry(' ', color);
}

static void print(const char *str, unsigned row, unsigned col, unsigned char color) {
    unsigned offset = row * VGA_COLS + col;
    for (unsigned i = 0; str[i] != '\0'; i++)
        vga[offset + i] = vga_entry(str[i], color);
}

void kernel_main(void) {
    clear_screen(0x7);
    print("Hello, World!", 0, 0, 0xf);

    for (;;)
        __asm__ volatile ("hlt");
}
