const int DISPLAYROUTINE_MS = 20;
volatile unsigned int displayroutine_t = 0;
extern unsigned char confflag;

void displayroutine(){
  if (displayroutine_t > DISPLAYROUTINE_MS){
    if ((confflag & CONF_DISPLAY_SCROLL_UPDATE) != 0){
      if (!charbuffer_isempty() and display_newbuf_left == 0){
        font_write(charbuffer_dequeue());
        if (charbuffer_isempty()){
          Serial.write("\xff\x02");
        }
      }
      display_update();
      display_scroll();
    }
    displayroutine_t -= DISPLAYROUTINE_MS;
  }
}
