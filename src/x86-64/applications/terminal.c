#include "./terminal.h"

static uint64_t timer_count = 0;

const char* terminal_root_msg = "[root@user ~]$ ";

static uint64_t char_counter = 0;

char command_buffer[128];

void init_terminal(){
  printf(terminal_root_msg);
}

void terminal_keyboard_intput(char c){
  if(c == '\n'){
    new_line();
  }else {
    write_char(c);
  }
}

void new_line(){
  putc('\n');
  
  printf(terminal_root_msg);

  if(command_buffer[0] == 0){
    return;
  }
  execute_command();
  
  printf(terminal_root_msg);

  char_counter = 0;
}

void execute_command(){


  printf("command '");
  printf(command_buffer);
  printf("' not found\n");

  uint64_t *buffer = (uint64_t*)&command_buffer[0];

  for (int i = 0;i< 128 / sizeof(uint64_t);i++) {
    buffer[i] = 0;
  }
}

void write_char(char c){
  if(char_counter < 128)
    command_buffer[char_counter] = c;


  char_counter++;

  if(char_counter > 127)
    kernel_panic("char buffer in terminal");

  putc(c);

}

void termnial_timer_input(void){
  timer_count++;
  if(timer_count > CURSOR_IDENT_SPEED){

    timer_count = 0;
    toggle_cursor();
  }
}


