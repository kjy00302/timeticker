int brightness = 0;

void brightness_init() {
  pinMode(A0, INPUT);
  brightness = analogRead(A0);
}

void brightness_update() {
  brightness = (brightness + analogRead(A0)) / 2;
  if (brightness > 30) {
    display_brightness(0x04);
  } else {
    display_brightness(0x00);
  }
}
