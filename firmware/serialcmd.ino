extern unsigned long display_buffer[];


void cmdparse(){
  serialwait(2);
  if (Serial.read() == 0xff){
    switch (Serial.read()){

      case 0x01: {
        Serial.write("\xff\x01");
        break;
      }

      case 0x21: {
        memset((char*)display_buffer, 0, 64);
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
    }
  }
}
