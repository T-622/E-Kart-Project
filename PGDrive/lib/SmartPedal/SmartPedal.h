#ifndef SMART_PEDAL_H
#define SMART_PEDAL_H
#include <MotorControllers/Vesc.h>
/**
 * Load motor config:
 * -- top rpm
 * -- max torque
 * -- speed/torque const
 * -- TODO: motor controller
 * 
 * Load race config:
 * -- max power
 * -- (later) engine profile
 * -- weight adjustment (+- max power %)
 */






class SmartPedal {
public:
  struct MotorConfig {
    float rated_rpm;      // Rated rpm (not no-load rpm)
    float rated_torque;      // Rated torque (not stall rpm)
    float torque_const; // in Nm/A
    uint8_t pole_count; // Ratio between ERPM / RPM
    // TODO: motor controller
  };
  struct RaceConfig {
    float power_rating; // Max power for the race in Watts
    float power_adjustment;
    // TODO: engine profile
  };
  SmartPedal(VescCAN& vesc);

  void init(const MotorConfig& motor_config, const RaceConfig& race_config);

  void setMotorConfig(const MotorConfig& motor_config);

  void setRaceConfig(const RaceConfig& race_config);

  void setThrottle(float throttle);

private:
  // config
  VescCAN &vesc_;
  MotorConfig motor_config_;
  RaceConfig race_config_;

  // TODO: make adaptable to engine profile
  // TODO: pingpong until connected to vesc
  float max_torque_;
  float max_current_;
};

extern SmartPedal::MotorConfig toms_motor_conf;
extern SmartPedal::MotorConfig golden_3kw_motor_conf;

#endif // SMART_PEDAL_H