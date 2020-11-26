#include <Wire.h>
#include <SPI.h>

extern char display_newbuf_left;
unsigned int i = 0;

void setup(){
  SPI.begin();
  Wire.begin();
  Serial.begin(9600);
  display_init();
}

void loop(){
  display_update();
  display_scroll();
  if ((!charbuffer_isempty()) && (display_newbuf_left == 0)){
    font_write(charbuffer_dequeue());
  }
  if (Serial.available()){
    cmdparse();
  }
  delay(10);
}
