FASMG ?= fasmg

GLOBAL_INC_DIR := $(ROOT)/include
PRELUDE := $(GLOBAL_INC_DIR)/prelude.asm

IMG := $(BUILD_DIR)/a.img
BOOT_INI := $(wildcard $(ROOT)/BOOT.INI)

define fasmg_compile
	$(call require_tool,$(FASMG))
	$(call mkdir_p,$(dir $(2)))
	INCLUDE="$(GLOBAL_INC_DIR);$(3);$$INCLUDE" \
	"$(FASMG)" \
		-i "include '$(PRELUDE)'" \
		-i "$(FASMG_MODE_LINE)" \
		$(if $(strip $(4)),-i "$(FASMG_TRACE_LINE)",) \
		-n \
		"$(1)" "$(2)"
endef
