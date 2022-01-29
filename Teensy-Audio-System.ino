#include "Samples.h"
int ranDistort = 0;
int ranDelay = 0;
int analog = 0;
int count = 0;


void setup() {
  analogWriteResolution(12);
  Serial.begin(9600);
}
void loop() {
  idleSound(4);
}

void stroke4T(int rpm) {
  ranDistort = random(-150, 51);
  rpm = map(rpm, 0, 6100, 30, 3);
  for (int i = 0; i < 4097; i++) {
    analogWrite(A14, realNoise4T[i] + ranDistort);
    delayMicroseconds(rpm);
  }
  for (int i = 0; i < 1730; i++) {
    analogWrite(A14, realNoise4T2[i] + ranDistort);
    delayMicroseconds(rpm);
  }
}

void stroke2T(int rpm) {
  ranDistort = random(-150, 51);
  rpm = map(rpm, 0, 13000, 25, 5);
  for (int i = 0; i < 2327; i++) {
    analogWrite(A14, realNoise2T[i]);
    delayMicroseconds(rpm);
  }
  for (int i = 0; i < 255; i++) {
    analogWrite(A14, noise4T[i]);
    delayMicroseconds(10);
  }
}

void electricSound(int rpm) {
  rpm = map(rpm, 0, 7000, 45, 10);
  for (int i = 0; i < 2675; i++) {
    analogWrite(A14, electricSamples[i] + 100);
    delayMicroseconds(rpm);
  }
}

void idleSound(int mode) {
  switch (mode) {
    case 4:
      for (int i = 0; i < 4097; i++) {
        analogWrite(A14, realNoise4T[i]);
        delayMicroseconds(22.67);
      }
      for (int i = 0; i < 1730; i++) {
        analogWrite(A14, realNoise4T2[i]);
        delayMicroseconds(22.67);
      }
      break;
    case 2:
      for (int i = 0; i < 4097; i++) {
        analogWrite(A14, realNoise2T[i]);
        delayMicroseconds(15);
      }
      for (int i = 0; i < 255; i++) {
        analogWrite(A14, noise4T[i]);
        delayMicroseconds(5);
      }
      break;

  }

}
