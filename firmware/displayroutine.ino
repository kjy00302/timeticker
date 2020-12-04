extern unsigned char evtflag;
extern unsigned char confflag;

const int DISPLAYROUTINE_DEFAULT_MS = 20;
unsigned char displayroutine_ms = DISPLAYROUTINE_DEFAULT_MS;
volatile unsigned int displayroutine_t = 0;
char displayroutine_left_line = 0;

void displayroutine(){
  if (displayroutine_t > displayroutine_ms){
    if ((confflag & CONF_DISPLAY_SCROLL_UPDATE) != 0){
      if (!charbuffer_isempty() and display_newbuf_left == 0){
        font_write(charbuffer_dequeue());
        if (charbuffer_isempty() && ((evtflag & EVT_CHAR_IN) != 0)){
          evtflag ^= EVT_CHAR_IN;
          Serial.write("\xff\x02\x00");
        }
      }
      display_scroll();
      display_update();
      if (displayroutine_left_line > 0){
          displayroutine_left_line--;
          if (displayroutine_left_line == 0){
           evtflag |= EVT_DISPLAY_EMPTY;
         }
      }
    }
    displayroutine_t = 0;
  }
}
