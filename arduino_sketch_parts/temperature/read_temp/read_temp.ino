#include <Wire.h>

uint16_t dig_t1;
int16_t dig_t2;
int16_t dig_t3;

uint8_t dig_h1;
int16_t dig_h2;
uint8_t dig_h3;
int16_t dig_h4;
int16_t dig_h5;
int8_t dig_h6;

int32_t t_fine;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  Serial.println("tempread test");
  if (readdata(0xd0) == 0x60){
    Serial.println("Comm OK");
  }
  writedata(0xe0, 0xb6);
  delay(1000);
  writedata(0xF2, 0b00000001);
  writedata(0xF4, 0b00100011);
  //writedata(0xF5, 0b11110000);
  dig_t1 = (readdata(0x89) << 8) | (readdata(0x88));
  dig_t2 = (int16_t)((readdata(0x8b) << 8) | (readdata(0x8a)));
  dig_t3 = (int16_t)((readdata(0x8d) << 8) | (readdata(0x8c)));

  dig_h1 = readdata(0xa1);
  dig_h2 = (int16_t)((readdata(0xe2) << 8) | (readdata(0xe1)));
  dig_h3 = readdata(0xe3);
  dig_h4 = (int16_t)((readdata(0xe4) << 4) | (readdata(0xe5) & 0x0f));
  dig_h5 = (int16_t)((readdata(0xe6) << 4) | (readdata(0xe5) >> 4));
  dig_h6 = (int8_t)readdata(0xe7);
  
}

void loop() {
  int16_t t = readtemp();
  Serial.print(t/100);
  Serial.print('.');
  Serial.print(t%100);
  Serial.println("C");
  uint32_t h = readhumi();
  Serial.print(h/1024);
  Serial.println("%H");
  delay(1000);
}

uint32_t readhumi(){
  int32_t uh, v_x1_u32r;
  uh = readdata(0xfd);
  uh <<= 8;
  uh |= readdata(0xfe);
  
  // compensation formular from datasheet
  v_x1_u32r = (t_fine -((int32_t)76800));
  v_x1_u32r = (((((uh << 14) -(((int32_t)dig_h4) << 20) - (((int32_t)dig_h5) * v_x1_u32r)) + ((int32_t)16384)) >> 15) *
  (((((((v_x1_u32r * ((int32_t)dig_h6)) >> 10) * (((v_x1_u32r * ((int32_t)dig_h3)) >> 11) + ((int32_t)32768))) >> 10) + ((int32_t)2097152)) * ((int32_t)dig_h2) + 8192) >> 14));
  v_x1_u32r = (v_x1_u32r -(((((v_x1_u32r >> 15) * (v_x1_u32r >> 15)) >> 7) * ((int32_t)dig_h1)) >> 4));
  v_x1_u32r = (v_x1_u32r < 0 ? 0 : v_x1_u32r);
  v_x1_u32r = (v_x1_u32r > 419430400L? 419430400L: v_x1_u32r);
  return (v_x1_u32r>>12); 
}

int readtemp(){
  int32_t ut, var1, var2;
  ut = readdata(0xfa);
  ut <<= 8;
  ut |= readdata(0xfb);
  ut <<= 8;
  ut |= readdata(0xfc);
  ut >>= 4;
  
  // compensation formular from datasheet
  var1 = (((ut >> 3) - ((int32_t)dig_t1 << 1)) * ((int32_t)dig_t2)) >> 11;
  var2 = ((ut >> 4) - ((int32_t)dig_t1));
  var2 = (((var2 * var2) >>12)* ((int32_t)dig_t3)) >> 14;
  t_fine = var1+var2;
  return (t_fine * 5 + 128) >> 8;
  
}

uint8_t readdata(uint8_t addr) {
  Wire.beginTransmission(0x76);
  Wire.write(addr);
  Wire.endTransmission();
  Wire.requestFrom(0x76, 1);
  return Wire.read();
}

void writedata(uint8_t addr, uint8_t data) {
  Wire.beginTransmission(0x76);
  Wire.write(addr);
  Wire.write(data);
  Wire.endTransmission();
}

void readdatabuf(uint8_t addr, uint8_t* buf, int len) {
  Wire.beginTransmission(0x76);
  Wire.write(addr);
  Wire.endTransmission();
  Wire.requestFrom(0x76, len);

  for (int i=0;i<len;i++){
    buf[i] = Wire.read();
  }
}
