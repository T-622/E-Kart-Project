#include <CANWrapper/CANWrapper.h>

CANWrapper Can0_wrapper(Can0);
#ifdef __MK66FX1M0__
CANWrapper Can1_wrapper(Can1);
#endif

CANWrapper::CANWrapper(FlexCAN& canbus)
 : canbus_(canbus)
{}

void CANWrapper::begin(uint32_t baud) {
  canbus_.begin(baud);
  canbus_.attachObj(this);
  // TODO: can we limit filters better? mailboxes don't seem to work as described by documentation
  // so for now listen to everything and filter ourselves
  CAN_filter_t allPassFilter;
  allPassFilter.id = 0;
  allPassFilter.ext = 1; // Need to enable extended id's for VESC
  allPassFilter.rtr = 0;
  for (uint8_t filterNum = 0; filterNum < canbus_.getNumRxBoxes(); filterNum++) {
    canbus_.setFilter(allPassFilter, filterNum);
  }
  this->attachGeneralHandler();
}

void CANWrapper::registerCallback(uint32_t filter, uint32_t mask, CANWrapperListener* listener) {
  if (num_callbacks_ >= MAX_CALLBACK_REGISTRATIONS) {
    return;
  }
  callbacks_[num_callbacks_].filter   = filter;
  callbacks_[num_callbacks_].mask     = mask;
  callbacks_[num_callbacks_].listener = listener;
  num_callbacks_++;
}

bool CANWrapper::frameHandler(CAN_message_t &frame, int mailbox, uint8_t controller) {
  // Call any callbacks interested in this message;
  for (size_t i = 0; i < num_callbacks_; i++) {
    if ((frame.id & callbacks_[i].mask) == callbacks_[i].filter) {
      callbacks_[i].listener->CANMsgCallback(frame);
    }
  }
  return true;
}

void CANWrapper::txHandler(int mailbox, uint8_t controller) {
  // TODO: does this give any useful info?
}

int CANWrapper::write(const CAN_message_t &msg) {
  return canbus_.write(msg);
}
