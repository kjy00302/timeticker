#include <SoftwareSerial.h>
#include <avr/pgmspace.h>

const int RTC_CLK = 9;
const int RTC_DIO = 8;
const int RTC_EN = 7;

const char ENABLEGPZDAONLY[] PROGMEM  = {
  181, 98, 6, 1, 3, 0, 240, 5, 0, 255, 25, // disable vtg
  181, 98, 6, 1, 3, 0, 240, 0, 0, 250, 15, // disable gga
  181, 98, 6, 1, 3, 0, 240, 2, 0, 252, 19, // disable gsa
  181, 98, 6, 1, 3, 0, 240, 3, 0, 253, 21, // disable gsv
  181, 98, 6, 1, 3, 0, 240, 1, 0, 251, 17, // disable gll
  181, 98, 6, 1, 3, 0, 240, 4, 0, 254, 23, // disable rmc
  181, 98, 6, 1, 3, 0, 240, 8, 1, 3, 32 // enable zda
};

SoftwareSerial gpsSerial = SoftwareSerial(4, 5);

void time_init(){
  pinMode(RTC_EN, OUTPUT);
  pinMode(RTC_CLK, OUTPUT);
  digitalWrite(RTC_EN, LOW);
  digitalWrite(RTC_CLK, LOW);
  gpsSerial.begin(9600);
  for (char i=0;i<77;i++){
    gpsSerial.write(pgm_read_byte(&ENABLEGPZDAONLY[i]));
  }
  gpsSerial.end();
}

char time_getgpstime(unsigned char* time){
  gpsSerial.begin(9600);
  flush_rx(&gpsSerial);
  String t = gpsSerial.readStringUntil('\n');
  gpsSerial.end();
  char firstcomma = t.indexOf(',');
  if (firstcomma == -1){
    return false;
  }
  char secondcomma = t.indexOf(',', firstcomma+1);
  String timestr = t.substring(firstcomma+1, secondcomma);
  time[0] = str2int(timestr.substring(0,2), 2);
  time[1] = str2int(timestr.substring(2,4), 2);
  time[2] = sec2int(timestr.substring(4));
  return true;
}

unsigned char sec2int(String str){
  return str2int(str.substring(0,2),2) + (str2int(str.substring(3),2) > 50 ? 1 : 0);
}

unsigned char time_readrtcregister(unsigned char addr){
  pinMode(RTC_DIO, OUTPUT);
  digitalWrite(RTC_EN, HIGH);
  shiftOut(RTC_DIO, RTC_CLK, LSBFIRST, addr);
  pinMode(RTC_DIO, INPUT);
  unsigned char t = shiftIn(RTC_DIO, RTC_CLK, LSBFIRST);
  digitalWrite(RTC_EN, LOW);
  return t;
}

void time_sync(){
  unsigned char time[3];
  if (time_getgpstime(time)){
    time_setrtctime((time[0]+9)%24, time[1], time[2]);
  }
}

void time_writertcregister(unsigned char addr, unsigned char val){
  pinMode(RTC_DIO, OUTPUT);
  digitalWrite(RTC_EN, HIGH);
  shiftOut(RTC_DIO, RTC_CLK, LSBFIRST, addr);
  shiftOut(RTC_DIO, RTC_CLK, LSBFIRST, val);
  digitalWrite(RTC_EN, LOW);
}

void time_getrtctime(unsigned char* time){
  time[2] = bcddecode(time_readrtcregister(0x81) & 0x7f);
  time[1] = bcddecode(time_readrtcregister(0x83) & 0x7f);
  time[0] = bcddecode(time_readrtcregister(0x85) & 0x3f);
}

void time_setrtctime(unsigned char h, unsigned char m, unsigned char s){
  time_writertcregister(0x84, bcdencode(h));
  time_writertcregister(0x82, bcdencode(m));
  time_writertcregister(0x80, bcdencode(s));
}
