#include <avr/pgmspace.h>

const char cho_tbl_1[] PROGMEM = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0};
const char cho_tbl_2[] PROGMEM = {0, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7, 7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5};
const char joong_tbl[] PROGMEM = {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1};
const char jong_tbl[] PROGMEM = {0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1};

void han844_splitjamo(unsigned int chr, char* jamo){
  chr -= 0xac00;
  jamo[0] = chr / (21*28);
  jamo[1] = chr % (21 * 28) / 28;
  jamo[2] = chr % 28;
}

void han844_getoffset(unsigned int chr, unsigned int* offset){
  char jamo[3];
  han844_splitjamo(chr, jamo);
  jamo[0]++; jamo[1]++;
  if (jamo[2] == 0){
    offset[0] = pgm_read_byte(&cho_tbl_1[jamo[1]])*20 + jamo[0];
  } else {
    offset[0] = pgm_read_byte(&cho_tbl_2[jamo[1]])*20 + jamo[0];
  }
  offset[1] = 8*20 + (pgm_read_byte(&joong_tbl[jamo[0]]) + (jamo[2] == 0 ? 0 : 2))*22 + jamo[1];
  offset[2] = 8*20 + 4*22 + pgm_read_byte(&jong_tbl[jamo[1]])*28 + jamo[2];
}
