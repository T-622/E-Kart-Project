#ifndef VESC_H
#define VESC_H

#include <CANWrapper/CANWrapper.h>

// TODO: I think we should make a dedicated CANListener that classes like VESC register filters+masks with, since mailboxes dont seem to work as described
class VescCAN : protected CANWrapperListener {
public:
  VescCAN(uint8_t vesc_id, CANWrapper& can_wrapper=Can0_wrapper);
  void begin();
  void printData();

  void commandCurrent(float current);
  void setCurrentLimit(float current_limit);

protected:
  virtual void CANMsgCallback(const CAN_message_t& msg);

private:
  // Vesc CAN messageIds: https://vesc-project.com/sites/default/files/imce/u15301/VESC6_CAN_CommandsTelemetry.pdf
  enum VescCommands : uint8_t {
    SET_DUTY = 0,
    SET_CURRENT = 1,
    SET_CURRENT_BRAKE = 2,
    SET_RPM = 3,
    SET_POS = 4,
    FILL_RX_BUFFER = 5,
    FILL_RX_BUFFER_LONG = 6,
    PROCESS_RX_BUFFER = 7,
    PROCESS_SHORT_BUFFER = 8,
    STATUS = 9,
    SET_CURRENT_REL = 10,
    SET_CURRENT_BRAKE_REL = 11,
    SET_CURRENT_HANDBRAKE = 12,
    SET_CURRENT_HANDBRAKE_REL = 13,
    STATUS_2 = 14,
    STATUS_3 = 15,
    STATUS_4 = 16,
    PING = 17,
    PONG = 18,
    DETECT_APPLY_ALL_FOC = 19,
    DETECT_APPLY_ALL_FOC_RES = 20,
    CONF_CURRENT_LIMITS = 21,
    CONF_STORE_CURRENT_LIMITS = 22,
    CONF_CURRENT_LIMITS_IN = 23,
    CONF_STORE_CURRENT_LIMITS_IN = 24,
    CONF_FOC_ERPMS = 25,
    CONF_STORE_FOC_ERPMS = 26,
    STATUS_5 = 27
  };

  // TODO: all data entries from VESC have reversed endianness.
  // Might be cleanest to reverse order of structs and bswap the whole buffer
  // Instead of remembering to bswap every element we read/write
  struct __attribute__ ((packed)) VescPacketStatus {
    int16_t duty_cycle_1000;
    int16_t current_10;
    int32_t rpm;
  };
  struct __attribute__ ((packed)) VescPacketSetCurrent {
    int32_t __unused__;
    int32_t current;
  };
  struct __attribute__ ((packed)) VescPacketConfCurrentLimits {
    int32_t max_current_1000;
    int32_t min_current_1000;
  };
  union VescPacketData
  {
    /* data */
    uint8_t buf[8];
    uint64_t data;
    VescPacketStatus            status;
    VescPacketSetCurrent        set_current;
    VescPacketConfCurrentLimits conf_current_limits;
  };

  bool writeCANCmd(VescCommands cmd, const VescPacketData& data);
  

  CANWrapper& can_wrapper_;
  uint8_t vesc_id_;
  
  float current_ = 0;
  int32_t rpm_ = 0;
  float duty_cycle_ = 0;

};

#endif //VESC_H