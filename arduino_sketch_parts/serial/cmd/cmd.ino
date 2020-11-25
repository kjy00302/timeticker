
unsigned int chrbuf[16] = {0,};
char chrbuf_frontptr = 0;
char chrbuf_backptr = 0;


void setup() {
  Serial.begin(9600);
}

void loop() {
  parsecmd();
  if (!chrbuf_isempty()){
    Serial.println(chrbuf_dequeue());
  }
}

void parsecmd(){
  if (Serial.available() > 1){
    if (Serial.read() == 0xff){
      switch (Serial.read()){
        
        case 0x01: // basic command test
          Serial.write("\xff\x01");
          break;
          
        case 0x02: // utf8 decoder test
          while (Serial.available() < 1);
          for (unsigned char i=Serial.read();i>0;i--){
            if (!chrbuf_isfull()){
              unsigned int t = utf8decode();
              chrbuf_enqueue(t);
            } else{
              break;
            }
          }
          break;
          
        case 0x03: // short char test
          while (Serial.available() < 1);
          for (unsigned char i=Serial.read();i>0;i--){
            if (!chrbuf_isfull()){
              unsigned int t;
              Serial.readBytes((byte*)&t, 2);
              chrbuf_enqueue(t);
            } else{
              break;
            }
          }
          break;
          
        case 0x04:  // time decode test
          while (Serial.available() < 3);
          {
          char sec = Serial.read();
          char minute = Serial.read();
          char hour = Serial.read();
          Serial.print(hour, DEC);
          Serial.print("hour ");
          Serial.print(minute, DEC);
          Serial.print("minute ");
          Serial.print(sec, DEC);
          Serial.println("second");
          }
          break;
          
        case 0x99: // resetting ¯\_(ツ)_/¯
          Serial.println("Resetting...");
          delay(1000);
          asm("jmp 0");
          break;
          
        default:
          Serial.println("Error!");
      }
    }
  }
}
