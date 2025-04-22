CROSS_COMPILE = riscv64-unknown-elf-
CFLAGS += -nostdlib -fno-builtin -g -Wall
CFLAGS += -march=rv32g -mabi=ilp32
CFLAGS += -Iinclude
GDB = gdb-multiarch
# GDB = gdb
CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump


QEMU = qemu-system-riscv32
QFLAGS = -nographic -smp 1 -machine virt -bios none

SRCS_ASM = \
	start.S \
	mem.S
SRCS_C = \
	kernel.c \
	uart.c \
	page.c \
	printf.c 

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
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Compiled $< to $@"

${OUTPUT_PATH}/%.o: %.S
	@echo "Assembling $<"
	$(CC) $(CFLAGS) -c $< -o $@
	@echo "Assembled $< to $@"

${ELF}: ${OBJS}
	${CC} ${CFLAGS} -o $@ ${OBJS} -T kernel.ld

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
	${GDB} ${ELF} -q -x ../gdbinit

code: all
	@${OBJDUMP} -S ${ELF} | less