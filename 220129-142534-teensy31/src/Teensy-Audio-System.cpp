#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
//#include "Samples.h"
#define SD_CS 10
int ranDistort = 0;
int ranDelay = 0;
int analog = 0;
int count = 0;
float realNoise4T[4096];
float realNoise4T2[1729];
float realNoise2T[255];
float realNoiseElectric[2674];

SdFs sd;
FsFile soundByte;

void stroke4T(int rpm);
void stroke2T(int rpm);
void electricSound(int rpm);
void idleSound(int mode); 
void load4T();
void load2T();
void loadElectric();

void setup() {
  analogWriteResolution(12);                        // Analog R/W resolution change (Dumb down to ~10 bits for lower mem usage)
  analogReadResolution(12);
  Serial.begin(9600);
  Serial.println("Boot...");
  if (!sd.begin(SD_CS, SPI_HALF_SPEED))
  {
    Serial.println("ERROR: SD initialization failed!");
    while (true);
  }
  Serial.println("SD Initialization complete");
  load4T();
  Serial.println(realNoise4T[1]);
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
    analogWrite(A14, realNoiseElectric[i] + 100);
    delayMicroseconds(rpm);
  }
}

void idleSound(int mode) {                       // Usage of electric, 2T and 4T strokes to create "Idle" sound (Safety Purposes)
  switch (mode) {
    case 4:
      for (int i = 0; i < 4096; i++) {
        analogWrite(A14, realNoise4T[i]);
        delayMicroseconds(28);
      }
      for (int i = 0; i < 1729; i++) {
        analogWrite(A14, realNoise4T[i]);
        delayMicroseconds(28);
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


void load2T(){
    soundByte = sd.open("2T.txt", O_READ);
    for(int i = 0; i < 255; i++){
    realNoise2T[i] = soundByte.parseFloat();
    }
    Serial.print("Loaded ");
    Serial.print(sizeof(realNoise2T));
    Serial.print(" bytes");
}

void load4T(){
    soundByte = sd.open("4T.txt", O_READ);
    for(int i = 0; i < 4096; i++){
    realNoise4T[i] = soundByte.parseFloat();
    }
    soundByte = sd.open("4T2.txt", O_READ);
    for(int i = 0; i < 1729; i++){
    realNoise4T2[i] = soundByte.parseFloat();
    }
    Serial.print("Loaded ");
    Serial.print(sizeof(realNoise4T) + sizeof(realNoise4T2));
    Serial.print(" bytes");

}

  void loadElectric(){
    soundByte = sd.open("Electric.txt", O_READ);
    for(int i = 0; i < 2674; i++){
    realNoiseElectric[i] = soundByte.parseFloat();
    }
    Serial.print("Loaded ");
    Serial.print(sizeof(realNoiseElectric));
    Serial.print(" bytes");

  }

