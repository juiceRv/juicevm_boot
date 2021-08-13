.PHONY:all clean
# CROSS_COMPILE?=riscv64-unknown-elf-
# CROSS_COMPILE?=/opt/xpack-riscv-none-embed-gcc-8.3.0-2.1/bin/riscv-none-embed-
CROSS_COMPILE?=/mnt/ssd_prj/juicevm_gcc_embed_toolchains/bin/riscv64-unknown-elf-
PRJ_NAME  ?=juicevm_boot

AS 		:= $(CROSS_COMPILE)gcc -x assembler-with-cpp
CC 		:= $(CROSS_COMPILE)gcc
OBJDUMP := $(CROSS_COMPILE)objdump
OBJCOPY := $(CROSS_COMPILE)objcopy
LD		:= $(CROSS_COMPILE)ld

DEVICE := -march=rv64ima -mabi=lp64 -mcmodel=medany
# -fno-builtin
CFLAGS  	:=  -static \
				-ffunction-sections \
				-fdata-sections \
				-fno-builtin -fno-builtin-printf -Os

CFLAGS		+= $(DEVICE)

OBJFLASGS 	:= --disassemble-all \
				--disassemble-zeroes \
				--section=.text \
				--section=.text.startup \
				--section=.text.init \
				--section=.data

LDFLAGS		:= $(DEVICE) 
LDFLAGS		+= -nostartfiles
LDFLAGS     += -T$(PRJ_NAME)_link.ld -lm

CFLAGS      += -D__riscv_xlen=64 -D__riscv64 
CFLAGS      += -I ../../ 
CFLAGS      += -I .
CFLAGS      += -I printf

SRCS       := main.c
SRCS       += printf/printf.c

ASMS       := start.S
OBJS       := $(SRCS:.c=.o)
OBJ        := $(ASMS:.S=.o)

LINK_OBJS += $(OBJS) $(OBJ)

all: $(PRJ_NAME).bin $(PRJ_NAME)_dump.s

$(OBJ): %.o : %.S
	@echo [AS] $<
	@$(AS) $(CFLAGS) -c $< -o $@

$(OBJS): %.o:%.c
	@echo [CC] $<
	@$(CC) $(CFLAGS) -c $< -o $@

$(PRJ_NAME).elf:$(OBJ) $(OBJS) 
	@echo [LD] Linking $@
	$(CC) $(LINK_OBJS) -o $@ $(LDFLAGS)

$(PRJ_NAME).bin:$(PRJ_NAME).elf
	@echo [LD] Linking $@
	@$(OBJCOPY) -O binary $< $@

$(PRJ_NAME)_dump.s:$(PRJ_NAME).elf
	@echo [OD] Objdump $@ $<
	@$(OBJDUMP) $(OBJFLASGS) $< > $@

clean:
	rm $(OBJS) $(OBJ) $(PRJ_NAME).bin $(PRJ_NAME).elf $(PRJ_NAME)_dump.s