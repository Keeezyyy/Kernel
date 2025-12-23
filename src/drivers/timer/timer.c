#include "./timer.h"


void handle_timer_interrupt(){
  send_eoi_irq(1);
  termnial_timer_input();
}
