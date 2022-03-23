#include <SmartPedal.h>

SmartPedal::MotorConfig toms_motor_conf {
  .rated_rpm = 10000,
  .rated_torque = 5.0,
  .torque_const = 1.0,
  .pole_count = 4
};
SmartPedal::MotorConfig golden_3kw_motor_conf {
  .rated_rpm = 3800,
  .rated_torque = 10.0,
  .torque_const = 0.183978693,
  .pole_count = 4
};


static inline float rpmToRadS(float rpm) {
  return rpm * (2 * M_PI / 60);
}

SmartPedal::SmartPedal(VescCAN& vesc)
 : vesc_(vesc)
{}

void SmartPedal::init(const SmartPedal::MotorConfig& motor_config, const SmartPedal::RaceConfig& race_config) {
  setMotorConfig(motor_config);
  setRaceConfig(race_config);
}

void SmartPedal::setMotorConfig(const MotorConfig& motor_config) {
  motor_config_ = motor_config;
  float max_current = motor_config_.rated_torque/motor_config.torque_const;
  vesc_.setCurrentLimit(max_current);
  Serial.print("Set max motor current to ");Serial.println(max_current);
  // TODO: set battery current limit
}

void SmartPedal::setRaceConfig(const RaceConfig& race_config) {
  race_config_ = race_config;
  max_torque_ = (race_config.power_rating + race_config.power_adjustment) / rpmToRadS(motor_config_.rated_rpm);
  // TODO: confirm within safe limits for motor
  max_current_ = max_torque_ / motor_config_.torque_const;
  Serial.print("Set max race current to ");Serial.println(max_current_);
}

void SmartPedal::setThrottle(float throttle) {
  // float torque_command = throttle * max_torque_;
  current_command_ = throttle * max_current_;
  Serial.print("Throttle cmd: ");Serial.print(throttle);Serial.print(". Sending current: ");Serial.println(current_command_);
  vesc_.commandCurrent(current_command_);
  throttle_ = throttle;
}

void SmartPedal::printInfo() {
  VescCAN::VescStats vesc_stats;
  vesc_.getStats(vesc_stats);

  char output[512];
  sprintf(output,
          "Time: %lu, maxpower: %f, max_rpm: %f, max_torque: %f, max_current: %f, "
          "throttle: %f, cmd: %f, current: %f, rpm: %f, "
          "vbat: %f, vmot: %f, cur_bat: %f, torque: %f, "
          "p_bat: %f, p_mot: %f, p_shaft:%f",
          millis(), race_config_.power_rating,motor_config_.rated_rpm, max_torque_,max_current_,
          throttle_, current_command_, vesc_stats.current, vesc_stats.erpm/motor_config_.pole_count,
          vesc_stats.input_voltage, vesc_stats.input_voltage * vesc_stats.duty, vesc_stats.input_current,vesc_stats.current * motor_config_.torque_const,
          vesc_stats.input_current * vesc_stats.input_voltage,
          vesc_stats.input_voltage * vesc_stats.duty * vesc_stats.current,
          vesc_stats.current * motor_config_.torque_const * rpmToRadS(vesc_stats.erpm/motor_config_.pole_count)
  );
  Serial.println(output);
}
void SmartPedal::printInfoCsvHeader() {
  Serial.println("Time,maxpower,max_rpm,max_torque,max_current,"
          "throttle,cmd,current,rpm,"
          "vbat,vmot,cur_bat,torque,"
          "p_bat,p_mot,p_shaft");
}
void SmartPedal::printInfoCsv() {
  VescCAN::VescStats vesc_stats;
  vesc_.getStats(vesc_stats);

  char output[512];
  sprintf(output,
          "%lu,%f,%f,%f,%f,"
          "%f,%f,%f,%f,"
          "%f,%f,%f,%f,"
          "%f,%f,%f",
          millis(), race_config_.power_rating,motor_config_.rated_rpm, max_torque_,max_current_,
          throttle_, current_command_, vesc_stats.current, vesc_stats.erpm/motor_config_.pole_count,
          vesc_stats.input_voltage, vesc_stats.input_voltage * vesc_stats.duty, vesc_stats.input_current,vesc_stats.current * motor_config_.torque_const,
          vesc_stats.input_current * vesc_stats.input_voltage,
          vesc_stats.input_voltage * vesc_stats.duty * vesc_stats.current,
          vesc_stats.current * motor_config_.torque_const * rpmToRadS(vesc_stats.erpm/motor_config_.pole_count)
  );
  Serial.println(output);
}