void cmdparse(){
  serialwait(2);
  if (Serial.read() == 0xff){
    switch (Serial.read()){

      case 0x01: {
        Serial.write("\xff\x01");
        break;
      }

    }
  }
}
