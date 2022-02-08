/* 
MIT License

Copyright (c) 2022 Tyler Peppy

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Arduino.h> 
#include <SPI.h>
#include <SdFat.h>
#define SD_CS 10
IntervalTimer timer4T;
uint16_t COUNT_TOP = 0;
uint16_t count = 0;
uint16_t mode = 0;
uint16_t analog = 0;
uint16_t SYNC_OUT = 6;
uint16_t realNoise4T[5825];
uint16_t realNoise2T[255];
uint16_t realNoiseElectric[4034];
float SCALE = 0.20;
float OUT = 0.00;
bool state = false;

SdFs sd;
FsFile soundByte;
void setup();
void loop();
void load4T();
void load2T();
void loadElectric();
void Emulator();
void updateRPM();

void setup() {
  mode = 0;

  pinMode(SYNC_OUT, OUTPUT);                        // Pin Declarations
  analogWriteResolution(12);                        // Analog R/W resolution change (Dumb down to ~10 bits for lower mem usage)
  analogReadResolution(12);

  Serial.begin(9600);                               // Debug Serial 9600 BPS
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
  analog = map(analog, 0, 4096, 35, 5);
  timer4T.update(analog);
}

void load2T(){
    soundByte = sd.open("2T.txt", O_READ);
    for(int i = 0; i < 255; i++){
    realNoise2T[i] = soundByte.parseInt();
    }
    Serial.print("Loaded ");
    Serial.print(sizeof(realNoise2T));
    Serial.print(" bytes");
}

void load4T(){
    soundByte = sd.open("4T.txt", O_READ);
    for(uint16_t i = 0; i < 5825; i++){
    realNoise4T[i] = soundByte.parseInt();
    }
    Serial.print("Loaded ");
    Serial.print(sizeof(realNoise4T));
    Serial.print(" bytes");

}

  void loadElectric(){
    soundByte = sd.open("Electric.txt", O_READ);
    for(uint16_t i = 0; i < 4034; i++){
    realNoiseElectric[i] = soundByte.parseInt();
    }
    Serial.print("Loaded ");
    Serial.print(sizeof(realNoiseElectric));
    Serial.print(" bytes");

  }

  void Emulator(){
  state = !state;
  digitalWrite(SYNC_OUT, state);
    switch(mode){
    case 0:
    if (COUNT_TOP == 5825){
      COUNT_TOP = 0;
      analogWrite(A14, realNoise4T[COUNT_TOP]);
    } else {
      OUT = (realNoise4T[COUNT_TOP] - 512) * SCALE + 512;   // Scale raw DAC values based on a scalar value (Volume)
      analogWrite(A14, OUT);
    }
    break;

    case 1:
    if (COUNT_TOP == 255){
      COUNT_TOP = 0;
      analogWrite(A14, realNoise2T[COUNT_TOP]);
    } else {
      analogWrite(A14, realNoise2T[COUNT_TOP]);
    }
    break;

    case 2:
    if (COUNT_TOP == 4034){
      analogWrite(A14, realNoiseElectric[0]);
      COUNT_TOP = 0;
    } else {
      analogWrite(A14, realNoiseElectric[COUNT_TOP]);
    }
    break;
    }
    state = !state;
    digitalWrite(SYNC_OUT, state);
    COUNT_TOP++;
  }

  void updateRPM(uint16_t rpm){
    rpm = map(rpm, 0, 14000, 35, 5);
    timer4T.update(analog);
  }

