const int DS1302_CE = 10;
const int DS1302_IO = 11;
const int DS1302_CLK = 13;

char rtcram[31] = {0,};
char rtcdata[8] = {0,};

void setup() {
  pinMode(DS1302_CE, OUTPUT);
  pinMode(DS1302_CLK, OUTPUT);
  digitalWrite(DS1302_CE, LOW);
  digitalWrite(DS1302_CLK, LOW);
  Serial.begin(9600);
  /*
  sethour12(10, true);
  setminute(16);
  setsecond(30);
  setdate(16);
  setday(2);
  setmonth(11);
  setyear(20);
  */
  
}

void loop() {
  ds1302_readrtcburst(rtcdata, 0xbf, 8);
  for (int i=0;i<8;i++){
    Serial.print(rtcdata[i], HEX);
    Serial.print(' ');
  }
  Serial.print("\nRTC ");
  if (rtcdata[0] & 0x80){
    Serial.println("ON");
  } else {
    Serial.print("ON ");
  }
  
  Serial.print(bcddecode(rtcdata[0] & 0x7f));
  Serial.print("Sec ");
  
  Serial.print(bcddecode(rtcdata[1]));
  Serial.print("Min ");
  
  if (rtcdata[2] & 0x80) {
    if (rtcdata[2] & 0x20){
      Serial.print("PM ");
    }
    else {
      Serial.print("AM ");
    }
    Serial.print(bcddecode(rtcdata[2] & 0x1f));
  } else {
    Serial.print("24H Mode ");
    Serial.print(bcddecode(rtcdata[2] & 0x3f));
  }
  Serial.print("Hour ");

  Serial.print(bcddecode(rtcdata[3] & 0x3f));
  Serial.print("Day ");
  
  Serial.print(bcddecode(rtcdata[4] & 0x1f));
  Serial.print("Month ");
  
  Serial.print(bcddecode(rtcdata[6])+2000);
  Serial.println("Year");
  delay(1000);

}
