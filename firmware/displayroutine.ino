const int DISPLAYROUTINE_DEFAULT_MS = 20;
unsigned char displayroutine_ms = DISPLAYROUTINE_DEFAULT_MS;
volatile unsigned int displayroutine_t = 0;
extern unsigned char evtflag;
extern unsigned char confflag;

void displayroutine(){
  if (displayroutine_t > displayroutine_ms){
    if ((confflag & CONF_DISPLAY_SCROLL_UPDATE) != 0){
      if (!charbuffer_isempty() and display_newbuf_left == 0){
        font_write(charbuffer_dequeue());
        if (charbuffer_isempty() && ((evtflag & EVT_CHAR_IN) != 0)){
          evtflag ^= EVT_CHAR_IN;
          Serial.write("\xff\x02");
        }
      }
      display_update();
      display_scroll();
    }
    displayroutine_t -= displayroutine_ms;
  }
}
