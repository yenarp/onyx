BUILD_MODE_LC := $(shell printf '%s' '$(BUILD_MODE)' | tr '[:upper:]' '[:lower:]')

ifeq ($(BUILD_MODE_LC),release)
  BUILD_MODE_CANON := Release
  CC_MODE_FLAGS    := -Os -DNDEBUG
  FASMG_MODE_LINE  := BUILD_MODE_RELEASE = 1
else ifeq ($(BUILD_MODE_LC),debug)
  BUILD_MODE_CANON := Debug
  CC_MODE_FLAGS    := -O0 -g -DDEBUG
  FASMG_MODE_LINE  := BUILD_MODE_DEBUG = 1
else
  $(warning unknown BUILD_MODE = '$(BUILD_MODE)')
  BUILD_MODE_CANON := Debug
  CC_MODE_FLAGS    := -O0 -g -DDEBUG
  FASMG_MODE_LINE  := BUILD_MODE_DEBUG = 1
endif
