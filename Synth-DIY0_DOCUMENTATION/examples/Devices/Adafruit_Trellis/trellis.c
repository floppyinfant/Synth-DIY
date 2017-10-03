/** Adafruit Trellis */
// Button-Array
// LED-Array
// I2C
//
// https://learn.adafruit.com/adafruit-trellis-diy-open-source-led-keypad/overview
// https://github.com/adafruit/Adafruit_Trellis_Library
// Examples
// https://learn.adafruit.com/mini-untztrument-3d-printed-midi-controller
// https://learn.adafruit.com/trellis-3d-printed-enclosure


/** INIT */

void initTrellis() {

Adafruit_Trellis matrix0 = Adafruit_Trellis();
Adafruit_TrellisSet trellis =  Adafruit_TrellisSet(&matrix0);

}


/** FUNCTIONS */

void trellisDemo() {
  // light up all the LEDs in order
  for (uint8_t i = 0; i < numKeys; i++) {
    trellis.setLED(i);
    trellis.writeDisplay();
    delay(50);
  }
  // then turn them off
  for (uint8_t i = 0; i < numKeys; i++) {
    trellis.clrLED(i);
    trellis.writeDisplay();
    delay(50);
  }
}


#define MOMENTARY 0
#define LATCHING 1
// set the mode here
#define MODE LATCHING

void trellisReadButtons() {
  delay(30); // 30ms delay is required, dont remove me!

  if (MODE == MOMENTARY) {
    // If a button was just pressed or released...
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
	// if it was pressed, turn it on
	if (trellis.justPressed(i)) {
	  Serial.print("v"); Serial.println(i);
	  trellis.setLED(i);
	}
	// if it was released, turn it off
	if (trellis.justReleased(i)) {
	  Serial.print("^"); Serial.println(i);
	  trellis.clrLED(i);
	}
      }
      // tell the trellis to set the LEDs we requested
      trellis.writeDisplay();
    }
  }

  if (MODE == LATCHING) {
    // If a button was just pressed or released...
    if (trellis.readSwitches()) {
      // go through every button
      for (uint8_t i=0; i<numKeys; i++) {
        // if it was pressed...
	if (trellis.justPressed(i)) {
	  Serial.print("v"); Serial.println(i);
	  // Alternate the LED
	  if (trellis.isLED(i))
	    trellis.clrLED(i);
	  else
	    trellis.setLED(i);
        }
      }
      // tell the trellis to set the LEDs we requested
      trellis.writeDisplay();
    }
  }
}
