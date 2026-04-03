CC := clang
OBJCOPY ?= llvm-objcopy

CC_TARGET := --target=i386-none-elf -std=gnu23
CC_FSTAND := -ffreestanding -nostdlib -nostdinc -fno-builtin -masm=intel
CC_HARDEN := -fno-stack-protector -fno-exceptions -fno-unwind-tables -fno-asynchronous-unwind-tables
CC_CODEGEN  := -mno-sse -mno-sse2 -mno-mmx -mno-80387 -fno-pie -fno-pic
CC_WARNINGS := -Wall -Wextra -Wpedantic -Werror=implicit-function-declaration -Werror=return-type

CC_MODE_FLAGS ?=

CC_EXTRA_FLAGS ?=

CFLAGS = $(CC_TARGET) $(CC_FSTAND) $(CC_HARDEN) $(CC_CODEGEN) $(CC_WARNINGS) $(CC_MODE_FLAGS) $(CC_EXTRA_FLAGS)

DEPFLAGS = -MD -MF $(@:.s=.d)
