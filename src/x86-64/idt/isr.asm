[BITS 64]

global load_idtr
load_idtr:
    lidt [rdi]
    ret

extern interrupt_handler
global isr_wrapper
isr_wrapper:
    ; --- Save all general-purpose registers ---
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    cld                     ; DF must be clear for SysV ABI
    call interrupt_handler  ; call your C handler

    ; --- Restore registers ---
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax

    iretq                  ; 64-bit return from interrupt
