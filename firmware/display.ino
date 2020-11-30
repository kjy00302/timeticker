#include <SPI.h>

const int M7219DIN = 11;
const int M7219LOAD = 10;
const int M7219CLK = 13;

unsigned long display_buffer[16] ={0,};
unsigned int display_new_buffer[16] = {0,};

char display_newbuf_left = 0;

void display_init(){
  pinMode(M7219LOAD, OUTPUT);
  // TODO: Remove magic number
  display_sendcommand(0x0f, 0x00);
  display_sendcommand(0x0a, 0x01);
  display_sendcommand(0x0c, 0x00);
  display_sendcommand(0x0b, 0x07);
  display_sendcommand(0x09, 0x00);
  display_update();
  display_sendcommand(0x0c, 0x01);
}

void display_sendcommand(byte addr, byte data){
  digitalWrite(M7219LOAD, LOW);
  for (byte i=0;i<8;i++){
    SPI.transfer(addr);
    SPI.transfer(data);
  }
  digitalWrite(M7219LOAD, HIGH);
}

void display_update() {
  for(int y=0;y<8;y++){
    digitalWrite(M7219LOAD, LOW);
    for (int i=0;i<4;i++){
      SPI.transfer(y+1);
      SPI.transfer(((char*)display_buffer)[y*4+3-i]); 
    }
    for (int i=0;i<4;i++){
      SPI.transfer(y+1);
      SPI.transfer(((char*)display_buffer)[32+y*4+3-i]); 
    }
    digitalWrite(M7219LOAD, HIGH);
  }
}

void display_scroll(){
  for(int i=0;i<16;i++){
    display_buffer[i] = (display_buffer[i] << 1) | (display_new_buffer[i] >> 15);
    display_new_buffer[i] <<=1 ;
  }
  if (display_newbuf_left > 0){
    display_newbuf_left--;
  }
}

void display_brightness(unsigned char brightness){
  if (brightness < 16){
    display_sendcommand(0x0a, brightness);
  }
}

void display_wipe(){
  memset((char*)display_buffer, 0, 64);
  memset((char*)display_new_buffer, 0, 64);
  display_newbuf_left = 0;
}
