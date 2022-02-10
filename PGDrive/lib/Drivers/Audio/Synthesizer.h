#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <Arduino.h>

class Synthesizer{
    public:
    void begin(uint16_t loadMode);
    void updateRPM(uint16_t rpm);
    void Emulator();
};

#endif