#include <Wire.h>

unsigned int temperature_dig_t1;
int temperature_dig_t2;
int temperature_dig_t3;

unsigned char temperature_dig_h1;
int temperature_dig_h2;
unsigned char temperature_dig_h3;
int temperature_dig_h4;
int temperature_dig_h5;
char temperature_dig_h6;

long temperature_t_fine;

char temperature_init() {
  if (temperature_readregister(0xd0) != 0x60) {
    return false;
  }
  temperature_writeregister(0xe0, 0xb6);
  delay(1000);
  temperature_writeregister(0xF2, 0b00000001);
  temperature_writeregister(0xF4, 0b00100011);
  temperature_dig_t1 = (temperature_readregister(0x89) << 8) | (temperature_readregister(0x88));
  temperature_dig_t2 = (int)((temperature_readregister(0x8b) << 8) | (temperature_readregister(0x8a)));
  temperature_dig_t3 = (int)((temperature_readregister(0x8d) << 8) | (temperature_readregister(0x8c)));

  temperature_dig_h1 = temperature_readregister(0xa1);
  temperature_dig_h2 = (int)((temperature_readregister(0xe2) << 8) | (temperature_readregister(0xe1)));
  temperature_dig_h3 = temperature_readregister(0xe3);
  temperature_dig_h4 = (int)((temperature_readregister(0xe4) << 4) | (temperature_readregister(0xe5) & 0x0f));
  temperature_dig_h5 = (int)((temperature_readregister(0xe6) << 4) | (temperature_readregister(0xe5) >> 4));
  temperature_dig_h6 = (char)temperature_readregister(0xe7);

}

int temperature_gettemperature() {
  long ut, var1, var2;
  ut = temperature_readregister(0xfa);
  ut <<= 8;
  ut |= temperature_readregister(0xfb);
  ut <<= 8;
  ut |= temperature_readregister(0xfc);
  ut >>= 4;

  // compensation formular from datasheet
  var1 = (((ut >> 3) - ((long)temperature_dig_t1 << 1)) * ((long)temperature_dig_t2)) >> 11;
  var2 = ((ut >> 4) - ((long)temperature_dig_t1));
  var2 = (((var2 * var2) >> 12) * ((long)temperature_dig_t3)) >> 14;
  temperature_t_fine = var1 + var2;
  return (temperature_t_fine * 5 + 128) >> 8;

}

unsigned long temperature_gethumidity() {
  long uh, v_x1_u32r;
  uh = temperature_readregister(0xfd);
  uh <<= 8;
  uh |= temperature_readregister(0xfe);

  // compensation formular from datasheet
  v_x1_u32r = (temperature_t_fine - ((long)76800));
  v_x1_u32r = (((((uh << 14) - (((long)temperature_dig_h4) << 20) - (((int32_t)temperature_dig_h5) * v_x1_u32r)) + ((long)16384)) >> 15) *
               (((((((v_x1_u32r * ((long)temperature_dig_h6)) >> 10) * (((v_x1_u32r * ((long)temperature_dig_h3)) >> 11) + ((long)32768))) >> 10) + ((long)2097152)) * ((long)temperature_dig_h2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r - (((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((long)temperature_dig_h1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
  v_x1_u32r = (v_x1_u32r > 419430400L ? 419430400L : v_x1_u32r);
  return (v_x1_u32r >> 12);
}

unsigned char temperature_readregister(unsigned char addr) {
  Wire.beginTransmission(0x76);
  Wire.write(addr);
  Wire.endTransmission();
  Wire.requestFrom(0x76, 1);
  return Wire.read();
}

void temperature_writeregister(unsigned char addr, unsigned char data) {
  Wire.beginTransmission(0x76);
  Wire.write(addr);
  Wire.write(data);
  Wire.endTransmission();
}
