
// dirty utf-8 decoder
unsigned int utf8decode() {
  while (Serial.available() < 1);
  unsigned int b = Serial.read();
  if (b < 0x80){
    return b;
  } else if (b < 0xdf) {
    while (Serial.available() < 1);
    b <<= 6;
    b |= Serial.read() & 0x3f;
    return b & 0x7ff;
  } else if (b < 0xef) {
    while (Serial.available() < 2);
    b <<= 6;
    b |= Serial.read() & 0x3f;
    b <<= 6;
    b |= Serial.read() & 0x3f;
    return b & 0xffff;
  }
}
