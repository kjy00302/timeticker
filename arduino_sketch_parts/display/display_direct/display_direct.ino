#include <SPI.h>

#include <avr/pgmspace.h>

unsigned long disparr[16] ={0,};
const int M7219DIN = 11;
const int M7219LOAD = 10;
const int M7219CLK = 13;

void setup() {
  SPI.begin();
  pinMode(M7219LOAD, OUTPUT);
  Serial.begin(115200);
  senddisplaycommand(8, 0x0f, 0x00);
  senddisplaycommand(8, 0x0a, 0x01);
  senddisplaycommand(8, 0x0c, 0x00);
  senddisplaycommand(8, 0x0b, 0x07);
  senddisplaycommand(8, 0x09, 0x00);
  updatedisplay();
  senddisplaycommand(8, 0x0c, 0x01);
}

void loop() {
  if (Serial.available() > 1){
    if (Serial.read() == 0xff){
      switch (Serial.read()){
        case 0x24:
          Serial.readBytes((unsigned char*)disparr, 64);
          updatedisplay();
          break;
        case 0x25:
          memset(disparr, 0, 64);
          break;
      }
    }
  }

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
