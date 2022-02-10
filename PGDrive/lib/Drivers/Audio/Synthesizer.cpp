#include <SPI.h>
#include <SdFat.h>
#include <Audio/Synthesizer.h>
#define SD_CS 10
IntervalTimer TMR0;
SdFs sd;
FsFile soundByte;
uint16_t COUNT_TOP = 0;
uint16_t count = 0;
uint16_t mode = 0;
uint16_t analog = 0;
uint16_t SYNC_OUT = 6;
uint16_t realNoise4T[5825]; // DO NOT CREATE STRUCT. This will increase memory usage by allocating all memory at boot
uint16_t realNoise2T[255];
uint16_t realNoiseElectric[4034];
float SCALE = 1.00;
float OUT = 0.00;
bool state = false;

void Synthesizer::begin(uint16_t loadMode){ 
    Serial.begin(9600);
    loadMode = mode;
    pinMode(SYNC_OUT, OUTPUT);
    analogWriteResolution(12);                        // Analog R/W resolution change (Dumb down to ~10 bits for lower mem usage)
    analogReadResolution(12);

    if (!sd.begin(SD_CS, SPI_HALF_SPEED))             // Check for SD
    {
    Serial.println("ERROR: SD initialization failed!");
    while (true);
    }
    Serial.println("SD Initialization complete");    

    switch(loadMode){
        case 0:
         Serial.println("Loading 2S to MEM");
         soundByte = sd.open("2T.txt", O_READ);
         for(int i = 0; i < 255; i++){
         realNoise2T[i] = soundByte.parseInt();
         }
         Serial.print("Loaded ");
         Serial.print(sizeof(realNoise2T));
         Serial.print(" bytes");
         TMR0.begin(Emulator, 26);
        break;

        case 1:
         Serial.println("Loading 4S to MEM");
         soundByte = sd.open("4T.txt", O_READ);
         for(uint16_t i = 0; i < 5825; i++){
         realNoise4T[i] = soundByte.parseInt();
         }
         Serial.print("Loaded ");
         Serial.print(sizeof(realNoise4T));
         Serial.print(" bytes");
         TMR0.begin(Emulator, 26);
        break;

        case 2:
         Serial.println("Loading Electric to MEM");
         soundByte = sd.open("Electric.txt", O_READ);
         for(uint16_t i = 0; i < 4034; i++){
         realNoiseElectric[i] = soundByte.parseInt();
         }
         Serial.print("Loaded ");
         Serial.print(sizeof(realNoiseElectric));
         Serial.print(" bytes");
         TMR0.begin(Emulator, 26);
        break;
    };
    
}

void Synthesizer::updateRPM(uint16_t rpm){
    rpm = map(rpm, 0, 14000, 35, 5);
    TMR0.update(analog);
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
      OUT = (realNoise2T[COUNT_TOP] - 512) * SCALE + 512;   // Scale raw DAC values based on a scalar value (Volume)
      analogWrite(A14, realNoise2T[COUNT_TOP]);
    }
    break;

    case 2:
    if (COUNT_TOP == 4034){
      analogWrite(A14, realNoiseElectric[0]);
      COUNT_TOP = 0;
    } else {
      OUT = (realNoiseElectric[COUNT_TOP] - 512) * SCALE + 512;   // Scale raw DAC values based on a scalar value (Volume)
      analogWrite(A14, realNoiseElectric[COUNT_TOP]);
    }
    break;
    }
    state = !state;
    digitalWrite(SYNC_OUT, state);
    COUNT_TOP++;
  }


