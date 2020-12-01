extern unsigned long display_buffer[];
extern unsigned char confflag;

void cmdparse(){
  serialwait(2);
  if (Serial.read() == 0xff){
    switch (Serial.read()){

      case 0x01: {
        Serial.write("\xff\x01");
        break;
      }

      case 0x02: {
        serialwait(1);
        confflag = Serial.read();
        Serial.write("\xff\x01");
        break;
      }

      case 0x11: {
        time_sync();
        Serial.write("\xff\x01");
        break;
      }

      case 0x21: {
        charbuffer_reset();
        display_wipe();
        display_update();
        Serial.write("\xff\x01");
        break;
      }

      case 0x22: {
        serialwait(1);
        display_brightness(Serial.read());
        Serial.write("\xff\x01");
        break;
      }

      case 0x23: {
        serialwait(1);
        for (unsigned char i=Serial.read();i>0;i--){
          if (!charbuffer_isfull()){
            unsigned int t;
            Serial.readBytes((byte*)&t, 2);
            charbuffer_enqueue(t);
          }
        }
        Serial.write("\xff\x01");
        break;
      }

      case 0x24: {
          Serial.readBytes((unsigned char*)display_buffer, 64);
          display_update();
          break;
      }
    }
  }
}
