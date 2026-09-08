[BITS 64]


;for now route all interrupts to the global wrapper with the index as argument 



;void global_interrupt_handler(uint16_t interrupt_num);
extern global_interrupt_handler

;global isr_wrapper
;isr_wrapper:
;;    push rdi
;;    cld
;;    mov rdi, 48 
;;    call global_interrupt_handler
;;    pop rdi
;;    iret


;save wrapper functions in array with fixed size align functions
%define ISR_STUB_SIZE 16
section .text align=ISR_STUB_SIZE

global isr_stub_table
align ISR_STUB_SIZE
isr_stub_table:

%assign i 0
%rep 256
    global isr_wrapper_ %+ i
    isr_wrapper_ %+ i:
      push rdi
      cld
      mov edi,  i
      call global_interrupt_handler
      pop rdi
      iretq
        times ISR_STUB_SIZE - ($ - isr_wrapper_ %+ i) db 0xCC
        ; void *start;
        ; void *this_line
        ;
        ; fill rest of buffer(also array item) mit 0xCC genau (this_line  - start) mal 
        ;
        
    %assign i i+1
%endrep

