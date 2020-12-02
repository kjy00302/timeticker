extern const unsigned int* const PROGMEM CONST_STRING[];
extern const unsigned int PROGMEM CONST_HOUR[][3];
extern const unsigned int PROGMEM CONST_DIGIT[];

extern unsigned char confflag;

void displaytime_numeric(unsigned char* time){
  if ((confflag & CONF_24H) != 0){
    writenumber(time[0], 2, false);
  } else {
    if (time[0] < 12){
      charbuffer_enqueue('A');
    }
    else {
      charbuffer_enqueue('P');
    }
    charbuffer_enqueue('M');
    charbuffer_enqueue(' ');
    char t = time[0] % 12;
    writenumber((t == 0) ? 12 : t, 2, false);
  }
  charbuffer_enqueue(':');
  writenumber(time[1], 2, false);
  charbuffer_enqueue(':');
  writenumber(time[2], 2, false);
}

void displaytime_hangul(unsigned char* time){
  writestring_progmem(CONST_STRING[0]); // 지금은
  charbuffer_enqueue(0xc624); //오
  if (time[0] < 12){
    charbuffer_enqueue(0xc804); //전
  } else {
    charbuffer_enqueue(0xd6c4); //후
  }
  charbuffer_enqueue(' ');
  writestring_progmem(CONST_HOUR[time[0]%12]);
  charbuffer_enqueue(0xc2dc); // 시
  charbuffer_enqueue(' ');
  if (time[1] == 0){
    charbuffer_enqueue(0xc815);charbuffer_enqueue(0xac01); // 정각
  } else{
    writenumberhan(time[1]);
    charbuffer_enqueue(0xbd84); // 분
  }
  writestring_progmem(CONST_STRING[1]); // 입니다
}
