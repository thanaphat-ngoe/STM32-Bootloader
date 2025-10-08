# Be silent per default, but 'make V=1' will show all compiler calls.
ifneq ($(V),1)
Q      := @
NULL   := 2>/dev/null
endif

###############################################################################
# Directories (this Makefile lives in ./src)
SRC_DIR        = .
INC_DIR        = inc
OPENCM3_DIR    = ../libopencm3
SHARED_SRC_DIR = ../shared/src
SHARED_INC_DIR = ../shared/inc

BINARY = firmware

###############################################################################
# Device / Arch (STM32L053R8)
LIBNAME     = opencm3_stm32l0
DEFS       += -DSTM32L0
ARCH_FLAGS  = -mthumb -mcpu=cortex-m0plus

###############################################################################
# Linkerscript
LDSCRIPT  = linkerscript.ld
LDLIBS   += -l$(LIBNAME)
LDFLAGS  += -L$(OPENCM3_DIR)/lib

###############################################################################
# Includes
DEFS      += -I$(OPENCM3_DIR)/include
DEFS      += -I$(INC_DIR)
DEFS      += -I$(SHARED_INC_DIR)

###############################################################################
# Toolchain
PREFIX   ?= arm-none-eabi-

CC       := $(PREFIX)gcc
CXX      := $(PREFIX)g++
LD       := $(PREFIX)gcc
AR       := $(PREFIX)ar
AS       := $(PREFIX)as
OBJCOPY  := $(PREFIX)objcopy
OBJDUMP  := $(PREFIX)objdump
GDB      := $(PREFIX)gdb
STFLASH  := $(shell which st-flash)

OPT      := -Os
DEBUG    := -ggdb3
CSTD     ?= -std=c99

###############################################################################
# Sources (only list what actually exists)
OBJS += $(SRC_DIR)/firmware.o
# OBJS += $(SRC_DIR)/timer.o
# OBJS += $(SRC_DIR)/info.o
# OBJS += $(SHARED_SRC_DIR)/core/system.o
# OBJS += $(SHARED_SRC_DIR)/core/uart.o
# OBJS += $(SHARED_SRC_DIR)/core/ring-buffer.o

###############################################################################
# Compiler flags
TGT_CFLAGS   += $(OPT) $(CSTD) $(DEBUG)
TGT_CFLAGS   += $(ARCH_FLAGS)
TGT_CFLAGS   += -Wextra -Wshadow -Wimplicit-function-declaration
TGT_CFLAGS   += -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes
TGT_CFLAGS   += -fno-common -ffunction-sections -fdata-sections

TGT_CXXFLAGS += $(OPT) $(CXXSTD) $(DEBUG)
TGT_CXXFLAGS += $(ARCH_FLAGS)
TGT_CXXFLAGS += -Wextra -Wshadow -Wredundant-decls -Weffc++
TGT_CXXFLAGS += -fno-common -ffunction-sections -fdata-sections

TGT_CPPFLAGS += -MD
TGT_CPPFLAGS += -Wall -Wundef
TGT_CPPFLAGS += $(DEFS)

###############################################################################
# Linker flags
TGT_LDFLAGS  += --static -nostartfiles
TGT_LDFLAGS  += -T$(LDSCRIPT)
TGT_LDFLAGS  += $(ARCH_FLAGS) $(DEBUG)
TGT_LDFLAGS  += -Wl,-Map=$(BINARY).map -Wl,--cref
TGT_LDFLAGS  += -Wl,--gc-sections
ifeq ($(V),99)
TGT_LDFLAGS  += -Wl,--print-gc-sections
endif

# C runtime libs
LDLIBS += -Wl,--start-group -lc -lgcc -lnosys -Wl,--end-group

###############################################################################
# Targets

.SUFFIXES: .elf .bin .hex .srec .list .map .images
.SECONDEXPANSION:
.SECONDARY:

all: $(BINARY).elf $(BINARY).bin

# Library: build libopencm3 if missing
$(OPENCM3_DIR)/lib/lib$(LIBNAME).a:
ifeq (,$(wildcard $@))
	$(warning $(LIBNAME).a not found, attempting to rebuild in $(OPENCM3_DIR))
	$(MAKE) -C $(OPENCM3_DIR)
endif

# Link to ELF
$(BINARY).elf: $(OBJS) $(LDSCRIPT) $(OPENCM3_DIR)/lib/lib$(LIBNAME).a Makefile
	$(Q)$(LD) $(TGT_LDFLAGS) $(LDFLAGS) $(OBJS) $(LDLIBS) -o $@

# Images
$(BINARY).bin: $(BINARY).elf
	$(Q)$(OBJCOPY) -Obinary $< $@

$(BINARY).hex: $(BINARY).elf
	$(Q)$(OBJCOPY) -Oihex $< $@

$(BINARY).srec: $(BINARY).elf
	$(Q)$(OBJCOPY) -Osrec $< $@

$(BINARY).list: $(BINARY).elf
	$(Q)$(OBJDUMP) -S $< > $@

images: $(BINARY).bin $(BINARY).hex $(BINARY).srec $(BINARY).list $(BINARY).map

# Flash with ST-Link
flash: $(BINARY).bin
	$(STFLASH) write $(BINARY).bin 0x8000000

# Compile rules
%.o: %.c
	$(Q)$(CC) $(TGT_CFLAGS) $(CFLAGS) $(TGT_CPPFLAGS) $(CPPFLAGS) -o $@ -c $<

%.o: %.S
	$(Q)$(CC) $(TGT_CFLAGS) $(CFLAGS) -o $@ -c $<

clean:
	$(Q)$(RM) $(BINARY).elf $(BINARY).bin $(BINARY).hex $(BINARY).srec $(BINARY).list $(BINARY).map \
	          generated.* $(OBJS) $(OBJS:.o=.d)

.PHONY: all images clean flash

# Auto-deps
-include $(OBJS:.o=.d)
