#include <Wire.h>

char data[32];

void setup() {
  Wire.begin();
  Serial.begin(9600);
  Serial.println("eeprom rw test");
  //writedata(0, 0x12); // Single data write
  for(int i=0;i<32;i++){
    Serial.print(readdata(i), HEX); // Single data read
    Serial.print(' ');
  }
  Serial.println();
}

void loop() {
  if (Serial.available() > 0){
    memset(data, Serial.read(), 32);
    writedatamulti(0, data, 16);
    Serial.read();
    Serial.read();
    Serial.read();
    Serial.read();
  }
  if (isready()){
  } else {
    Serial.println("Writing");
  }
}

uint8_t readdata(unsigned int addr) {
  Wire.beginTransmission(0x50);
  Wire.write((addr >> 8));
  Wire.write((addr & 0xff));
  Wire.endTransmission();
  Wire.requestFrom(0x50, 1);
  while (Wire.available() < 1);
  return Wire.read();
}

char isready(){
  Wire.beginTransmission(0x50);
  return Wire.endTransmission() == 0;
}

void writedata(unsigned int addr, char data) {
  Wire.beginTransmission(0x50);
  Wire.write((addr >> 8));
  Wire.write((addr & 0xff));
  Wire.write(data);
  Wire.endTransmission();
}

void writedatamulti(unsigned int addr, char* data, int len) {
  Wire.beginTransmission(0x50);
  Wire.write((addr >> 8));
  Wire.write((addr & 0xff));
  Wire.write(data, len);
  Wire.endTransmission();
}
