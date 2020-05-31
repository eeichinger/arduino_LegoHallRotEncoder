#ifndef LegoHallRotEncoder_h
#define LegoHallRotEncoder_h

#include <inttypes.h>

class LegoHallRotEncoder {

  public:
    static void rotenc_changed();

    LegoHallRotEncoder(uint8_t pin1, uint8_t pin2);
    int counter();
    int8_t dir();
    void resetCounter();
    void printDebug();
};

#endif
