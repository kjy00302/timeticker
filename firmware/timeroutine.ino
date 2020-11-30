const int TIMEROUTINE_MS = 100;
unsigned char timeroutine_time[3];
unsigned char lasttime[3] = {255,255,255};

extern unsigned char evtflag;
extern unsigned char confflag;

volatile unsigned int timeroutine_t = 0;
unsigned char timeroutine_state = 0;

void timeroutine(){
  if (timeroutine_t > TIMEROUTINE_MS){
    if ((confflag & CONF_CLOCK_UPDATE) != 0){
      time_getrtctime(timeroutine_time);
      if (lasttime[2] != timeroutine_time[2]){
        lasttime[2] = timeroutine_time[2];
        evtflag |= EVT_SECOND;
      }
      if ((lasttime[1] != timeroutine_time[1])){
        lasttime[1] = timeroutine_time[1];
        evtflag |= EVT_MINUTE;
      }
      if ((lasttime[0] != timeroutine_time[0])){
        lasttime[0] = timeroutine_time[0];
        evtflag |= EVT_HOUR;
      }
    }
    timeroutine_t -= TIMEROUTINE_MS;
  }
}
