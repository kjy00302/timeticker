#include <SPI.h>
#include <Wire.h>

unsigned long disparr[16] ={0,};
unsigned int newbuf[16] = {0,};

char text[] = "EEPROM Font scroll!";
char textind = 0;
char bufcnt = 0;

const int M7219DIN = 11;
const int M7219LOAD = 10;
const int M7219CLK = 13;

void setup() {
  SPI.begin();
  Wire.begin();
  pinMode(M7219LOAD, OUTPUT);
  
  senddisplaycommand(8, 0x0f, 0x00);
  senddisplaycommand(8, 0x0a, 0x01);
  senddisplaycommand(8, 0x0c, 0x00);
  senddisplaycommand(8, 0x0b, 0x07);
  senddisplaycommand(8, 0x09, 0x00);
  updatedisplay();
  senddisplaycommand(8, 0x0c, 0x01);
}

void loop() {
  if (bufcnt == 0){
    chrcopytobuf(text[textind]);
    textind = (textind + 1) % sizeof(text);
    bufcnt = 8;
  }
  
  for(int i=0;i<16;i++){
    disparr[i] = (disparr[i] << 1) | (newbuf[i] >> 15);
    newbuf[i] <<=1 ;
  }
  updatedisplay();
  bufcnt -= 1;
  delay(50);
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


void readdatamulti(unsigned int addr, unsigned char* data, int len) {
  Wire.beginTransmission(0x50);
  Wire.write((addr >> 8));
  Wire.write((addr & 0xff));
  Wire.endTransmission();
  Wire.requestFrom(0x50, len);
  for(int i=0;i<len;i++){
    data[i] = Wire.read();
  }
}

void chrcopytobuf(char chr){
  unsigned char fnt[16];
  readdatamulti(chr*16, fnt, 16);
  for(int i=0;i<16;i++){
    newbuf[i] = fnt[i] << 8;
  }
}
