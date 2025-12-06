[BITS 64]

global load_idtr
load_idtr:
    lidt [rdi]
    ret


extern interrupt_handler

%macro ISR_NOERR 1
global isr_routine_%1
isr_routine_%1:
    cld
    ; Wenn du die Interrupt-Nummer brauchst:
    mov     rdi, %1          ; oder %1-1, je nach deinem Schema
    call    interrupt_handler
    iretq
%endmacro

; ISRs 1–255 erzeugen
%assign i 1
%rep 256
    ISR_NOERR i
    %assign i i+1
%endrep

