#include <FlexCAN.h>

static CAN_message_t msg;
bool led = false;
void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  Can0.begin(500000);    // init CAN bus
  Serial.println("CAN Receiver Initialized");
}

void loop() {
  if (Can0.read(msg)) {
   led = !led;
   digitalWrite(13, led);
    Serial.print("Receiving: ");
    for (int i = 0; i < msg.len; i++) {
      Serial.print(msg.buf[i]); Serial.print(" ");
    }
    Serial.println("");
  }
}
