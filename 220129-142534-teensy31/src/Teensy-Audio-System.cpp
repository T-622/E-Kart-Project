#include <Arduino.h>
#include "Samples.h"
int ranDistort = 0;
int ranDelay = 0;
int analog = 0;
int count = 0;

void stroke4T(int rpm);
void stroke2T(int rpm);
void electricSound(int rpm);
void idleSound(int mode); 

void setup() {
  analogWriteResolution(12);                        // Analog R/W resolution change (Dumb down to ~10 bits for lower mem usage)
  analogReadResolution(12);
}
void loop() {
  idleSound(4);                                     // Start with idle sound from 4T kart
}

void stroke4T(int rpm) {                            // Synthesize sound of a stroke (Fire) of a 4T pulse
  ranDistort = random(-150, 51);
  rpm = map(rpm, 0, 6100, 30, 3);
  for (int i = 0; i < 4095; i++) {
    analogWrite(A14, realNoise4T[i] + ranDistort);
    delayMicroseconds(rpm);
  }
  for (int i = 0; i < 1729; i++) {
    analogWrite(A14, realNoise4T2[i] + ranDistort);
    delayMicroseconds(rpm);
  }
}

void stroke2T(int rpm) {                            // Synthesize sound of a stroke (Fire) of a 2T pulse
  rpm = map(rpm, 0, 13000, 800, 250);
  for (int i = 0; i < 255; i++) {
    analogWrite(A14, realNoise2T[i]);
    delayMicroseconds(rpm);
  }
}

void electricSound(int rpm) {                       // Synthesize sound of revolution in an electric motor (Taycan Turbo as ref)
  rpm = map(rpm, 0, 7000, 45, 10);
  for (int i = 0; i < 2674; i++) {
    analogWrite(A14, electricSamples[i] + 100);
    delayMicroseconds(rpm);
  }
}

void idleSound(int mode) {                         // Usage of electric, 2T and 4T strokes to create "Idle" sound (Safety Purposes)
  switch (mode) {
    case 4:
      for (int i = 0; i < 4096; i++) {
        analogWrite(A14, realNoise4T[i]);
        delayMicroseconds(22.67);
      }
      for (int i = 0; i < 1729; i++) {
        analogWrite(A14, realNoise4T2[i]);
        delayMicroseconds(22.67);
      }
      break;
    case 2:
      for (int i = 0; i < 255; i++) {
        analogWrite(A14, realNoise2T[i]);
        delayMicroseconds(700);
      }
      break;

  }

}
