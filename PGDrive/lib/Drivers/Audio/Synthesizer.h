#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <Arduino.h>

class Synthesizer{
    public:
    void begin(uint16_t loadMode);
    void updateRPM(uint16_t rpm);
    uint16_t COUNT_TOP = 0;
    uint16_t SYNC_OUT = 6;
    uint16_t count = 0;
    uint16_t mode = 0;
    uint16_t analog = 0;
    uint16_t realNoise4T[5825]; // DO NOT CREATE STRUCT. This will increase memory usage by allocating all memory at boot
    uint16_t realNoise2T[255];
    uint16_t realNoiseElectric[4034];
    float SCALE = 1.00;
    float OUT = 0.00;
    bool state = false;
    private:
    static void Emulator();
};

extern Synthesizer Synth1;

#endif