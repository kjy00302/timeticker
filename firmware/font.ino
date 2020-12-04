const int HAN_OFFSET = 4096;
const int CUSTOM_16_OFFSET = 16384;
const int CUSTOM_32_OFFSET = 24576;

extern unsigned int display_new_buffer[];
extern char display_newbuf_left;
extern char displayroutine_left_line;

void font_writetonewbuf16(unsigned int addr){
  unsigned char fnt[16];
  eeprom_read(addr, fnt, 16);
  for (char i=0;i<16;i++){
    display_new_buffer[i] |= fnt[i] << 8;
  }
  display_newbuf_left = 8;
  displayroutine_left_line = 40;
}

void font_writetonewbuf32(unsigned int addr){
  unsigned char fnt[32];
  eeprom_read(addr, fnt, 32);
  for (char i=0;i<16;i++){
    display_new_buffer[i] |= fnt[i*2] << 8;
    display_new_buffer[i] |= fnt[i*2+1];
  }
  display_newbuf_left = 16;
  displayroutine_left_line = 48;
}

void font_writeascii(unsigned char chr){
  font_writetonewbuf16(chr * 16);
}

void font_writehan(unsigned int chr){
  unsigned int offset[3];
  han844_getoffset(chr, offset);
  font_writetonewbuf32(HAN_OFFSET + offset[0] * 32);
  font_writetonewbuf32(HAN_OFFSET + offset[1] * 32);
  font_writetonewbuf32(HAN_OFFSET + offset[2] * 32);
}

void font_writecustom16(unsigned int chr){
  font_writetonewbuf16(CUSTOM_16_OFFSET + (chr - 0xf000) * 16);
}

void font_writecustom32(unsigned int chr){
  font_writetonewbuf32(CUSTOM_32_OFFSET + (chr - 0xf100) * 32);
}

void font_write(unsigned int chr){
  if (chr < 256){
    font_writeascii(chr);
  } else if (chr >= 0xac00 && chr < 0xd7a4) {
    font_writehan(chr);
  } else if (chr >= 0xf000 && chr < 0xf100){
    font_writecustom16(chr);
  } else if (chr >= 0xf100 && chr < 0xf180){
    font_writecustom32(chr);
  }
}
