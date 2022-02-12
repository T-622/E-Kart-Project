#ifndef SYNTHESIZER_H
#define SYNTHESIZER_H

#include <Arduino.h>

class Synthesizer{                  // Class Synthsizer
    public:                         // Public elements (User accessible functions and variables for class Synthesizer) Accessible by each object
    void begin(int loadMode); 
    void updateRPM(uint16_t rpm);
    static void Emulator();         // Private static element for sound emulation
    private:
    float mapFloat(long x, long in_min, long in_max, long out_min, long out_max);
    uint8_t speed = 26;
    uint8_t idle4T = 26;
    uint8_t idle2T = 15;
    uint8_t idleElectric = 20;
    uint16_t COUNT_TOP = 0;
    uint16_t SYNC_OUT = 6;
    uint16_t PULSE_OUT = 7;
    uint16_t count = 0;
    uint16_t runMode = 0;
    uint16_t analog = 0;
    uint16_t realNoise4T[5825];     // DO NOT CREATE STRUCT. This will increase memory usage by allocating all memory at boot
    uint16_t realNoise2T[255];
    uint16_t realNoiseElectric[4032];
    float SCALE = 1.00;
    float OUT = 0.00;
    bool state = false;
    bool state2 = false;
    
};

extern Synthesizer Synth1;          // Init object of type Synth1 of class Synthesizer

#endif