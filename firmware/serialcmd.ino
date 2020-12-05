extern unsigned char evtflag;
extern unsigned char confflag;

extern unsigned long display_buffer[];
extern unsigned int tickerroutine_message[];
extern unsigned char displayroutine_ms;

void cmdparse() {
  serialwait(2);
  if (Serial.read() == 0xff) {
    switch (Serial.read()) {

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

      case 0x03: {
          Serial.write("\xff\x02\x01");
          Serial.write(confflag);
          break;
        }

      case 0x04: {
          serialwait(1);
          unsigned char len = Serial.read();
          Serial.readBytes((byte*)tickerroutine_message, 2 * len);
          tickerroutine_message[len] = 0;
          Serial.write("\xff\x01");
          break;
        }

      case 0x11: {
          evtflag |= EVT_TIMESYNC_PENDING;
          Serial.write("\xff\x01");
          break;
        }

      case 0x12: {
          unsigned char buf[3];
          Serial.readBytes(buf, 3);
          time_setrtctime(buf[0], buf[1], buf[2]);
          Serial.write("\xff\x01");
          break;
        }

      case 0x13: {
          unsigned char buf[3];
          time_getrtctime(buf);
          Serial.write("\xff\x02\x03");
          Serial.write(buf, 3);
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
          unsigned int t;
          serialwait(1);
          for (unsigned char i = Serial.read(); i > 0; i--) {
            Serial.readBytes((byte*)&t, 2);
            if (!charbuffer_isfull()) {
              charbuffer_enqueue(t);
            }
          }
          evtflag |= EVT_CHAR_IN;
          Serial.write("\xff\x01");
          break;
        }

      case 0x24: {
          Serial.readBytes((unsigned char*)display_buffer, 64);
          display_update();
          Serial.write("\xff\x01");
          break;
        }

      case 0x25: {
          serialwait(1);
          displayroutine_ms = Serial.read();
          Serial.write("\xff\x01");
          break;
        }

      case 0x32: {
          unsigned int addr;
          unsigned char buf[16];
          Serial.readBytes((char*)&addr, 2);
          Serial.readBytes(buf, 16);
          eeprom_write(addr, buf, 16);
          while (!eeprom_isready());
          Serial.write("\xff\x01");
          break;
        }

      case 0x33: {
          unsigned int addr;
          unsigned char buf[32];
          Serial.readBytes((char*)&addr, 2);
          eeprom_read(addr, buf, 32);
          Serial.write("\xff\x02\x20");
          for (char i = 0; i < 32; i++) {
            Serial.write(buf[i]);
          }
          break;
        }

      case 0x41: {
          int temp = temperature_gettemperature();
          unsigned long humi = temperature_gethumidity();
          Serial.write("\xff\x02\x06");
          Serial.write((char*)&temp, 2);
          Serial.write((char*)&humi, 4);
          break;
        }

      case 0x42: {
          serialwait(1);
          evtflag = Serial.read();
          Serial.write("\xff\x01");
          break;
        }

      case 0x43: {
          Serial.write("\xff\x02\x01");
          Serial.write(evtflag);
          break;
        }

      case 0x44: {
          asm("jmp 0");
          break;
        }
    }
  }
}
