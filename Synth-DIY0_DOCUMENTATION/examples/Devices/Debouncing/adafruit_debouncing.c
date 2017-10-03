// https://blog.adafruit.com/2009/10/20/example-code-for-multi-button-checker-with-debouncing/

#define DEBOUNCE 10  // button debouncer, how many ms to debounce, 5+ ms is usually plenty

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {14, 15, 16, 17, 18, 19}; // the analog 0-5 pins are also known as 14-19

// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)

// we will track if a button is just pressed, just released, or 'currently pressed' 
byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];

void setup() {
  byte i;
  
  // set up serial port
  Serial.begin(9600);
  Serial.print("Button checker with ");
  Serial.print(NUMBUTTONS, DEC);
  Serial.println(" buttons");

  // pin13 LED
  pinMode(13, OUTPUT);
 
  // Make input & enable pull-up resistors on switch pins
  for (i=0; i < NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH);
  }
}

void check_switches() {
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  static long lasttime;
  byte index;

  // TODO
  if (millis() > (lasttime + DEBOUNCE)){ // we wrapped around, lets just try again
     lasttime = millis();
  }
  
  if ((lasttime + DEBOUNCE) > millis()) {
    // not enough time has passed to debounce
    return; 
  }
  
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();
  
  // TODO
  for (index = 0; index < NUMBUTTONS; index++){ // when we start, we clear out the "just" indicators
    justreleased[index] = 0;
     
    currentstate[index] = digitalRead(buttons[index]);   // read the button
    
    /*     
    Serial.print(index, DEC);
    Serial.print(": cstate=");
    Serial.print(currentstate[index], DEC);
    Serial.print(", pstate=");
    Serial.print(previousstate[index], DEC);
    Serial.print(", press=");
    */
    
    if (currentstate[index] == previousstate[index]) {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
          // just pressed
          justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
          // just released
          justreleased[index] = 1;
      }
      pressed[index] = !currentstate[index];  // remember, digital HIGH means NOT pressed
    }
    //Serial.println(pressed[index], DEC);
    previousstate[index] = currentstate[index];   // keep a running tally of the buttons
  }
}


void loop() {
  check_switches();      // when we check the switches we'll get the current state
  
  for (byte i = 0; i < NUMBUTTONS; i++){
    if (justpressed[i]) {
      Serial.print(i, DEC);
      Serial.println(" Just pressed"); 
      // remember, check_switches() will CLEAR the 'just pressed' flag
    }
    if (justreleased[i]) {
      Serial.print(i, DEC);
      Serial.println(" Just released");
      // remember, check_switches() will CLEAR the 'just pressed' flag
    }
    if (pressed[i]) {
      Serial.print(i, DEC);
      Serial.println(" pressed");
      // is the button pressed down at this moment
    }
  }
}