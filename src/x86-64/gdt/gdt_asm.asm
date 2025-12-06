[BITS 64]

KERNEL_CODE_SELECTOR equ 0x08
KERNEL_DATA_SELECTOR equ 0x10

;
; rdi => holds gdtr pointer
;
global load_gdtr
load_gdtr:
    mov rbx, rdi ;rdi 
    LGDT  [rbx]
    ret


    
global gdt_reload_segments
gdt_reload_segments:
   mov ax, KERNEL_DATA_SELECTOR
   mov ds, ax
   mov es, ax
   mov ss, ax
   mov fs, ax
   mov gs, ax

   push KERNEL_CODE_SELECTOR
   lea  rax, [rel .flush_cs]
   push rax
   retfq             

.flush_cs:
    ret

