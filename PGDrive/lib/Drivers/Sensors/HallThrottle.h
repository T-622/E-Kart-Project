#ifndef HALL_THROTTLE_H
#define HALL_THROTTLE_H

#include <Arduino.h>

class HallThrottle {
public:
  /**
   * @brief Construct a new Hall Throttle object
   * 
   * @param pin Analog pin for the throttle input
   * @param min analog reading for unpressed throttle     TODO: make this a voltage?
   * @param max analog reading for fully pressed throttle TODO: make this a voltage?
   */
  HallThrottle(uint8_t pin, int16_t min=0, int16_t max=1024);
  void setup();
  float readPercent();
  int16_t readRaw();
  // TODO: functionality for calibrating max/min and loading calibration from SD card
private:
  // TODO: would automatic readings with timers be beneficial?

  uint8_t pin_;
  int16_t min_;
  int16_t max_;
};

#endif // HALL_THROTTLE_H