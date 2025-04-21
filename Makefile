SRCS_ASM := $(shell find . -name '*.S' | sed 's|^\./||')
SRCS_C   := $(shell find . -name '*.c' | sed 's|^\./||')


CROSS_COMPILE = riscv64-unknown-elf-
CFLAGS += -nostdlib -fno-builtin -g -Wall
CFLAGS += -march=rv32g -mabi=ilp32

QEMU = qemu-system-riscv32
QFLAGS = -nographic -smp 1 -machine virt -bios none

GDB = gdb-multiarch
CC = ${CROSS_COMPILE}gcc
OBJCOPY = ${CROSS_COMPILE}objcopy
OBJDUMP = ${CROSS_COMPILE}objdump
MKDIR = mkdir -p
RM = rm -rf


OUTPUT_PATH =  build


OBJS_ASM := $(patsubst %.S, $(OUTPUT_PATH)/%.o, $(SRCS_ASM))
OBJS_C := $(patsubst %.c, $(OUTPUT_PATH)/%.o, $(SRCS_C))
OBJS = ${OBJS_ASM} ${OBJS_C}

ELF = ${OUTPUT_PATH}/os.elf
BIN = ${OUTPUT_PATH}/os.bin

USE_LINKER_SCRIPT ?= true
ifeq (${USE_LINKER_SCRIPT}, true)
LDFLAGS = -T kernel/kernel.ld
else
LDFLAGS = -Ttext=0x80000000
endif


all: $(OUTPUT_PATH) ${ELF} 
$(OUTPUT_PATH):
	@$(MKDIR) ${OUTPUT_PATH}

${ELF}: ${OBJS}
ifeq (${USE_LINKER_SCRIPT}, true)
	${CC} -E -P -x c ${DEFS} ${CFLAGS} kernel/kernel.ld > ${OUTPUT_PATH}/kernel.ld.generated
endif
	${CC} ${CFLAGS} ${LDFLAGS} -o ${ELF} $^
	${OBJCOPY} -O binary ${ELF} ${BIN}

${OUTPUT_PATH}/%.o: %.c | ${OUTPUT_PATH}
	$(MKDIR) $(dir $@)
	${CC} ${DEFS} ${CFLAGS} -c -o $@ $<

${OUTPUT_PATH}/%.o: %.S | ${OUTPUT_PATH}
	$(MKDIR) $(dir $@)
	${CC} ${DEFS} ${CFLAGS} -c -o $@ $<

.PHONY: build
build: all


run: all
	@${QEMU} -M ? | grep virt >/dev/null || { echo "QEMU does not support 'virt' machine"; exit 1; }
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo "------------------------------------"
	@${QEMU} ${QFLAGS} -kernel ${ELF}

clean:
	@${RM} ${OUTPUT_PATH} ${OUTPUT_PATH}/os.ld.generated

debug: all
	@echo "Press Ctrl-A and then X to exit QEMU"
	@echo "------------------------------------"
	@${QEMU} ${QFLAGS} -kernel ${ELF} -gdb tcp::1235 -S &
	@${GDB} ${ELF} -q -x ./gdbinit