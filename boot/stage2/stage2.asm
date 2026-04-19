org 0x500

    mov bp, sp
    sub bp, 6

    ; prepare the gdtr on the stack.
    mov word [bp], gdt_end - gdt_start - 1
    mov dword [bp + 2], gdt_start

    ; clear IF and load the global descriptor table.
    cli
    lgdt [bp]

    ; set the protected mode bit.
    mov eax, cr0
    or al, 1
    mov cr0, eax

    ; far jump to flush pipeline and load CS with code selector.
    jmp 0x8:pm_entry

use32

pm_entry:
    ; load data segments.
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax

    ; load stack segment and prepare stack.
    mov ax, 0x18
    mov ss, ax
    mov esp, 0x7fffe

    ; fast a20 gate enable.
    in al, 0x92
    or al, 2
    out 0x92, al

    mov esp, esp
    sub ebp, 6

    ; prepare an idtr on the stack.
    mov word [ebp], idt_end - idt_start - 1
    mov dword [ebp + 2], idt_start

    ; load the interrupt descriptor table.
    lidt [ebp]

    ; initialize the programmable interrupt controller.
    call pic_init

    sti

    jmp stage2_main

gdt_start:
    dq 0x0

    dw 0xffff
    dw 0x0000
    db 0x00
    db 10011010b
    db 11001111b
    db 0x00

    dw 0xffff
    dw 0x0000
    db 0x00
    db 10010010b
    db 11001111b
    db 0x00

    dw 0x0000
    dw 0x0000
    db 0x00
    db 10010110b
    db 11000000b
    db 0x00
gdt_end:

include 'c.asm'
include 'isr.asm'

lowbuf_storage: rb 16384
