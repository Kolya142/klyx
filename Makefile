.PHONY: run

CC=cc
CFLAGS=-static -Iinclude -ffreestanding -nostdlib -fno-builtin -m32 -c -Wall -O -static -fno-toplevel-reorder -mgeneral-regs-only -fno-stack-protector -fno-pic -fno-pie -no-pie -ffreestanding -m32 -mhard-float -mfpmath=387
LD=ld
LDFLAGS_BIN=-T link.ld -melf_i386 -s -x -M
LDFLAGS_ELF=-T link_elf.ld -melf_i386
SAS=nasm
SASFLAGS=

objects=build/_main.o $(subst kernel/,build/,$(patsubst %.c,%.o,$(shell find kernel/ -name "*.c")))

drive.img: build/kernel.bin build/bootloader.bin
	cat build/bootloader.bin build/kernel.bin > $@

run: drive.img
	qemu-system-i386 drive.img -m 4G

build/bootloader.bin: boot/bootloader.s
	$(SAS) $(SASFLAGS) $< -o $@
build/kernel.bin: $(objects)
	$(LD) $(LDFLAGS_BIN) $(objects) -o $@
	truncate -s 524288 $@
build/kernel.elf: $(objects)
	$(LD) $(LDFLAGS_ELF) $(objects) -o $@

build/_main.o: kernel/main.Q
	$(CC) -x c $(CFLAGS) $< -o $@
build/%.o: kernel/%.c
	$(CC) $(CFLAGS) $< -o $@
