#include <Wire.h>
#include <SPI.h>

extern char display_newbuf_left;
unsigned int i = 0;

void setup(){
  SPI.begin();
  Wire.begin();
  display_init();
}

void loop(){
  display_update();
  display_scroll();
  if (display_newbuf_left == 0){
    //font_writeascii(i);
    //i = (i+1) % 256;
    
    font_writehan(0xac00+i);
    i = (i+1) % 11172;
    
  }
  delay(10);
}
