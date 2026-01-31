PREFIX = arm-none-eabi-
CC = $(PREFIX)gcc
AS = $(PREFIX)AS
LD = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump
SIZE = $(PREFIX)size

SRC_DIR = src
INC_DIR = inc
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj

C_SOURCES = $(wildcard $(SRC_DIR)/*.c)
ASM_SOURCES = startup.s

C_OBJECTS = $(C_SOURCES:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
ASM_OBJECTS = $(ASM_SOURCES:%.s=$(OBJ_DIR)/%.o)
OBJECTS = $(C_OBJECTS) $(ASM_OBJECTS)

INCLUDES = -I$(INC_DIR) -I$(SRC_DIR)

MCU = -mcpu=cortex-m3 -mthumb
CFLAGS = $(MCU) -Wall -Wextra -g -O0 -ffunction-sections -fdata-sections
CFLAGS += -DSTM32F103xE $(INCLUDES)
ASFLAGS = $(MCU) -g

LDFLAGS = $(MCU) -T linker.ld -Wl,--gc-sections -Wl,-Map=${BUILD_DIR}/os.map

LDFLAGS += -nostdlib -Wl,--start-group -lgcc -Wl,--end-group

all: $(BUILD_DIR)/os.elf $(BUILD_DIR)/os.hex $(BUILD_DIR)/os.bin
	@$(SIZE) $(BUILD_DIR)/os.elf

$(BUILD_DIR)/os.elf: $(OBJECTS) | $(BUILD_DIR)
	@echo "link $@"
	@$(CC) $(LDFLAGS) -o $@ $(OBJECTS) -v

$(BUILD_DIR)/os.hex: $(BUILD_DIR)/os.elf
	@echo "gen HEX: $@"
	@$(OBJCOPY) -O ihex $< $@

$(BUILD_DIR)/os.bin: $(BUILD_DIR)/os.elf
	@echo "gen BIN: $@"
	@$(OBJCOPY) -O binary $< $@

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "compile $<"
	@$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o:%.s | $(OBJ_DIR)
	@echo "compile $<"
	@$(CC) $(ASFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

.PHONY: all
