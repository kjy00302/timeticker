extern unsigned char evtflag;
extern unsigned char confflag;

char tickerroutine_index = 0;
unsigned int tickerroutine_message[33] = {0,};

void tickerroutine() {
  if ((confflag & CONF_CLOCK_UPDATE) != 0) {
    switch (tickerroutine_index) {
      case 0: {
          if ((confflag & CONF_HANGULTIME) != 0) {
            displaytime_hangul(timeroutine_time);
          } else {
            displaytime_numeric(timeroutine_time);
          }
          break;
        }

      case 1: {
          if ((confflag & CONF_HANGULTIME) != 0) {
            displaytemphumi();
          } else {
            displaytemp();
            charbuffer_enqueue(' ');
            displayhumi();
          }
          break;
        }

      case 2: {
          writestring(tickerroutine_message);
          if (tickerroutine_message[0] == 0) {
            evtflag |= EVT_DISPLAY_EMPTY;
          }
          break;
        }
    }
    tickerroutine_index = (tickerroutine_index + 1) % 3;
  }
}
