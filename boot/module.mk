BOOT_DIR := $(ROOT)/boot
BOOT_SECT_DIR := $(BOOT_DIR)/sector
STAGE2_SRC_DIR := $(BOOT_DIR)/stage2

PREPROC_ASM := $(ROOT)/tools/preproc_asm.sh

BOOT_SECT_SRCS := $(wildcard $(BOOT_SECT_DIR)/*.asm)
BOOT_SECT_BINS := $(patsubst $(BOOT_SECT_DIR)/%.asm, $(BUILD_DIR)/boot/sector/%.bin, $(BOOT_SECT_SRCS))

$(BUILD_DIR)/boot/sector/%.bin: $(BOOT_SECT_DIR)/%.asm $(PRELUDE)
	$(call fasmg_compile,$<,$@,$(BOOT_SECT_DIR))

STAGE2_C_SRCS := $(wildcard $(STAGE2_SRC_DIR)/*.c)
STAGE2_S_OUTS := $(patsubst $(STAGE2_SRC_DIR)/%.c, $(BUILD_DIR)/boot/stage2/%.s, $(STAGE2_C_SRCS))
STAGE2_D_DEPS := $(STAGE2_S_OUTS:.s=.d)

STAGE2_C_ASM  := $(BUILD_DIR)/boot/stage2/c.asm
STAGE2_ASM := $(STAGE2_SRC_DIR)/stage2.asm
STAGE2_BIN := $(BUILD_DIR)/boot/stage2/stage2.bin

STAGE2_CFLAGS = $(CFLAGS) -I$(STAGE2_SRC_DIR)

$(BUILD_DIR)/boot/stage2/%.s: $(STAGE2_SRC_DIR)/%.c $(PREPROC_ASM)
	$(call mkdir_p,$(dir $@))
	$(CC) $(STAGE2_CFLAGS) $(DEPFLAGS) -S -o - $< | sh $(PREPROC_ASM) > $@

$(STAGE2_C_ASM): $(STAGE2_S_OUTS)
	$(call mkdir_p,$(dir $@))
	@echo '; auto-generated - do not edit' > $@
	$(foreach s,$(notdir $(STAGE2_S_OUTS)),@echo "include '$(s)'" >> $@;)

$(STAGE2_BIN): $(STAGE2_ASM) $(STAGE2_C_ASM) $(PRELUDE)
	$(call fasmg_compile,$<,$@,$(abspath $(BUILD_DIR)/boot/stage2))

-include $(STAGE2_D_DEPS)

.PHONY: boot-sectors stage2

boot-sectors: $(BOOT_SECT_BINS)
stage2: $(STAGE2_BIN)
