#==================================================
# Toolchain
#==================================================

ARCH = rv64imac_zicsr
ABI  = lp64

CC = riscv64-linux-gnu-gcc
LD = riscv64-linux-gnu-gcc


#==================================================
# Directories
#==================================================

SRC_DIR   = src
BUILD_DIR = build
RUN_DIR   = run



#==================================================
# Compiler Flags
#==================================================

CFLAGS = -nostdlib -nostartfiles -ffreestanding \
	-march=$(ARCH) -mabi=$(ABI) \
	-mcmodel=medany \
	-Wall -Wextra \
	-O0 -g \
	-fno-builtin \
	-fno-inline \
	-fno-pic \
	-fno-pie \
	-mno-relax



#==================================================
# Include Paths
#==================================================

CFLAGS += \
	-I$(SRC_DIR)/kernel \
	-I$(SRC_DIR)/kernel/task \
	-I$(SRC_DIR)/kernel/sched \
	-I$(SRC_DIR)/trap \
	-I$(SRC_DIR)/console \
	-I$(SRC_DIR)/logger \
	-I$(SRC_DIR)/panic \
	-I$(SRC_DIR)/interrupt \
	-I$(SRC_DIR)/sync \
	-I$(SRC_DIR)/mm \
	-I$(SRC_DIR)/ds \
	-I$(SRC_DIR)/ds/list \
	-I$(SRC_DIR)/ds/queue \
	-I$(SRC_DIR)/ds/ring \
	-I$(SRC_DIR)/ds/bitmap \
	-I$(SRC_DIR)/ds/waitqueue \
	-I$(SRC_DIR)/drivers/uart \
	-I$(SRC_DIR)/drivers/timer \
	-I$(SRC_DIR)/drivers/clint \
	-I$(SRC_DIR)/drivers/plic \
	-I$(SRC_DIR)/shell \
	-I$(SRC_DIR)/commands \
	-I$(SRC_DIR)/lib



#==================================================
# Linker Flags
#==================================================

LDFLAGS = -nostdlib \
	-T linker.ld \
	-no-pie \
	-Wl,--no-relax \
	-Wl,--no-warn-rwx-segments



#==================================================
# Assembly Sources
#==================================================

BOOT_S = $(SRC_DIR)/boot/boot.S

TRAP_S = $(SRC_DIR)/trap/trap.S



#==================================================
# Kernel
#==================================================

KERNEL_SRC = \
	$(SRC_DIR)/kernel/main.c



#==================================================
# Task
#==================================================

TASK_SRC = \
	$(SRC_DIR)/kernel/task/task.c



#==================================================
# Scheduler
#==================================================

SCHED_SRC = \
	$(SRC_DIR)/kernel/sched/scheduler.c



#==================================================
# Trap
#==================================================

TRAP_SRC = \
	$(SRC_DIR)/trap/trap.c



#==================================================
# Drivers
#==================================================

DRIVER_SRC = \
	$(SRC_DIR)/drivers/uart/uart.c \
	$(SRC_DIR)/drivers/timer/timer.c \
	$(SRC_DIR)/drivers/plic/plic.c



#==================================================
# Console
#==================================================

CONSOLE_SRC = \
	$(SRC_DIR)/console/console.c



#==================================================
# Logger
#==================================================

LOGGER_SRC = \
	$(SRC_DIR)/logger/logger.c



#==================================================
# Panic
#==================================================

PANIC_SRC = \
	$(SRC_DIR)/panic/panic.c



#==================================================
# Interrupt
#==================================================

INTERRUPT_SRC = \
	$(SRC_DIR)/interrupt/interrupt.c



#==================================================
# Synchronization
#==================================================

SYNC_SRC = \
	$(SRC_DIR)/sync/spinlock.c



#==================================================
# Memory
#==================================================

MM_SRC = \
	$(SRC_DIR)/mm/memory.c \
	$(SRC_DIR)/mm/heap.c



#==================================================
# Data Structures
#==================================================

DS_SRC = \
	$(SRC_DIR)/ds/list/list.c \
	$(SRC_DIR)/ds/queue/queue.c \
	$(SRC_DIR)/ds/ring/ring.c \
	$(SRC_DIR)/ds/bitmap/bitmap.c \
	$(SRC_DIR)/ds/waitqueue/waitqueue.c



#==================================================
# Libraries
#==================================================

LIB_SRC = \
	$(SRC_DIR)/lib/string.c \
	$(SRC_DIR)/lib/ctype.c \
	$(SRC_DIR)/lib/stdio.c \
	$(SRC_DIR)/lib/math.c \
	$(SRC_DIR)/lib/stdlib.c \
	$(SRC_DIR)/lib/errno.c



#==================================================
# Shell
#==================================================

SHELL_SRC = \
	$(SRC_DIR)/shell/shell.c



#==================================================
# Commands
#==================================================

COMMAND_SYSTEM_SRC = \
	$(SRC_DIR)/commands/command.c


COMMAND_SRC = \
	$(SRC_DIR)/commands/cmd_help.c \
	$(SRC_DIR)/commands/cmd_echo.c \
	$(SRC_DIR)/commands/cmd_time.c \
	$(SRC_DIR)/commands/cmd_ticks.c \
	$(SRC_DIR)/commands/cmd_clear.c \
	$(SRC_DIR)/commands/cmd_exit.c \
	$(SRC_DIR)/commands/cmd_cpuinfo.c \
	$(SRC_DIR)/commands/cmd_mem.c



#==================================================
# Object Files
#==================================================

BOOT_O = $(BUILD_DIR)/boot.o

TRAP_O = $(BUILD_DIR)/trap.o



KERNEL_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(KERNEL_SRC))


TASK_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(TASK_SRC))


SCHED_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SCHED_SRC))


TRAP_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(TRAP_SRC))


DRIVER_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(DRIVER_SRC))


CONSOLE_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(CONSOLE_SRC))


LOGGER_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(LOGGER_SRC))


PANIC_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(PANIC_SRC))


INTERRUPT_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(INTERRUPT_SRC))


SYNC_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SYNC_SRC))


MM_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(MM_SRC))


DS_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(DS_SRC))


LIB_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(LIB_SRC))


SHELL_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SHELL_SRC))


COMMAND_SYSTEM_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(COMMAND_SYSTEM_SRC))


COMMAND_OBJ = \
	$(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(COMMAND_SRC))



#==================================================
# All Objects
#==================================================

OBJS = \
	$(BOOT_O) \
	$(TRAP_O) \
	$(KERNEL_OBJ) \
	$(TASK_OBJ) \
	$(SCHED_OBJ) \
	$(TRAP_OBJ) \
	$(DRIVER_OBJ) \
	$(CONSOLE_OBJ) \
	$(LOGGER_OBJ) \
	$(PANIC_OBJ) \
	$(INTERRUPT_OBJ) \
	$(SYNC_OBJ) \
	$(MM_OBJ) \
	$(DS_OBJ) \
	$(LIB_OBJ) \
	$(SHELL_OBJ) \
	$(COMMAND_SYSTEM_OBJ) \
	$(COMMAND_OBJ)



#==================================================
# Target
#==================================================

TARGET = $(RUN_DIR)/kernel.elf



all: $(TARGET)



#==================================================
# Directory Creation
#==================================================

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)


$(RUN_DIR):
	mkdir -p $(RUN_DIR)



#==================================================
# Assembly Build
#==================================================

$(BOOT_O): $(BOOT_S) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


$(TRAP_O): $(TRAP_S) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@



#==================================================
# C Build Rule
#==================================================

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@



#==================================================
# Linking
#==================================================

$(TARGET): $(OBJS) | $(RUN_DIR)
	$(LD) $(LDFLAGS) $(OBJS) -o $@



#==================================================
# QEMU
#==================================================

emu: $(TARGET)
	qemu-system-riscv64 \
		-machine virt \
		-bios none \
		-kernel $(TARGET) \
		-nographic \
		-serial mon:stdio



#==================================================
# Clean
#==================================================

clean:
	rm -rf $(BUILD_DIR) $(RUN_DIR)



#==================================================
# Debug
#==================================================

disasm: $(TARGET)
	riscv64-linux-gnu-objdump -d $(TARGET) | less


symbols: $(TARGET)
	riscv64-linux-gnu-nm $(TARGET)


size: $(TARGET)
	riscv64-linux-gnu-size $(TARGET)



#==================================================
# Checks
#==================================================

check-task: $(TARGET)
	@echo "=== Task ==="
	riscv64-linux-gnu-nm $(TARGET) | grep task


check-sched: $(TARGET)
	@echo "=== Scheduler ==="
	riscv64-linux-gnu-nm $(TARGET) | grep scheduler


check-ds: $(TARGET)
	@echo "=== Data Structures ==="
	riscv64-linux-gnu-nm $(TARGET) | \
	grep -E "(list|queue|ring|bitmap|waitqueue)"


check-interrupt: $(TARGET)
	@echo "=== Interrupt ==="
	riscv64-linux-gnu-nm $(TARGET) | grep interrupt


check-plic: $(TARGET)
	@echo "=== PLIC ==="
	riscv64-linux-gnu-nm $(TARGET) | grep plic


check-sync: $(TARGET)
	@echo "=== Spinlock ==="
	riscv64-linux-gnu-nm $(TARGET) | grep spinlock


check-mm: $(TARGET)
	@echo "=== Memory ==="
	riscv64-linux-gnu-nm $(TARGET) | grep -E "(heap|memory)"



#==================================================
# PHONY
#==================================================

.PHONY: \
	all \
	clean \
	emu \
	disasm \
	symbols \
	size \
	check-task \
	check-sched \
	check-ds \
	check-interrupt \
	check-plic \
	check-sync \
	check-mm