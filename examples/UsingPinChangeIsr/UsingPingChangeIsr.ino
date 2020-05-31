/*
  hall rotary encoder demo using any pins and pin change interrupts

  you may connect rotary encoder to other pins, e.g. 4&5 and use pin change interrupts, see https://thewanderingengineer.com/2014/08/11/arduino-pin-change-interrupts/
*/

#include "LegoHallRotEncoder.h"

const int PIN_ROTENC1 = 4;
const int PIN_ROTENC2 = 5;

LegoHallRotEncoder rotEnc = LegoHallRotEncoder(PIN_ROTENC1, PIN_ROTENC2);

ISR(PCINT2_vect){
  static byte oldRotEncState = 0x0;

  // see https://arduino.stackexchange.com/a/12958 how to handle pin change interrupts
  byte newRotEncState = PIND & 0b00110000;  //(bit(4)|bit(5));
  if (newRotEncState != oldRotEncState) {
    LegoHallRotEncoder::rotenc_changed();
  }
  oldRotEncState = newRotEncState;
}


void setup() {
  cli(); // disable interrupts
  PCICR |= 0b00000100; // turn on pin change irq on port d
  PCMSK2 |= 0b00110000;    // turn on pins PD4 & PD5, PCINT20 & PCINT21 (=Arduino pins D4, D5)
  sei(); // reenable interrupts

  Serial.begin(115200);

  delay(5);
  Serial.println( (bit(4)|bit(5)), BIN);
}


void loop() {
  Serial.print("dir:");
  Serial.print(rotEnc.dir());
  Serial.print("counter:");
  Serial.print(rotEnc.counter());
  Serial.println();

  if (rotEnc.counter() > 50) rotEnc.resetCounter();

  delay(100);                       // wait a bit
}
