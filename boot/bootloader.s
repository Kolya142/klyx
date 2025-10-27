bits 16
org 0x7C00

;; Kernel size is 512kb
;; Sector size is 0.5kb
;; Sector count for kernel is 1024

mov ax, cs
mov ds, ax

mov ah, 0x42
mov dl, 0x80
mov si, read_block
int 0x13

jnae panic

;; TODO: enable A20. Currently i won't do this because qemu already did it for me.

enable_pm:
   cli
   lgdt [gdt_descriptor]
   mov eax, cr0
   or eax, 1
   mov cr0, eax
   jmp 0x08:pm

bits 32
pm:
   mov ax, 0x10
   mov ds, ax
   mov cs, ax
   mov fs, ax
   mov gs, ax
   mov ss, ax
   mov esp, 0x1000000
   mov ebp, esp
   jmp 0x7e00

bits 16

panic:
   mov ax, 0xB800
   mov ds, ax
   mov word [0], 0x0145
   jmp $

;; Yeah, i truncate kernel size to 512kb but SeaBIOS cannot load >127 sectors

read_block:
   db 16 ;; u8 size        |0
   db 0 ;; u8 reserved     |1
   dw 127  ;; u16 count    |2
   dw 0x7e00 ;; u16 memoff |4
   dw 0x0000 ;; u16 memseg |6
   dq 1 ;; u64 lba         |8

;; GDT is a useless thing that can only make developers angry.
gdt:
   dq 0x0000000000000000
   dq 0x00CF9A000000FFFF
   dq 0x00CF92000000FFFF
   dq 0x00CFFA000000FFFF
   dq 0x00CFF2000000FFFF
gdt_end:
gdt_descriptor:
   dw gdt_end-gdt-1
   dq gdt
gdtr:
   dw 0
   dd 0
times 510-($-$$) db 0
dw 0xAA55
