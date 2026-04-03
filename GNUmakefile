ROOT := $(abspath $(dir $(lastword $(MAKEFILE_LIST))))

include config.mk
include mk/tools.mk
include mk/mode.mk
include mk/fasmg.mk
include mk/cc.mk

include boot/module.mk

include mk/floppy.mk
include mk/run.mk

.DEFAULT_GOAL := all

.PHONY: all boot floppy run-floppy run-bochs-floppy clean reset

all: run-floppy

boot: boot-sectors stage2

floppy: boot

run-floppy: floppy
	$(call qemu_run_floppy)

run-bochs-floppy: floppy
	$(call bochs_run_floppy)

clean:
	$(call rm_rf,$(BUILD_DIR))

reset: clean
	$(call maybe_clear_screen)
	$(MAKE) all
