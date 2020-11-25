#include <SPI.h>

const int M7219DIN = 11;
const int M7219LOAD = 10;
const int M7219CLK = 13;

unsigned long disparr[16] ={0,};
int cdsvalue = 0;

void setup() {
  SPI.begin();
  Serial.begin(9600);
  pinMode(A0, INPUT);
  memset(disparr, 0b10101010, 64);
  senddisplaycommand(8, 0x0f, 0x00);
  senddisplaycommand(8, 0x0a, 0x01);
  senddisplaycommand(8, 0x0c, 0x00);
  senddisplaycommand(8, 0x0b, 0x07);
  senddisplaycommand(8, 0x09, 0x00);
  updatedisplay();
  senddisplaycommand(8, 0x0c, 0x01);
}

void loop() {
  cdsvalue = (cdsvalue + analogRead(A0)) / 2;
  Serial.println(cdsvalue);
  if (cdsvalue > 40){
    senddisplaycommand(8, 0x0a, 0x02);
  } else {
    senddisplaycommand(8, 0x0a, 0x00);
  }
  delay(500);
}


void senddisplaycommand(byte number, byte addr, byte data){
  digitalWrite(M7219LOAD, LOW);
  for (byte i=0;i<number;i++){
    SPI.transfer(addr);
    SPI.transfer(data);
  }
  digitalWrite(M7219LOAD, HIGH);
}

void updatedisplay() {
  for(int y=0;y<8;y++){
    digitalWrite(M7219LOAD, LOW);
    for (int i=0;i<4;i++){
      SPI.transfer(y+1);
      SPI.transfer(((char*)disparr)[y*4+3-i]); 
    }
    for (int i=0;i<4;i++){
      SPI.transfer(y+1);
      SPI.transfer(((char*)disparr)[32+y*4+3-i]); 
    }
    digitalWrite(M7219LOAD, HIGH);
  }
}
