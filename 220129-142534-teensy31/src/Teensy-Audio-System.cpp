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
uint16_t realNoiseElectric[2673];
bool state = false;

SdFs sd;
FsFile soundByte;
void setup();
void loop();
void load4T();
void load2T();
void loadElectric();
void Emulator();

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
    for(uint16_t i = 0; i < 2673; i++){
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
    if (COUNT_TOP == 4096){
      COUNT_TOP = 0;
    } else {
      analogWrite(A14, realNoise4T[COUNT_TOP]);
    }
    break;

    case 1:
    if (COUNT_TOP == 255){
      COUNT_TOP = 0;
    } else {
      analogWrite(A14, realNoise2T[COUNT_TOP]);
    }
    break;

    case 2:
    if (COUNT_TOP == 2673){
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

