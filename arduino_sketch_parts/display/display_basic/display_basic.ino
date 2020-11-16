#include <SPI.h>

#include <avr/pgmspace.h>

char disparr[64] ={0,};
const int M7219DIN = 11;
const int M7219LOAD = 10;
const int M7219CLK = 13;

const char font_0[] PROGMEM = {0x18,0x24,0x24,0x24,0x24,0x24,0x24,0x18};
const char font_1[] PROGMEM = {0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x00};
const char font_2[] PROGMEM = {0x18,0x04,0x04,0x18,0x20,0x20,0x20,0x18};
const char font_3[] PROGMEM = {0x18,0x04,0x04,0x18,0x04,0x04,0x04,0x18};
const char font_4[] PROGMEM = {0x00,0x24,0x24,0x18,0x04,0x04,0x04,0x00};
const char font_5[] PROGMEM = {0x18,0x20,0x20,0x18,0x04,0x04,0x04,0x18};
const char font_6[] PROGMEM = {0x18,0x20,0x20,0x18,0x24,0x24,0x24,0x18};
const char font_7[] PROGMEM = {0x18,0x24,0x24,0x00,0x04,0x04,0x04,0x00};

const char* const  fonts[] PROGMEM = {font_0,font_1,font_2,font_3,font_4,font_5,font_6,font_7};

void setup() {
  SPI.begin();
  pinMode(M7219LOAD, OUTPUT);
  Serial.begin(9600);
  
  // put your setup code here, to run once:
  for(int i=0;i<8;i++){
    memcpy_P(disparr+i*8, pgm_read_byte(&(fonts[i])), 8);
  }
  //disparr[57] = 0xff;
  senddisplaycommand(8, 0x0f, 0x00);
  senddisplaycommand(8, 0x0a, 0x01);
  senddisplaycommand(8, 0x0c, 0x00);
  senddisplaycommand(8, 0x0b, 0x07);
  senddisplaycommand(8, 0x09, 0x00);
  updatedisplay();
  senddisplaycommand(8, 0x0c, 0x01);
  for(int i=0;i<64;i++){
    Serial.print(disparr[i], HEX);
    Serial.print(' ');
  }
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:

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
    for (int i=0;i<8;i++){
      SPI.transfer(y+1);
      SPI.transfer(disparr[i*8+y]); // 한번 전송에 y축 하나씩 총 8번 전송
    }
    digitalWrite(M7219LOAD, HIGH);
  }
}
