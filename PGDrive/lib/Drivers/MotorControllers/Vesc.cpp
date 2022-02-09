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
  CAN_message_t msg = {};
  msg.id = ((((uint32_t)VescCommands::SET_CURRENT) << 8) | vesc_id_);
  msg.flags.extended = true;
  msg.len = 8;
  int32_t current_1000 = (int32_t)(current * 1000.0f);
  VescPacketSetCurrent* cmd_packet = (VescPacketSetCurrent*)msg.buf;
  cmd_packet->current = __builtin_bswap32((uint32_t)current_1000);
  cmd_packet->__unused__ = 0;
  if (!can_wrapper_.write(msg)) {
    Serial.println("Failed to send current command");
  }
}


void VescCAN::CANMsgCallback(const CAN_message_t &msg) {
  if ((msg.id & vesc_id_) != vesc_id_) {
    Serial.println("Got message efor wrong vesc! Check configuration");
    return;
  }
  switch ((msg.id & 0x0000FF00) >> 8) {
    case VescCommands::STATUS:
    {
      VescPacketStatus* status = (VescPacketStatus*)msg.buf;
      rpm_ = (int32_t)__builtin_bswap32(status->rpm);
      duty_cycle_ = ((int16_t)__builtin_bswap16(status->duty_cycle_1000)) / 1000.0f;
      current_ = ((int16_t)__builtin_bswap16(status->current_10)) / 10.0f;
      break;
    }
    default:
      // error
      break;
  }
}
