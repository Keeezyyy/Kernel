[BITS 64]

extern handle_timer_interrupt

global timer_catch_wrapper
timer_catch_wrapper:
    cld
    call    handle_timer_interrupt
    iretq


