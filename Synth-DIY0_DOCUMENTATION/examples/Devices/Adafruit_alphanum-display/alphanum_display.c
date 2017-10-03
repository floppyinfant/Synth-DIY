/** alphanumeric Display */
// 0.54", 14 segments, 4 letters
// I2C (Chip on PCB)
// https://learn.adafruit.com/adafruit-led-backpack/0-54-alphanumeric
// File > Examples > Adafruit_LEDBackpack > quadalphanum

/** INIT */
void initAlpha() {

Adafruit_AlphaNum4 alpha4 = Adafruit_AlphaNum4();

}

/** FUNCTIONS */

void alphaWriteWellcome() {
  char displaybuffer[4] = {'M', 'A', 'D', 'S'};

  // set every digit to the buffer
  alpha4.writeDigitAscii(0, displaybuffer[0]);
  alpha4.writeDigitAscii(1, displaybuffer[1]);
  alpha4.writeDigitAscii(2, displaybuffer[2]);
  alpha4.writeDigitAscii(3, displaybuffer[3]);

  // write it out!
  alpha4.writeDisplay();
  //delay(2000);
}

void alphaReadSerial() {
  char displaybuffer[4] = {' ', ' ', ' ', ' '};

  while (! Serial.available()) return;

  char c = Serial.read();
  if (! isprint(c)) return; // only printable!

  // scroll down display
  displaybuffer[0] = displaybuffer[1];
  displaybuffer[1] = displaybuffer[2];
  displaybuffer[2] = displaybuffer[3];
  displaybuffer[3] = c;

  // set every digit to the buffer
  alpha4.writeDigitAscii(0, displaybuffer[0]);
  alpha4.writeDigitAscii(1, displaybuffer[1]);
  alpha4.writeDigitAscii(2, displaybuffer[2]);
  alpha4.writeDigitAscii(3, displaybuffer[3]);

  // write it out!
  alpha4.writeDisplay();
  delay(200);
}
// alpha
