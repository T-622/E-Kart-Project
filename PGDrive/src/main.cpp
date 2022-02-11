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
#include <MotorControllers/Vesc.h>
#include <Sensors/HallThrottle.h>
#include <Audio/Synthesizer.h>
int analog = 0;
// Simple VESC demo -- feel free to delete or comment out
VescCAN vesc(0x02);
HallThrottle throttle(PIN_A9, 265, 1023);

void setup() {
  Serial.begin(9600);
  Synth1.begin(0);

  delay(1000);
  // put your setup code here, to run once:

  // Simple VESC demo -- feel free to delete or comment out
  pinMode(LED_BUILTIN, OUTPUT);
  Can0_wrapper.begin(250000);
  vesc.begin();
}

void loop() {
  analog = analogRead(A9);
  analog = map(analog, 0, 4096, 1800, 6100);
  Synth1.updateRPM(analog);
  delay(250);

  // Simple VESC demo -- feel free to delete or comment out
  vesc.printData();
  Serial.print("Throttle: ");Serial.print(throttle.readPercent() * 100);Serial.println("%");
  vesc.commandCurrent(2.0*throttle.readPercent());

}