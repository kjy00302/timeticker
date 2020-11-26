const int HAN_OFFSET = 4096;

extern unsigned int display_new_buffer[];
extern char display_newbuf_left;

void font_writetonewbuf16(unsigned int addr){
  unsigned char fnt[16];
  eeprom_read(addr, fnt, 16);
  for (char i=0;i<16;i++){
    display_new_buffer[i] |= fnt[i] << 8;
  }
  display_newbuf_left = 8;
}

void font_writetonewbuf32(unsigned int addr){
  unsigned char fnt[32];
  eeprom_read(addr, fnt, 32);
  for (char i=0;i<16;i++){
    display_new_buffer[i] |= fnt[i*2] << 8;
    display_new_buffer[i] |= fnt[i*2+1];
  }
  display_newbuf_left = 16;
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
