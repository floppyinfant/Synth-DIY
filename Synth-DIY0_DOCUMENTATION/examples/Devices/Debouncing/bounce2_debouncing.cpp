// https://github.com/thomasfredericks/Bounce2

#include <Bounce2.h>

#define INTERVALL 5

int inputPin = 14;
pinMode(inputPin, INPUT);
digitalWrite(inputPin, HIGH);

// setup()
Bounce debouncer = Bounce();
debouncer.attach(inputPin);
debouncer.interval(INTERVAL);

// loop()
boolean changed = debouncer.update();
int state = debouncer.read();

if (state == LOW) {
    // button is pressed

} else {
    // button is not pressed (pullup)

}

if (changed) {

}

// Call code if Bounce fell (transition from HIGH to LOW)
if ( debouncer.fell() ) {

}
