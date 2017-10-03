
int getButtonState() {

  static int muxChannel[16][4]={
    {0,0,0,0}, //channel 0
    {1,0,0,0}, //channel 1
    {0,1,0,0}, //channel 2
    {1,1,0,0}, //channel 3
    {0,0,1,0}, //channel 4
    {1,0,1,0}, //channel 5
    {0,1,1,0}, //channel 6
    {1,1,1,0}, //channel 7
    {0,0,0,1}, //channel 8
    {1,0,0,1}, //channel 9
    {0,1,0,1}, //channel 10
    {1,1,0,1}, //channel 11
    {0,0,1,1}, //channel 12
    {1,0,1,1}, //channel 13
    {0,1,1,1}, //channel 14
    {1,1,1,1}  //channel 15
  };

  // TODO: Debounce-Code here


  // shift-register pin offsets:
  // first 2 ICs are used to controll button LEDs,
  // 3rd IC is used to set MUX1 (buttons[16]) and MUX2 (potis[16])
  const int OFFSET_MUX1 = 16;
  const int OFFSET_MUX2 = 20;
  // iterate over the 16 channels of the mux
  for (int channel = 0; channel < 16; channel++) {
    // set shift-register
    for (int i=0; i < 4; i++) {
        register[OFFSET_MUX1 + i] = muxChannel[channel][i];
        register[OFFSET_MUX2 + i] = muxChannel[channel][i];
    }
    writeRegisters();
    potis[channel] = analogRead(PIN_MUX2_SIG);

    // TODO: Debounce-Code here
    buttons[channel] = analogRead(PIN_MUX1_SIG);


  } // end for channel
} // end getButtonStates()


void loop() {

    if (DEBUG) {

        //break;
    }

  // 1) Input: MIDI-In-Port abfragen
  //    a) clock
  //    b) note-on
  //    @see MIDI-Implementation-Chart
  // 2) Input: Sensoren abfragen
  //    a) MUX <- Buttons, Potis
  //    b) Encoder
  //    c) Trellis Button-Array (I2C)
  //    d) analogRead() <- Joystick
  // 3) Menue (contains Logic)
  //    a) ARPEGGIATOR Steuerung
  //    b) save state to EEPROM when a setting changes
  //    c) OUTPUT
  //       - LED of Buttons <- ShiftRegister
  //         just set the bits; shiftOut() is done in ReadMUX()
  //       - RGB-LED of Encoder
  //         shows the beats on PLAY (blue), REC (red), STEP: MIDI-IN (green), STOP (rainbow)
  //       - alphanum Display <- I2C
  //       - Trellis LED-Array <- I2C
  // 4) Sequencer
  //    a) clock and time
  //    b) mode
  //    c) MIDI-Out
  //    d) DAC <- I2C (CV) + Gate

    MIDI.read(); // TODO: handleClock()

    getButtonState();
    // Rotary Encoder
    // Trellis

    // TODO menu logic
    switch (mode):
    case PLAY:
        break;
    case REC:
        break;
    case STOP:
        break;
    default:
        break;

    switch (menu):
    case MAIN:
        break;
    case SET_CLOCK:
        break;
    case SET_SEQUENCE:
        break;
    case SET_TRANSPOSITION:
        break;
    // ...

    for (int i = 0; i < sizeof(buttons); i++) {
        if (i == 0 && buttons[i] == 0) {

        }
    }

    // alphanum Display
    // set Button LEDs
    // set RGB-LED of Rotary Encoder
    // set Trellis-LEDs


    // Sequencer
    if (mode == PLAY && clock >= nextStep && done != true) {
        nextStep += step_time;
        step_count++;

        // Mix in Variations
        // ... Transposition, 1.U, 2.U, INV, REV ...
        int note = sequence[step_count] + ... * ... ;
        // MIDI-OUT
        sendMidiOut(note);
        // CV/ Gate Out
        sendCvGate(note);

        done = true;
    }

    // Auduino
    // set Frequency from sequence[i]
    // set filter/ grain from Potis

}
