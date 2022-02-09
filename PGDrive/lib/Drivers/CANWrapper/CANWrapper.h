#ifndef CANWRAPPER_H
#define CANWRAPPER_H

#include <FlexCAN.h>

#define MAX_CALLBACK_REGISTRATIONS (5)


class CANWrapperListener { // Not a fan of this, but easiest way to allow callbacks to a class function for now
public:
  virtual void CANMsgCallback(const CAN_message_t& msg) = 0;
};

class CANWrapper : protected CANListener {
public:
  CANWrapper(FlexCAN& canbus);
  void begin(uint32_t baud);
  void registerCallback(uint32_t filter, uint32_t mask, CANWrapperListener* listener);
  int write (const CAN_message_t &msg);

protected:
  virtual bool frameHandler (CAN_message_t &frame, int mailbox, uint8_t controller);
  virtual void txHandler (int mailbox, uint8_t controller);

private:
  struct CallbackRegistration {
    uint32_t filter;
    uint32_t mask;
    CANWrapperListener* listener;
  };

  FlexCAN& canbus_;
  CallbackRegistration callbacks_[MAX_CALLBACK_REGISTRATIONS];
  size_t num_callbacks_ = 0;
};

extern CANWrapper Can0_wrapper;
#ifdef __MK66FX1M0__
extern CanWrapper Can1_wrapper;
#endif


#endif // CANWRAPPER_H