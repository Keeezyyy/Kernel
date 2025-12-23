#pragma once
#include "../stdio/stdio.h"
#include "../../kernel/kernel.h"

#define CURSOR_IDENT_SPEED 7

void init_terminal();
void terminal_keyboard_intput(char c);
void termnial_timer_input(void);
void new_line();
void write_char(char c);
void execute_command();
