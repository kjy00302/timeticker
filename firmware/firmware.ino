#include <Wire.h>
#include <SPI.h>

extern const unsigned int* const PROGMEM CONST_STRING[];
extern const unsigned int PROGMEM CONST_HOUR[][3];
extern const unsigned int PROGMEM CONST_DIGIT[];

extern unsigned char timeroutine_time[];
extern char display_newbuf_left;

unsigned char evtflag = 0;
unsigned char confflag = 0b01000011;

const unsigned char EVT_SECOND = _BV(0);
const unsigned char EVT_MINUTE = _BV(1);
const unsigned char EVT_HOUR = _BV(2);
const unsigned char EVT_CHAR_IN = _BV(3);

const unsigned char CONF_DISPLAY_SCROLL_UPDATE = _BV(0);
const unsigned char CONF_CLOCK_UPDATE = _BV(1);
const unsigned char CONF_HANGULTIME = _BV(6);

void setup(){
  timer_init();
  SPI.begin();
  Wire.begin();
  Serial.begin(9600);
  display_init();
  time_init();
}

void displaytimehan(unsigned char* time){
  writestring_progmem(CONST_STRING[0]); // 지금은
  charbuffer_enqueue(0xc624); //오
  if (time[0] < 12){
    charbuffer_enqueue(0xc804); //전
  } else {
    charbuffer_enqueue(0xd6c4); //후
  }
  charbuffer_enqueue(' ');
  writestring_progmem(CONST_HOUR[time[0]%12]);
  charbuffer_enqueue(0xc2dc); // 시
  charbuffer_enqueue(' ');
  if (time[1] == 0){
    charbuffer_enqueue(0xc815);charbuffer_enqueue(0xac01); // 정각
  } else{
    writenumberhan(time[1]);
    charbuffer_enqueue(0xbd84); // 분
  }
  writestring_progmem(CONST_STRING[1]); // 입니다
}

void displaytime(unsigned char* time){
  writenumber(time[0], 2);
  charbuffer_enqueue(':');
  writenumber(time[1], 2);
  charbuffer_enqueue(':');
  writenumber(time[2], 2);
}

void at_second(){
  if (timeroutine_time[2] % 10 == 0){
    if ((confflag & CONF_HANGULTIME) != 0) {
      displaytimehan(timeroutine_time);
    } else {
      displaytime(timeroutine_time);
    }
  }
}

void at_minute(){
}

void at_hour(){
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
