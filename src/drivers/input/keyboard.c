#include "./keyboard.h"

void handle_keyboard_interrupt(void){
  uint8_t sc = inb(0x60);
  //printf("key pressed : %x\n", sc);

  struct key_event ke = translate_scancode(sc);

  if(ke.pressed)
     putc(translate_keycode_to_asci(ke.key));

  send_eoi_irq(1);
  return;
}

char translate_keycode_to_asci(Key key){
  switch (key) {
    case KEY_A: return 'a';
    case KEY_B: return 'b';
    case KEY_C: return 'c';
    case KEY_D: return 'd';
    case KEY_E: return 'e';
    case KEY_F: return 'f';
    case KEY_G: return 'g';
    case KEY_H: return 'h';
    case KEY_I: return 'i';
    case KEY_J: return 'j';
    case KEY_K: return 'k';
    case KEY_L: return 'l';
    case KEY_M: return 'm';
    case KEY_N: return 'n';
    case KEY_O: return 'o';
    case KEY_P: return 'p';
    case KEY_Q: return 'q';
    case KEY_R: return 'r';
    case KEY_S: return 's';
    case KEY_T: return 't';
    case KEY_U: return 'u';
    case KEY_V: return 'v';
    case KEY_W: return 'w';
    case KEY_X: return 'x';
    case KEY_Y: return 'y';
    case KEY_Z: return 'z';

    case KEY_1: return '1';
    case KEY_2: return '2';
    case KEY_3: return '3';
    case KEY_4: return '4';
    case KEY_5: return '5';
    case KEY_6: return '6';
    case KEY_7: return '7';
    case KEY_8: return '8';
    case KEY_9: return '9';
    case KEY_0: return '0';

    case KEY_MINUS:      return '-';
    case KEY_EQUAL:      return '=';
    case KEY_LEFTBRACE:  return '[';
    case KEY_RIGHTBRACE: return ']';
    case KEY_BACKSLASH:  return '\\';
    case KEY_SEMICOLON:  return ';';
    case KEY_APOSTROPHE: return '\'';
    case KEY_GRAVE:      return '`';
    case KEY_COMMA:      return ',';
    case KEY_DOT:        return '.';
    case KEY_SLASH:      return '/';

    case KEY_SPACE: return ' ';
    case KEY_TAB:   return '\t';
    case KEY_ENTER: return '\n';

    default:
      return 0;
  }
  }

  struct key_event translate_scancode(uint8_t scancode) {
    struct key_event event;

    event.pressed = !(scancode & 0x80);
    uint8_t code = scancode & 0x7F;

    switch (code) {
      case SC_A: event.key = KEY_A; break;
      case SC_B: event.key = KEY_B; break;
      case SC_C: event.key = KEY_C; break;
      case SC_D: event.key = KEY_D; break;
      case SC_E: event.key = KEY_E; break;
      case SC_F: event.key = KEY_F; break;
      case SC_G: event.key = KEY_G; break;
      case SC_H: event.key = KEY_H; break;
      case SC_I: event.key = KEY_I; break;
      case SC_J: event.key = KEY_J; break;
      case SC_K: event.key = KEY_K; break;
      case SC_L: event.key = KEY_L; break;
      case SC_M: event.key = KEY_M; break;
      case SC_N: event.key = KEY_N; break;
      case SC_O: event.key = KEY_O; break;
      case SC_P: event.key = KEY_P; break;
      case SC_Q: event.key = KEY_Q; break;
      case SC_R: event.key = KEY_R; break;
      case SC_S: event.key = KEY_S; break;
      case SC_T: event.key = KEY_T; break;
      case SC_U: event.key = KEY_U; break;
      case SC_V: event.key = KEY_V; break;
      case SC_W: event.key = KEY_W; break;
      case SC_X: event.key = KEY_X; break;
      case SC_Y: event.key = KEY_Y; break;
      case SC_Z: event.key = KEY_Z; break;

      case SC_1: event.key = KEY_1; break;
      case SC_2: event.key = KEY_2; break;
      case SC_3: event.key = KEY_3; break;
      case SC_4: event.key = KEY_4; break;
      case SC_5: event.key = KEY_5; break;
      case SC_6: event.key = KEY_6; break;
      case SC_7: event.key = KEY_7; break;
      case SC_8: event.key = KEY_8; break;
      case SC_9: event.key = KEY_9; break;
      case SC_0: event.key = KEY_0; break;

      case SC_MINUS:      event.key = KEY_MINUS; break;
      case SC_EQUAL:      event.key = KEY_EQUAL; break;
      case SC_LBRACKET:   event.key = KEY_LEFTBRACE; break;
      case SC_RBRACKET:   event.key = KEY_RIGHTBRACE; break;
      case SC_BACKSLASH:  event.key = KEY_BACKSLASH; break;
      case SC_SEMICOLON:  event.key = KEY_SEMICOLON; break;
      case SC_APOSTROPHE: event.key = KEY_APOSTROPHE; break;
      case SC_GRAVE:      event.key = KEY_GRAVE; break;
      case SC_COMMA:      event.key = KEY_COMMA; break;
      case SC_DOT:        event.key = KEY_DOT; break;
      case SC_SLASH:      event.key = KEY_SLASH; break;

      case SC_ENTER:     event.key = KEY_ENTER; break;
      case SC_BACKSPACE: event.key = KEY_BACKSPACE; break;
      case SC_TAB:       event.key = KEY_TAB; break;
      case SC_SPACE:     event.key = KEY_SPACE; break;
      case SC_ESC:       event.key = KEY_ESC; break;

      case SC_LSHIFT: event.key = KEY_LEFTSHIFT; break;
      case SC_RSHIFT: event.key = KEY_RIGHTSHIFT; break;
      case SC_LCTRL:  event.key = KEY_LEFTCTRL; break;
      case SC_LALT:   event.key = KEY_LEFTALT; break;
      case SC_CAPSLOCK: event.key = KEY_CAPSLOCK; break;

      case SC_F1:  event.key = KEY_F1; break;
      case SC_F2:  event.key = KEY_F2; break;
      case SC_F3:  event.key = KEY_F3; break;
      case SC_F4:  event.key = KEY_F4; break;
      case SC_F5:  event.key = KEY_F5; break;
      case SC_F6:  event.key = KEY_F6; break;
      case SC_F7:  event.key = KEY_F7; break;
      case SC_F8:  event.key = KEY_F8; break;
      case SC_F9:  event.key = KEY_F9; break;
      case SC_F10: event.key = KEY_F10; break;

      case SC_KP_0: event.key = KEY_KP_0; break;
      case SC_KP_1: event.key = KEY_KP_1; break;
      case SC_KP_2: event.key = KEY_KP_2; break;
      case SC_KP_3: event.key = KEY_KP_3; break;
      case SC_KP_4: event.key = KEY_KP_4; break;
      case SC_KP_5: event.key = KEY_KP_5; break;
      case SC_KP_6: event.key = KEY_KP_6; break;
      case SC_KP_7: event.key = KEY_KP_7; break;
      case SC_KP_8: event.key = KEY_KP_8; break;
      case SC_KP_9: event.key = KEY_KP_9; break;
      case SC_KP_DOT: event.key = KEY_KP_DOT; break;
      case SC_KP_PLUS: event.key = KEY_KP_PLUS; break;
      case SC_KP_MINUS: event.key = KEY_KP_MINUS; break;
      case SC_KP_ASTERISK: event.key = KEY_KP_MULTIPLY; break;

      default:
        event.key = KEY_NONE;
        break;
    }

    return event;
  }
