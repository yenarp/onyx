ISR_STUB_SIZE = 24

macro emit_isr_stub vec*
@@:
    if vec = 8 | vec = 10 | vec = 11 | vec = 12 | vec = 13 | vec = 14 | vec = 17 | vec = 21
    else
        push 0
    end if
    push vec

    pusha

    push esp
    call isr_dispatch
    add esp, 4

    popa

    add esp, 8
    iret

rb ISR_STUB_SIZE - ($ - @b)
end macro

isr_stubs:
iterate vec, \
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15, \
    16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, \
    32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47
    emit_isr_stub vec
end iterate

macro emit_idt_entry vec*
    local addr
    addr = isr_stubs + vec * ISR_STUB_SIZE

    dw addr and 0xffff
    dw 0x8
    db 0
    db 10001110b
    dw (addr shr 16) and 0xffff
end macro

idt_start:
iterate vec, \
    0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15, \
    16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31, \
    32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47
    emit_idt_entry vec
end iterate
idt_end:
