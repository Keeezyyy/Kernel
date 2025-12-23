[BITS 64]

extern handle_keyboard_interrupt

global keyboard_catch_wrapper
keyboard_catch_wrapper:
    cld
    call    handle_keyboard_interrupt
    iretq
