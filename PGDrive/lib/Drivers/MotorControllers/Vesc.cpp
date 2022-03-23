#include <MotorControllers/Vesc.h>


VescCAN::VescCAN(uint8_t vesc_id, CANWrapper& can_wrapper) 
  : can_wrapper_(can_wrapper), vesc_id_(vesc_id)
{}

void VescCAN::begin() {
  can_wrapper_.registerCallback((uint32_t)vesc_id_, 0xFFFF00FF, this);
}

void VescCAN::printData() {
  Serial.print("Vesc Data: ");
  Serial.print(rpm_);
  Serial.print(" rpm, ");
  Serial.print(current_);
  Serial.print(" A, ");
  Serial.print(duty_cycle_ * 100.0f);
  Serial.println("%");
}

void VescCAN::commandCurrent(float current) {
  VescPacketData data= {};
  int32_t current_1000 = (int32_t)(current * 1000.0f);
  data.set_current.current = current_1000;
  if (!writeCANCmd(VescCommands::SET_CURRENT, data)) {
    Serial.println("Failed to send current command");
  }
}

void VescCAN::setCurrentLimit(float current_limit) {
  VescPacketData data= {};
  int32_t current_limit_1000 = (int32_t)(current_limit * 1000.0f);
  data.conf_current_limits.max_current_1000 = current_limit_1000;
  data.conf_current_limits.min_current_1000 = -current_limit_1000;
  if (!writeCANCmd(VescCommands::CONF_CURRENT_LIMITS, data)) {
    Serial.println("Failed to send current limit command");
  }
}

void VescCAN::CANMsgCallback(const CAN_message_t &msg) {
  if ((msg.id & vesc_id_) != vesc_id_) {
    Serial.println("Got message efor wrong vesc! Check configuration");
    return;
  }
  VescPacketData data;
  memcpy(data.buf, msg.buf, 8); // Could just cast the pointer, but compiler doesn't like that :P
  data.data = __builtin_bswap64(data.data);
  switch ((msg.id & 0x0000FF00) >> 8) {
    case VescCommands::STATUS:
    {
      rpm_ = (0.9f*rpm_) + 0.1f* data.status.rpm;
      duty_cycle_ = (0.9f*duty_cycle_) + 0.1f* data.status.duty_cycle_1000 / 1000.0f;
      current_ = (0.9f*current_) + 0.1f*data.status.current_10 / 10.0f;
      break;
    }
    case VescCommands::STATUS_4:
    {
      input_current_ = (0.9f* input_current_) + 0.1f*data.status4.input_current_10 / 10.0f;
      break;
    }
    case VescCommands::STATUS_5:
    {
      // Serial.print("old avg: ");Serial.print(input_voltage_);Serial.print(" 0.9o");Serial.print(0.9f*input_voltage_);Serial.print(" in ");Serial.print(data.status5.input_voltage_10);Serial.print(" t2 ");Serial.println((0.1f* data.status5.input_voltage_10 / 10.0f));
      input_voltage_ =(0.9f*input_voltage_) + (0.1f* data.status5.input_voltage_10 / 10.0f);
      break;
    }
    default:
      // error
      break;
  }
}

bool VescCAN::writeCANCmd(VescCommands cmd, const VescPacketData& data) {
  VescPacketData reversed_data {
    .data = __builtin_bswap64(data.data)
  };
  CAN_message_t msg = {};
  msg.id = ((((uint32_t)cmd) << 8) | vesc_id_);
  msg.flags.extended = true;
  msg.len = 8;
  memcpy(msg.buf, reversed_data.buf, 8);
  return can_wrapper_.write(msg);
}

void VescCAN::getStats(VescStats& stats) {
  stats.current = current_;
  stats.duty = duty_cycle_;
  stats.erpm = rpm_;
  stats.input_current = input_current_;
  stats.input_voltage = input_voltage_;
}

