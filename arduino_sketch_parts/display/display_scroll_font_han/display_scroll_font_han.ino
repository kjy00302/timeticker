#include <SPI.h>
#include <Wire.h>

unsigned long disparr[16] ={0,};
unsigned int newbuf[16] = {0,};

unsigned int text[] = {0xd55c, 0xae00, 0xb3c4, 0x20, 0xb3fc, 0xc694, 0x21, 0x20}; // "한글도 돼요! "

char textind = 0;
char bufcnt = 0;

const int HAN_OFFSET = 4096;

char cho_tbl_1[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 3, 3, 1, 2, 4, 4, 4, 2, 1, 3, 0};
char cho_tbl_2[] = {0, 5, 5, 5, 5, 5, 5, 5, 5, 6, 7, 7, 7, 6, 6, 7, 7, 7, 6, 6, 7, 5};
char joong_tbl[] = {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1};
char jong_tbl[] = {0, 0, 2, 0, 2, 1, 2, 1, 2, 3, 0, 2, 1, 3, 3, 1, 2, 1, 3, 3, 1, 1};


const int M7219DIN = 11;
const int M7219LOAD = 10;
const int M7219CLK = 13;

void setup() {
  SPI.begin();
  Wire.begin();
  pinMode(M7219LOAD, OUTPUT);
  senddisplaycommand(8, 0x0f, 0x00);
  senddisplaycommand(8, 0x0a, 0x01);
  senddisplaycommand(8, 0x0c, 0x00);
  senddisplaycommand(8, 0x0b, 0x07);
  senddisplaycommand(8, 0x09, 0x00);
  updatedisplay();
  senddisplaycommand(8, 0x0c, 0x01);
}

void loop() {
  if (bufcnt == 0){
    if (0xac00 <= text[textind]){
      chrcopyhan(text[textind]);
      bufcnt = 16;
    } else {
      chrcopytobuf(text[textind]);
      bufcnt = 8;
      }
    textind = (textind + 1) % (sizeof(text)/sizeof(int));
   
  }
  
  for(int i=0;i<16;i++){
    disparr[i] = (disparr[i] << 1) | (newbuf[i] >> 15);
    newbuf[i] <<=1 ;
  }
  updatedisplay();
  bufcnt -= 1;
  delay(20);
}

void senddisplaycommand(byte number, byte addr, byte data){
  digitalWrite(M7219LOAD, LOW);
  for (byte i=0;i<number;i++){
    SPI.transfer(addr);
    SPI.transfer(data);
  }
  digitalWrite(M7219LOAD, HIGH);
}

void updatedisplay() {
  for(int y=0;y<8;y++){
    digitalWrite(M7219LOAD, LOW);
    for (int i=0;i<4;i++){
      SPI.transfer(y+1);
      SPI.transfer(((char*)disparr)[y*4+3-i]); 
    }
    for (int i=0;i<4;i++){
      SPI.transfer(y+1);
      SPI.transfer(((char*)disparr)[32+y*4+3-i]); 
    }
    digitalWrite(M7219LOAD, HIGH);
  }
}


void readdatamulti(unsigned int addr, char* data, int len) {
  Wire.beginTransmission(0x50);
  Wire.write((addr >> 8));
  Wire.write((addr & 0xff));
  Wire.endTransmission();
  Wire.requestFrom(0x50, len);
  for(int i=0;i<len;i++){
    data[i] = Wire.read();
  }
}

void chrcopytobuf(char chr){
  char fnt[16];
  readdatamulti(chr*16, fnt, 16);
  for(int i=0;i<16;i++){
    newbuf[i] |= fnt[i] << 8;
  }
}

void hancopytobuf(int idx){
  unsigned char fnt[32];
  readdatamulti(HAN_OFFSET+idx*32, fnt, 32);
  for(int i=0;i<16;i++){
    newbuf[i] |= fnt[i*2] << 8;
    newbuf[i] |= fnt[i*2+1];
  }
}

void splitjamo(unsigned int chr, char* jamo){
  chr -= 0xac00;
  jamo[0] = chr / (21*28);
  jamo[1] = chr % (21 * 28) / 28;
  jamo[2] = chr % 28;
}

void chrcopyhan(unsigned int chr){
  char jamo[3];
  splitjamo(chr, jamo);
  jamo[0]++; jamo[1]++;
  if (jamo[2] == 0){
    hancopytobuf(cho_tbl_1[jamo[1]]*20 + jamo[0]);
  } else {
    hancopytobuf(cho_tbl_2[jamo[1]]*20 + jamo[0]);
  }
  hancopytobuf(8*20 + (joong_tbl[jamo[0]] + (jamo[2] == 0 ? 0 : 2))*22 + jamo[1]);
  hancopytobuf(8*20 + 4*22 + jong_tbl[jamo[1]]*28 + jamo[2]);
}
