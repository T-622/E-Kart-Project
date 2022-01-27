#include "Samples.h"
int ranDistort = 0;
int ranDelay = 0;
int analog = 0;


void setup() {
  analogWriteResolution(12);
}
void loop() {
  analog = analogRead(A9);
  analog = map(analog, 0, 1023, 0, 13000);
  stroke2T(analog);

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
  for (int i = 0; i < 4097; i++) {
    analogWrite(A14, realNoise2T[i]);
    delayMicroseconds(rpm);
  }
  for (int i = 0; i < 255; i++) {
    analogWrite(A14, noise4T[i]);
    delayMicroseconds(10);
  }

}
