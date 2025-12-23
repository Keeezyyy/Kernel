#pragma once 
#include "../../x86-64/stdio/stdio.h"
#include "../../x86-64/idt/idt.h"
#include "../../x86-64/applications/terminal.h"

void handle_timer_interrupt();

extern void timer_catch_wrapper(void);
