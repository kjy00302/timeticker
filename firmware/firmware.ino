#include <Wire.h>
#include <SPI.h>

const unsigned char EVT_SECOND = _BV(0);
const unsigned char EVT_MINUTE = _BV(1);
const unsigned char EVT_HOUR = _BV(2);
const unsigned char EVT_CHAR_IN = _BV(3);
const unsigned char EVT_TIMESYNC_PENDING = _BV(4);
const unsigned char EVT_DISPLAY_EMPTY = _BV(5);

const unsigned char CONF_DISPLAY_SCROLL_UPDATE = _BV(0);
const unsigned char CONF_CLOCK_UPDATE = _BV(1);
const unsigned char CONF_AUTO_BRIGHTNESS = _BV(4);
const unsigned char CONF_TIME_AUTOSYNC = _BV(5);
const unsigned char CONF_HANGULTIME = _BV(6);
const unsigned char CONF_24H = _BV(7);

unsigned char evtflag = 0;
unsigned char confflag =
  CONF_DISPLAY_SCROLL_UPDATE |
  CONF_CLOCK_UPDATE |
  CONF_AUTO_BRIGHTNESS |
  CONF_TIME_AUTOSYNC |
  CONF_HANGULTIME ;

void setup() {
  timer_init();
  SPI.begin();
  Wire.begin();
  Serial.begin(115200);
  display_init();
  temperature_init();
  time_init();
  brightness_init();
  evtflag |= EVT_DISPLAY_EMPTY;
}

void at_second() {
  if ((confflag & CONF_AUTO_BRIGHTNESS) != 0) {
    brightness_update();
  }
}

void at_minute() {
}

void at_hour() {
  if ((confflag & CONF_TIME_AUTOSYNC) != 0) {
    evtflag |= EVT_TIMESYNC_PENDING;
  }
}

void loop() {

  if (Serial.available()) {
    cmdparse();
  }

  timeroutine();

  if ((confflag & CONF_CLOCK_UPDATE) != 0) {
    if ((evtflag & EVT_SECOND) != 0) {
      at_second();
      evtflag ^= EVT_SECOND;
    }

    if ((evtflag & EVT_MINUTE) != 0) {
      at_minute();
      evtflag ^= EVT_MINUTE;
    }

    if ((evtflag & EVT_HOUR) != 0) {
      at_hour();
      evtflag ^= EVT_HOUR;
    }
  }

  if ((evtflag & EVT_DISPLAY_EMPTY) != 0) {
    if ((confflag & CONF_CLOCK_UPDATE) != 0) {
      if ((evtflag & EVT_TIMESYNC_PENDING) != 0) {
        if (time_sync()) {
          evtflag ^= EVT_TIMESYNC_PENDING;
        }
      }
      evtflag ^= EVT_DISPLAY_EMPTY;
      tickerroutine();
    }
  }

  displayroutine();
}
