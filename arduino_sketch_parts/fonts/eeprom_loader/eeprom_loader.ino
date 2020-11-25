#include <Wire.h>

char data[16];
int addr;

void setup() {
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  if (Serial.available() > 1){
    if (Serial.read() == 0xff){
      switch (Serial.read()){
        
        case 0x01: // basic command test
          Serial.write("\xff\x01");
          break;
        case 0x32: // write eeprom (16byte)
          Serial.readBytes((char*)&addr, 2);
          Serial.readBytes(data, 16);
          writedatamulti(addr, data, 16);
          while(!isready());
          Serial.write("\xff\x01");
          break;
        case 0x33: // read eeprom (16byte)
          Serial.readBytes((char*)&addr, 2);
          readdatamulti(addr, data, 16);
          Serial.write("\xff\x02\x10");
          Serial.write(data, 16);
          break;
      }
    }
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
