/** SHIFT-REGISTER: LED-ARRAY */
// 74HC595 serial to parallel shift registers
// http://bildr.org/2011/02/74hc595/

//How many of the shift registers - change this
#define number_of_74hc595s 2

//do not touch
#define NUM_REGISTER_PINS number_of_74hc595s * 8

boolean registers[NUM_REGISTER_PINS];

void initShiftRegister() {
  pinMode(PIN_SHIFT_SER, OUTPUT);
  pinMode(PIN_SHIFT_SCLK, OUTPUT);
  pinMode(PIN_SHIFT_RCLK, OUTPUT);

  //reset all register pins
  clearRegisters();
  writeRegisters();
}

//set all register pins to LOW
void clearRegisters(){
  for(int i = NUM_REGISTER_PINS - 1; i >=  0; i--){
     registers[i] = LOW;
  }
}


//Set and display registers
//Only call AFTER all values are set how you would like (slow otherwise)
void writeRegisters(){

  digitalWrite(PIN_SHIFT_RCLK, LOW);

  for(int i = NUM_REGISTER_PINS - 1; i >=  0; i--){
    digitalWrite(PIN_SHIFT_SCLK, LOW);

    int val = registers[i];

    digitalWrite(PIN_SHIFT_SER, val);
    digitalWrite(PIN_SHIFT_SCLK, HIGH);

  }
  digitalWrite(PIN_SHIFT_RCLK, HIGH);

}

//set an individual pin HIGH or LOW
void setRegisterPin(int index, int value){
  registers[index] = value;
}


void loop_shift(){

  setRegisterPin(2, HIGH);
  setRegisterPin(3, HIGH);
  setRegisterPin(4, LOW);
  setRegisterPin(5, HIGH);
  setRegisterPin(7, HIGH);


  writeRegisters();  //MUST BE CALLED TO DISPLAY CHANGES
  //Only call once after the values are set how you need.
}

// ---------------------

// http://arduino.cc/en/Tutorial/ShiftOut
// setup code for controlling the LEDs via 74HC595 serial to parallel shift registers
/*
void initShiftRegister() {
    byte leds[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
}
*/
// end ShiftRegister (74HC595)
