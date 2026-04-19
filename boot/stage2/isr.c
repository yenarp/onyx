#include "isr.h"
#include "dev/pic.h"
#include "dev/pit.h"
#include "log.h"

static const char *const exception_names[19] = {
    "#DE Divide Error",
    "#DB Debug",
    "NMI",
    "#BP Breakpoint",
    "#OF Overflow",
    "#BR BOUND Range",
    "#UD Invalid Opcode",
    "#NM Device Not Available",
    "#DF Double Fault",
    "Coprocessor Segment Overrun",
    "#TS Invalid TSS",
    "#NP Segment Not Present",
    "#SS Stack Fault",
    "#GP General Protection",
    "#PF Page Fault",
    "Reserved (15)",
    "#MF x87 FP Error",
    "#AC Alignment Check",
    "#MC Machine Check",
};

static const isr_handler_t irq_handlers[16] = {
    [0] = pit_irq_handler,
    /* TODO: other slots */
};

static void panic_exception(InterruptFrame *f) {
    log_str("\n!! EXCEPTION: ");
    if (f->vector < 19)
        log_str(exception_names[f->vector]);

    log_str("\n   vec = ");
    log_hex32(f->vector);
    log_str(" err = ");
    log_hex32(f->error_code);
    log_str("\n   eip = ");
    log_hex32(f->eip);
    log_str(" cs = ");
    log_hex32(f->cs);
    log_str(" eflags = ");
    log_hex32(f->eflags);
    log_str("\n   eax = ");
    log_hex32(f->eax);
    log_str(" ebx = ");
    log_hex32(f->ebx);
    log_str(" ecx = ");
    log_hex32(f->ecx);
    log_str(" edx = ");
    log_hex32(f->edx);
    log_str("\n   esi = ");
    log_hex32(f->esi);
    log_str(" edi = ");
    log_hex32(f->edi);
    log_str(" ebp = ");
    log_hex32(f->ebp);
    log_str("\n");

    for (;;)
        __asm__ volatile("cli; hlt");
}

void isr_dispatch(InterruptFrame *frame) {
    uint32_t vec = frame->vector;

    if (vec < 32) {
        panic_exception(frame);
        return;
    }

    if (vec < 48) {
        uint8_t irq = vec - 32;
        isr_handler_t h = irq_handlers[irq];
        if (h)
            h(frame);

        pic_send_eoi(irq);
        return;
    }
}
