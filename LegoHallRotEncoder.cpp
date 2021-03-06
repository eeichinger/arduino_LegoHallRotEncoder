
#include "LegoHallRotEncoder.h"
#include <Arduino.h>

static const int8_t PROGMEM rot_enc_table[] = {
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, -1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, -1, 1, 0, 0, -1, 0, 0, 0, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
0, 0, -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 1, 0,
0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, -1, 1, 0,
0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};



static volatile uint8_t _prevNextCode = 0;
static volatile int _counter = 0;
static volatile int8_t _dir = 0;

int PIN_HALL1 = -1;
int PIN_HALL2 = -1;

/*
extern void init_encoder_table() {
  // dir +1
  rot_enc_table[B00011101] = 1;
  rot_enc_table[B01110111] = 1;
  rot_enc_table[B11011110] = 1;
  rot_enc_table[B01111011] = 1;
  rot_enc_table[B11101110] = 1;
  rot_enc_table[B10111000] = 1;
  rot_enc_table[B11100001] = 1;
  rot_enc_table[B10000111] = 1;
  // dir -1
  rot_enc_table[B10111011] = -1;
  rot_enc_table[B11101101] = -1;
  rot_enc_table[B10110111] = -1;
  rot_enc_table[B11011101] = -1;
  rot_enc_table[B01110100] = -1;
  rot_enc_table[B11010010] = -1;
  rot_enc_table[B01001011] = -1;
  rot_enc_table[B00101110] = -1;
}
*/

// A vald CW or CCW move returns 1, invalid returns 0.
static int8_t read_rotary() {
  // experimental sequence dir=1: 00 01 11 01 11 10 11 10
//  static int8_t rot_enc_table[255] = {    0    };

//  int prevdir = rot_enc_table[( _prevNextCode )];
  _prevNextCode <<= 2;
  if (digitalRead(PIN_HALL1)) _prevNextCode |= 0x02;
  if (digitalRead(PIN_HALL2)) _prevNextCode |= 0x01;
  _prevNextCode &= 0xff;

  return pgm_read_byte_near(rot_enc_table + _prevNextCode);
  // return ( rot_enc_table[( _prevNextCode )]);
}

/*static*/ void LegoHallRotEncoder::rotenc_changed() {
  _dir = read_rotary();
  _counter += _dir;
}

LegoHallRotEncoder::LegoHallRotEncoder(uint8_t pin1, uint8_t pin2) {
  // initialize digital pin LED_BUILTIN as an output.
  if (PIN_HALL1 != -1) {
    return;
  }
  PIN_HALL1 = pin1;
  PIN_HALL2 = pin2;
  pinMode(pin1, INPUT_PULLUP);
  pinMode(pin2, INPUT_PULLUP);
  if (pin1 >=2 && pin1<=3 && pin2>=2 && pin2<=3) { // use external interrupts
    attachInterrupt(digitalPinToInterrupt(pin1), LegoHallRotEncoder::rotenc_changed, CHANGE);
    attachInterrupt(digitalPinToInterrupt(pin2), LegoHallRotEncoder::rotenc_changed, CHANGE);
  }
}

void LegoHallRotEncoder::resetCounter() {
  _counter = 0;
}

int LegoHallRotEncoder::counter() {
  return _counter;
}

int8_t LegoHallRotEncoder::dir() {
  return _dir;
}

void LegoHallRotEncoder::printDebug() {
  Serial.print(_prevNextCode, BIN);
  Serial.print(" ");
  Serial.print(_dir);
  Serial.print(" ");
  Serial.print(_counter);
  Serial.println("");
}
