/*
  hall rotary encoder demo

  you must connect rotary encoder to pins 2 & 3
*/

#include "LegoHallRotEncoder.h"

const int PIN_HALL1 = 2;
const int PIN_HALL2 = 3;

LegoHallRotEncoder rotEnc = LegoHallRotEncoder(PIN_HALL1, PIN_HALL2);

void setup() {
  Serial.begin(115200);
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
