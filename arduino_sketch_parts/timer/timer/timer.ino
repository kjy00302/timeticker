volatile int t = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(13, OUTPUT);
  noInterrupts();
  TCCR2A =  _BV(WGM21); // Set CTC mode
  TCCR2B = _BV(CS22); //Timer divider 64
  TCNT2 = 0;
  OCR2A = 249; // 16MHz(16000000) / 64 (timer2 divider) / 1000 (1000Hz) - 1
  TIMSK2 |= _BV(OCIE2A); // Enable Timer2 Compare Interrupt A
  interrupts();

}

void loop() {
  // put your main code here, to run repeatedly:
  if(t > 1000){
    t -= 1000;
    digitalWrite(13, !digitalRead(13));
  }

}

ISR(TIMER2_COMPA_vect){
  // Runs every 1 millisecond
  t += 1;
}
