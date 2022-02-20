#include <SPI.h>
#include <SdFat.h>
#include <Audio/Synthesizer.h>
#define SD_CS 10
IntervalTimer TMR0;
SdFs sd;
FsFile soundByte;

float mapfloat(long x, long in_min, long in_max, long out_min, long out_max)
{
  return (float)(x - in_min) * (out_max - out_min) / (float)(in_max - in_min) + out_min;
}

void Synthesizer::begin(uint8_t loadMode){ 
  Serial.println(loadMode);
  Synth1.runMode = loadMode;
  pinMode(Synth1.SYNC_OUT, OUTPUT);
  analogWriteResolution(12);                        // Analog R/W resolution change (Dumb down to ~10 bits for lower mem usage)
  analogReadResolution(12);
  Serial.println("Boot...");

  if (!sd.begin(SD_CS, SPI_HALF_SPEED)){             // Check for SD
   Serial.println("ERROR: SD initialization failed!");
   while (true);
  }
  Serial.println("SD Initialization complete");    
  switch(loadMode){   
   case 0:
    Serial.println("Loading 4S to MEM");
    soundByte = sd.open("4T.arb", O_READ);
    for(uint16_t i = 0; i < 5825; i++){
     Synth1.soundData[i] = soundByte.parseInt();
    }
    Serial.print("Loaded ");
    Serial.print(sizeof(Synth1.soundData));
    Serial.print(" bytes");
    TMR0.begin(Emulator, Synth1.speed); 
   break;

   case 1:
    Serial.println("Loading 2S to MEM");
    soundByte = sd.open("2T.arb", O_READ);
    for(uint8_t i = 0; i < 255; i++){
     Synth1.soundData[i] = soundByte.parseInt();
    }
    Serial.print("Loaded ");
    Serial.print(sizeof(Synth1.soundData));
    Serial.print(" bytes");
    TMR0.begin(Emulator, Synth1.speed);
   break;

   case 2:
    Serial.println("Loading Electric to MEM");
    soundByte = sd.open("Electric.arb", O_READ);
    for(uint16_t i = 0; i < 4032; i++){
     Synth1.soundData[i] = soundByte.parseInt();
    }
    Serial.print("Loaded ");
    Serial.print(sizeof(Synth1.soundData));
    Serial.print(" bytes");
    TMR0.begin(Emulator, Synth1.speed);
   break;

   default:
   break;
  };
}

void Synthesizer::updateRPM(uint16_t rpm){
  switch(Synth1.runMode){
   case 0:
    Synth1.speed = map(rpm, 0, 6100, 35, 10);
    TMR0.update(Synth1.speed);
   break;

   case 1:
    Synth1.speed = map(rpm, 0, 14000, 300, 85);
    TMR0.update(Synth1.speed);
   break;

   case 2:
    Synth1.speed = map(rpm, 0, 14000, 35, 5);
    TMR0.update(Synth1.speed);
   break;
  }
}

void Synthesizer::Emulator(){
Synth1.state = !Synth1.state;
digitalWrite(Synth1.SYNC_OUT, Synth1.state);
 switch(Synth1.runMode){
  case 0:
   if (Synth1.COUNT_TOP == 5825){
    Synth1.COUNT_TOP = 0;
    analogWrite(A14, Synth1.soundData[Synth1.COUNT_TOP]);
   } else {
    Synth1.SCALE = mapfloat(Synth1.speed, 35, 10, 0.5, 3);
    Synth1.OUT = (Synth1.soundData[Synth1.COUNT_TOP] - 512) * Synth1.SCALE + 512;   // Scale raw DAC values based on a scalar value (Volume)
    analogWrite(A14, Synth1.OUT);
   }
  break;

  case 1:
   if (Synth1.COUNT_TOP == 255){
    Synth1.COUNT_TOP = 0;
    analogWrite(A14, Synth1.soundData[Synth1.COUNT_TOP]);
   } else {
    Synth1.SCALE = mapfloat(Synth1.speed, 35, 5, 0.8, 5);
    Synth1.OUT = (Synth1.soundData[Synth1.COUNT_TOP] - 100) * Synth1.SCALE + 100;   // Scale raw DAC values based on a scalar value (Volume)
    analogWrite(A14, Synth1.OUT);
   }
  break;

  case 2:
   if (Synth1.COUNT_TOP == 4032){
    Synth1.COUNT_TOP = 0;
   } else {
    Synth1.SCALE = mapfloat(Synth1.speed, 35, 5, 0.8, 3);
    Synth1.OUT = (Synth1.soundData[Synth1.COUNT_TOP] - 250) * Synth1.SCALE + 250;   // Scale raw DAC values based on a scalar value (Volume)
    analogWrite(A14, Synth1.OUT);
   }  
  break;
}
Synth1.state = !Synth1.state;
digitalWrite(Synth1.SYNC_OUT, Synth1.state);
 Synth1.COUNT_TOP++;
}

uint32_t Synthesizer::freeMem(){ 
    uint32_t stackTop;
    uint32_t heapTop;

    // current position of the stack.
    stackTop = (uint32_t) &stackTop;

    // current position of heap.
    void* hTop = malloc(1);
    heapTop = (uint32_t) hTop;
    free(hTop);

    // The difference is (approximately) the free, available ram.
    return stackTop - heapTop;
}

void Synthesizer::restartModule(uint8_t rebootMode){
  TMR0.end();
  Serial.print("Restarting Audio Into Mode: ");
  Serial.println(rebootMode);
  for(int i = 0; i < 5284; i++){
    Synth1.soundData[i] = 0;
  }
  begin(rebootMode);
}

Synthesizer Synth1;


