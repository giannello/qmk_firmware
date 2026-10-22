# NUC123-specific rules
SRC += bootloader.c matrix.c

BOOTLOADER = custom
PROGRAM_CMD = hyisp-util flash --convert --yes $(BUILD_DIR)/$(TARGET).bin

MCU_FAMILY = NUMICRO
MCU_SERIES = NUC123

# linker script to use
MCU_LDSCRIPT = NUC123xD4xx0

# startup code to use
MCU_STARTUP = NUC123
BOARD = NUC123SD4AN0

# NUC123 series is Cortex M0
MCU  = cortex-m0
# ARM version, CORTEX-M0/M1 are 6, CORTEX-M3/M4/M7 are 7
ARMV = 6
