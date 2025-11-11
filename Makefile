#    Klyx - an operating system kernel.
#    Copyright (C) 2025 Nikolay Shevelko
#
#    This program is free software; you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation; either version 2 of the License, or
#    (at your option) any later version.
#
#    This program is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License along
#    with this program; if not, see <https://www.gnu.org/licenses/>.



.PHONY: run run_gdb run_debug

CC=cc
CFLAGS=-static -I./include -nostdlib -fno-builtin -m32 -c -Wall -O -static -fno-toplevel-reorder -fno-stack-protector -fno-pic -fno-pie -no-pie -ffreestanding -nostdinc
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

run_gdb: drive.img
	qemu-system-i386 drive.img -m 4G --no-reboot --no-shutdown -s -S

run_debug: drive.img
	qemu-system-i386 drive.img -m 4G --no-reboot --no-shutdown -d int

build/bootloader.bin: boot/bootloader.s
	$(SAS) $(SASFLAGS) $< -o $@
build/kernel.bin: $(objects)
	$(LD) $(LDFLAGS_BIN) $(objects) -o $@
	echo ENDOFKERNEL! >> $@
	truncate -s 65024 $@
build/kernel.elf: $(objects)
	$(LD) $(LDFLAGS_ELF) $(objects) -o $@

build/_main.o: kernel/main.Q
	$(CC) -x c $(CFLAGS) $< -o $@
build/%.o: kernel/%.c
	$(CC) $(CFLAGS) $< -o $@
	$(CC) $(CFLAGS) -S $< -o $@.s
