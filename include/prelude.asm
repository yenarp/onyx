include 'fasm2/xcalm.asm'
include 'fasm2/p5.asm'
include 'fasm2/inlinemacro.asm'
include 'fasm2/@@.asm'
include 'fasm2/struct.asm'
include 'fasm2/times.asm'

macro pause
    db 0xF3, 0x90
end macro

use16
