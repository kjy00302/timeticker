extern volatile unsigned int timeroutine_t;
extern volatile unsigned int displayroutine_t;

void timer_init() {
  noInterrupts();
  TCCR2A =  _BV(WGM21); // Set CTC mode
  TCCR2B = _BV(CS22); //Timer divider 64
  TCNT2 = 0;
  OCR2A = 249; // 16MHz(16000000) / 64 (timer2 divider) / 1000 (1000Hz) - 1
  TIMSK2 |= _BV(OCIE2A); // Enable Timer2 Compare Interrupt A
  interrupts();
}

ISR(TIMER2_COMPA_vect) {
  // Runs every 1 millisecond
  timeroutine_t++;
  displayroutine_t++;
}
