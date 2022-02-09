#include <Sensors/HallThrottle.h>

HallThrottle::HallThrottle(uint8_t pin, int16_t min, int16_t max)
 : pin_(pin), min_(min), max_(max)
{}

void HallThrottle::setup() {
  pinMode(pin_, INPUT);
}

float HallThrottle::readPercent() {
  float reading = (float)analogRead(pin_);
  float result = (reading - min_)/(max_ - min_);
  result = (result > 0.0f)?result:0.0f;
  result = (result < 1.0f)?result:1.0f;
  return result;
}
int16_t HallThrottle::readRaw() {
  return analogRead(pin_);
}