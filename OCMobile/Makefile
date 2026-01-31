CC := clang
CXX := clang++
CFLAGS := -ffreestanding -nostdlib -O2 -Wall -Wextra -Wimplicit-function-declaration -target aarch64-none-elf -I.
AS := nasn
ASFLAGS := # later
DTC := dtc

sources := loader.c \
	BootMenu.c \
	Framebuffer.c \
	ACPIParser.c \
	platform.c \
	Platform/plist/plist.c \
	Platform/OpenPartitionDxe/Gpt.c \
	Platform/OpenPartitionDxe/Mbr.c \
	Platform/Kextld.c \
	bootlib.c \
	kernel_patcher.c

config.dtb: config.dts
	-$(DTC) -O dtb -o config.dtb config.dts

patch-kernel:
	./Tools/patch_kernel.sh

.PHONY: patch-kernel

all: config.dtb
	$(CC) $(CFLAGS) -o ocmboot $(sources)
