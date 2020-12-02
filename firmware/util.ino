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

void writenumber(long n, unsigned char len, char skippadding){
  long e = 1;
  for (char i=0;i<len-1;i++){
    e *= 10;
  }
  for (char i=0;i<len;i++){
    if (!skippadding || ((n / e) % 10 != 0)){
      charbuffer_enqueue(0x30 + ((n / e) % 10));
      skippadding = true;
    }
    e /= 10;
  }
}

void writestring(unsigned int* arr){
  unsigned char i = 0;
  while (arr[i] != 0){
    charbuffer_enqueue(arr[i++]);
  }
}

void writestring_progmem(unsigned int* arr){
  unsigned char i = 0;
  unsigned int t = 0;
  while ((t = pgm_read_word(&(arr[i]))) != 0){
    charbuffer_enqueue(t);
    i++;
  }
}

void writenumberhan(unsigned char n) {
  if ((n/100)%10 != 0){
    if ((n/100)%10 != 1){
      charbuffer_enqueue(pgm_read_word(&CONST_DIGIT[(n/100)%10-1]));
    }
  charbuffer_enqueue(0xbc31); // 백
  }
  if ((n/10)%10 != 0){
    if ((n/10)%10 != 1){
      charbuffer_enqueue(pgm_read_word(&CONST_DIGIT[(n/10)%10-1]));
    }
  charbuffer_enqueue(0xc2ed); // 십
  }
  if (n%10 != 0){
    charbuffer_enqueue(pgm_read_word(&CONST_DIGIT[(n%10)-1]));
  }
}
