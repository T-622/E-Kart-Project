#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <Arduino.h>

class Synthesizer{
    public:
    void start(uint16_t loadMode);
    void updateRPM(uint16_t rpm);
    private:
    static void Emulator();
};

#endif