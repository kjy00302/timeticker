#include <Wire.h>

void eeprom_read(unsigned int addr, char* data, char len){
  Wire.beginTransmission(0x50);
  Wire.write(addr >> 8);
  Wire.write(addr & 0xff);
  Wire.endTransmission();
  Wire.requestFrom(0x50, len);
  for (char i=0;i<len;i++){
    data[i] = Wire.read();
  }
}
