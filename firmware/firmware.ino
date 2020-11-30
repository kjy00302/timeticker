#include <Wire.h>
#include <SPI.h>

extern unsigned char timeroutine_time[];
extern char display_newbuf_left;

unsigned char evtflag = 0;
unsigned char confflag = 0;

const unsigned char EVT_SECOND = _BV(0);
const unsigned char EVT_MINUTE = _BV(1);
const unsigned char EVT_HOUR = _BV(2);

void setup(){
  timer_init();
  SPI.begin();
  Wire.begin();
  Serial.begin(9600);
  display_init();
  time_init();
}

void at_second(){
}

void at_minute(){
}

void at_hour(){
}

void loop(){

  if (Serial.available()){
    cmdparse();
  }

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

  displayroutine();
  timeroutine();
}
