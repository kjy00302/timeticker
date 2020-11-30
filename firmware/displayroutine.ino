const int DISPLAYROUTINE_MS = 20;
volatile unsigned int displayroutine_t = 0;

void displayroutine(){
  if (displayroutine_t > DISPLAYROUTINE_MS){
    if (!charbuffer_isempty() and display_newbuf_left == 0){
      font_writeascii(charbuffer_dequeue());
    }
    display_update();
    display_scroll();
    displayroutine_t -= DISPLAYROUTINE_MS;
  }
}
