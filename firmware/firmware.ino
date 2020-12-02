#include <Wire.h>
#include <SPI.h>

extern unsigned char timeroutine_time[];
extern char display_newbuf_left;

unsigned char evtflag = 0;
unsigned char confflag = 0b01100011;

const unsigned char EVT_SECOND = _BV(0);
const unsigned char EVT_MINUTE = _BV(1);
const unsigned char EVT_HOUR = _BV(2);
const unsigned char EVT_CHAR_IN = _BV(3);
const unsigned char EVT_TIMESYNC_PENDING = _BV(4);

const unsigned char CONF_DISPLAY_SCROLL_UPDATE = _BV(0);
const unsigned char CONF_CLOCK_UPDATE = _BV(1);
const unsigned char CONF_TIME_AUTOSYNC = _BV(5);
const unsigned char CONF_HANGULTIME = _BV(6);

void setup(){
  timer_init();
  SPI.begin();
  Wire.begin();
  Serial.begin(9600);
  display_init();
  temperature_init();
  time_init();
}

void at_second(){
  if (timeroutine_time[2] % 20 == 0){
    if ((confflag & CONF_HANGULTIME) != 0) {
      displaytime_hangul(timeroutine_time);
    } else {
      displaytime_numeric(timeroutine_time);
    }
  } else if ((timeroutine_time[2] + 10) % 20 == 0){
    if ((confflag & CONF_HANGULTIME) != 0) {
      displaytemphumi();
    } else {
      displaytemphumi();
      charbuffer_enqueue(' ');
      displayhumi();
    }
  } else if ((timeroutine_time[2] == 7) && ((evtflag & EVT_TIMESYNC_PENDING) != 0)){
    time_sync();
    evtflag ^= EVT_TIMESYNC_PENDING;
  }
}

void at_minute(){
}

void at_hour(){
  if ((confflag & CONF_TIME_AUTOSYNC) != 0){
    evtflag |= EVT_TIMESYNC_PENDING;
  }
}

void loop(){

  if (Serial.available()){
    cmdparse();
  }

  if ((confflag & CONF_CLOCK_UPDATE) != 0){
    if ((evtflag & EVT_SECOND) != 0){
      at_second();
      evtflag ^= EVT_SECOND;
    }

    if ((evtflag & EVT_MINUTE) != 0){
      at_minute();
      evtflag ^= EVT_MINUTE;
    }

    if ((evtflag & EVT_HOUR) != 0){
      at_hour();
      evtflag ^= EVT_HOUR;
    }
  }

  displayroutine();
  timeroutine();
}
