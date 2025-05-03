SYSCALL = y

CROSS_COMPILE = riscv64-unknown-elf-
CFLAGS += -nostdlib -fno-builtin -g -Wall
CFLAGS += -march=rv32g -mabi=ilp32
CFLAGS += -Iinclude
GDB = gdb-multiarch
# GDB = gdb
CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump

DEFS +=

ifeq (${SYSCALL}, y)
DEFS += -DCONFIG_SYSCALL
endif

QEMU = qemu-system-riscv32
QFLAGS = -nographic -smp 1 -machine virt -bios none

SRCS_ASM = \
	kernel/start.S \
	kernel/mem.S \
	kernel/entry.S \
	user/usys.S
	
SRCS_C = \
	kernel/kernel.c \
	kernel/uart.c \
	kernel/page.c \
	kernel/printf.c \
	kernel/sched.c \
	user/user.c \
	kernel/trap.c \
	kernel/plic.c \
	kernel/timer.c \
	kernel/lock.c \
	kernel/syscall.c

OUTPUT_PATH = build

OBJS_ASM := $(addprefix ${OUTPUT_PATH}/, $(patsubst %.S, %.o, ${SRCS_ASM}))
OBJS_C   := $(addprefix $(OUTPUT_PATH)/, $(patsubst %.c, %.o, ${SRCS_C}))

OBJS = ${OBJS_ASM} ${OBJS_C}


ELF = ${OUTPUT_PATH}/os.elf

all: ${OUTPUT_PATH} ${ELF}

${OUTPUT_PATH}:
	@mkdir -p ${OUTPUT_PATH}
	@echo "Creating output directory: ${OUTPUT_PATH}"

${OUTPUT_PATH}/%.o: %.c
	@echo "Compiling $<"
	@mkdir -p $(dir $@)
	$(CC) ${DEFS} ${CFLAGS} -c $< -o $@
	@echo "Compiled $< to $@"

${OUTPUT_PATH}/%.o: %.S
	@echo "Assembling $<"
	@mkdir -p $(dir $@)
	$(CC) ${DEFS} ${CFLAGS} -c $< -o $@
	@echo "Assembled $< to $@"

${ELF}: ${OBJS}
	${CC} ${DEFS} ${CFLAGS} -o $@ ${OBJS} -T kernel.ld

run: all
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo "------------------------------------"
	@${QEMU} ${QFLAGS} -kernel ${ELF}

clean:
	@rm -rf ${OUTPUT_PATH}

debug: all
	@echo "Press Ctrl-C and then input 'quit' to exit GDB and QEMU"
	@echo "-------------------------------------------------------"
	${QEMU} ${QFLAGS} -kernel ${ELF} -s -S &
	${GDB} ${ELF} -q -x gdbinit

code: all
	@${OBJDUMP} -S ${ELF} | less