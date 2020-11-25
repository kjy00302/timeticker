#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(4, 5); // RX, TX
String datetimestr;

const char ENABLEGPZDAONLY[] = {
  181, 98, 6, 1, 3, 0, 240, 5, 0, 255, 25, // disable vtg
  181, 98, 6, 1, 3, 0, 240, 0, 0, 250, 15, // disable gga
  181, 98, 6, 1, 3, 0, 240, 2, 0, 252, 19, // disable gsa
  181, 98, 6, 1, 3, 0, 240, 3, 0, 253, 21, // disable gsv
  181, 98, 6, 1, 3, 0, 240, 1, 0, 251, 17, // disable gll
  181, 98, 6, 1, 3, 0, 240, 4, 0, 254, 23, // disable rmc
  181, 98, 6, 1, 3, 0, 240, 8, 1, 3, 32 // enable zda
};

const char RESET[] = {
  181, 98, 6, 4, 4, 0, 0, 0, 0, 0, 14, 100 // reboot gps
};


void setup() {
  Serial.begin(9600);
  gpsSerial.begin(9600);
  /*
  delay(500);
  gpsSerial.write(RESET, sizeof(RESET));
  gpsSerial.flush();
  */
  delay(500);
  flush_rx(&gpsSerial);
  gpsSerial.write(ENABLEGPZDAONLY, sizeof(ENABLEGPZDAONLY));
  gpsSerial.flush();
  delay(500);
  flush_rx(&gpsSerial);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (gpsSerial.available()){
    //Serial.write(gpsSerial.read());
    
    datetimestr = gpsSerial.readStringUntil('\n');
    char firstcomma = datetimestr.indexOf(',');
    char secondcomma = datetimestr.indexOf(',', firstcomma+1);
    char thirdcomma = datetimestr.indexOf(',', secondcomma+1);
    char forthcomma = datetimestr.indexOf(',', thirdcomma+1);
    char fifthcomma = datetimestr.indexOf(',', forthcomma+1);
    String time = datetimestr.substring(firstcomma+1, secondcomma);
    
    if (fifthcomma - forthcomma > 1){
    int year = str2int(datetimestr.substring(forthcomma+1, fifthcomma), 4); //year
    int month = str2int(datetimestr.substring(thirdcomma+1, forthcomma),2); // month
    int date = str2int(datetimestr.substring(secondcomma+1, thirdcomma), 2); //date
    
    int hour = str2int(time.substring(0,2), 2);
    int minute = str2int(time.substring(2,4), 2);
    int second = sec2int(time.substring(4));
    Serial.print(year);
    Serial.print('-');
    Serial.print(month);
    Serial.print('-');
    Serial.print(date);
    Serial.print(' ');
    Serial.print(hour);
    Serial.print(':');
    Serial.print(minute);
    Serial.print(':');
    Serial.println(second);
    }
    
  }

}

void flush_rx(Stream *stream){
  while (stream->available()){
    stream->read();
  }
}

int sec2int(String str){
  return str2int(str.substring(0,2),2) + (str2int(str.substring(3),2) > 50 ? 1 : 0);
}

int str2int(String str, int n){
  int t=0;
  int e = 1;
  for (int i=0;i<n;i++){
    t += (str.charAt(n-1-i) - 0x30) * e;
    e *= 10;
  }
return t;
}
