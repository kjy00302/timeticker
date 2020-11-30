unsigned char bcdencode(unsigned char b){
  return (b / 10) * 16 + (b % 10);
}

unsigned char bcddecode(unsigned char b){
  return (b >> 4) * 10 + (b & 0x0f);
}

void flush_rx(Stream *stream){
  while (stream->available()){
    stream->read();
  }
}

int str2int(String str, int n){
  int t=0;
  int e = 1;
  for (int i=0;i<n;i++){
    t += (str.charAt(n-1-i) - 0x30) * e;
    e *= 10;
  }
return t;
}

void serialwait(char i){
  while (Serial.available() < i);
}

void writenumber(long n, unsigned char len){
  long e = 1;
  for (char i=0;i<len-1;i++){
    e *= 10;
  }
  for (char i=0;i<len;i++){
    charbuffer_enqueue(0x30 + ((n / e) % 10));
    e /= 10;
  }
}
