extern const unsigned int* const PROGMEM CONST_STRING[];

void displaytemphumi() {
  writestring_progmem(CONST_STRING[2]); // 온도는
  displaytemp();
  charbuffer_enqueue(',');
  charbuffer_enqueue(' ');
  writestring_progmem(CONST_STRING[3]); // 습도는
  displayhumi();
  writestring_progmem(CONST_STRING[1]); // 입니다
}

void displaytemp() {
  int t = temperature_gettemperature();
  writenumber(t / 100, 2, true);
  charbuffer_enqueue('.');
  writenumber(t % 100, 2, false);
  charbuffer_enqueue(0xf8); // <degree>
  charbuffer_enqueue('C');
}

void displayhumi() {
  writenumber(temperature_gethumidity() / 1024, 3, true);
  charbuffer_enqueue('%');
}
