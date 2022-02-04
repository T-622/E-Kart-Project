#include <Arduino.h>
#include <SPI.h>
#include <SdFat.h>
//#include "Samples.h"
IntervalTimer timer4T;
#define SD_CS 10
int ranDistort = 0;
int ranDelay = 0;
int COUNT_TOP = 0;
int count = 0;
int mode = 0;
int analog = 0;
int SYNC_OUT = 6;
int realNoise4T[5825];
float realNoise2T[255];
float realNoiseElectric[2674];
bool state = false;

SdFs sd;
FsFile soundByte;

void load4T();
void load2T();
void loadElectric();
void Emulator();

void setup() {
  mode = 0;
  pinMode(SYNC_OUT, OUTPUT);
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
  timer4T.begin(Emulator, 26);
}
void loop() {
   /*------Code Here (Nothing currently as timer is running system)------*/
  analog = analogRead(A9);
  analog = map(analog, 0, 4096, 35, 18);
  timer4T.update(analog);
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
    for(int i = 0; i < 5825; i++){
    realNoise4T[i] = soundByte.parseFloat();
    }
    Serial.print("Loaded ");
    Serial.print(sizeof(realNoise4T));
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

  void Emulator(){
  state = !state;
  digitalWrite(SYNC_OUT, state);
  COUNT_TOP++;
    switch(mode){
    case 0:
    if (COUNT_TOP == 4096){
      COUNT_TOP = 0;
    } else {
      analogWrite(A14, realNoise4T[COUNT_TOP] + ranDistort);
    }
    break;

    case 1:
    if (COUNT_TOP == 255){
      COUNT_TOP = 0;
    } else {
      analogWrite(A14, realNoise2T[COUNT_TOP] + ranDistort);
    }
    break;

    case 2:
    if (COUNT_TOP == 255){
      COUNT_TOP = 0;
    } else {
      analogWrite(A14, realNoiseElectric[COUNT_TOP] + 100);
    }
    break;
    }
    state = !state;
    digitalWrite(SYNC_OUT, state);
  }

