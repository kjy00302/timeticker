
void ds1302_setsecond(char s){
  ds1302_writertcregister(0x80, bcdencode(s));
}

void ds1302_setminute(char m){
  ds1302_writertcregister(0x82, bcdencode(m));
}

void ds1302_sethour24(char h){
  ds1302_writertcregister(0x84, bcdencode(h));
}

void ds1302_sethour12(char h, char ispm){
  ds1302_writertcregister(0x84, bcdencode(h) | 0x80 | (ispm ? 0x20 : 0));
}

void ds1302_setdate(char d){
  ds1302_writertcregister(0x86, bcdencode(d));
}

void ds1302_setmonth(char m){
  ds1302_writertcregister(0x88, bcdencode(m));
}

void ds1302_setday(char d){
  ds1302_writertcregister(0x8a, d); // 1 = Sunday
}

void ds1302_setyear(char d){
  ds1302_writertcregister(0x8c, bcdencode(d));
}

char ds1302_readrtcregister(char addr){
  digitalWrite(DS1302_CE, HIGH);
  pinMode(DS1302_IO, OUTPUT);
  shiftOut(DS1302_IO, DS1302_CLK, LSBFIRST, addr);
  pinMode(DS1302_IO, INPUT);
  char v = shiftIn(DS1302_IO, DS1302_CLK, LSBFIRST);
  digitalWrite(DS1302_CE, LOW);
  return v;
}

void ds1302_writertcregister(char addr, char value){
  digitalWrite(DS1302_CE, HIGH);
  pinMode(DS1302_IO, OUTPUT);
  shiftOut(DS1302_IO, DS1302_CLK, LSBFIRST, addr);
  shiftOut(DS1302_IO, DS1302_CLK, LSBFIRST, value);
  
  digitalWrite(DS1302_CE, LOW);
}

void ds1302_readrtcburst(char* arr, char addr, char n){
  digitalWrite(DS1302_CE, HIGH);
  pinMode(DS1302_IO, OUTPUT);
  shiftOut(DS1302_IO, DS1302_CLK, LSBFIRST, addr);
  pinMode(DS1302_IO, INPUT);
  for(int i=0;i<n;i++){
    arr[i] = shiftIn(DS1302_IO, DS1302_CLK, LSBFIRST);
  }
  digitalWrite(DS1302_CE, LOW);
}

void ds1302_writertcburst(char* arr, char addr, char n){
  digitalWrite(DS1302_CE, HIGH);
  pinMode(DS1302_IO, OUTPUT);
  shiftOut(DS1302_IO, DS1302_CLK, LSBFIRST, addr);
  for(int i=0;i<n;i++){
    shiftOut(DS1302_IO, DS1302_CLK, LSBFIRST, arr[i]);
  }
  digitalWrite(DS1302_CE, LOW);
}
