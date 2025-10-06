PROJECT = firmware
BUILD   = build
LIBOPENCM3 = libopencm3

CFLAGS  = -Os -g -Wall -I$(LIBOPENCM3)/include
LDFLAGS = -L$(LIBOPENCM3)/lib -lopencm3_stm32l0 -nostartfiles

OBJS = $(BUILD)/firmware.o

$(BUILD)/%.o: src/%.c
	mkdir -p $(BUILD)
	arm-none-eabi-gcc $(CFLAGS) -mcpu=cortex-m0plus -mthumb -c $< -o $@

$(BUILD)/$(PROJECT).elf: $(OBJS)
	arm-none-eabi-gcc $(OBJS) $(LDFLAGS) -o $@

clean:
	rm -rf $(BUILD)
