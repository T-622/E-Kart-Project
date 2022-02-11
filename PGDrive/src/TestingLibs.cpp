#include <Audio/Synthesizer.h>
#include <FlexCan.h>

void setup() {
    Serial.begin(9600);
    Synth1.begin(1);
    Synth1.updateRPM(3400);
}

void loop() {


}