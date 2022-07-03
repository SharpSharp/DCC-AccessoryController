#include <NmraDcc.h>
NmraDcc  Dcc;

int accessoryAddress = 1;

// This function is called whenever a normal DCC Turnout Packet is received
// It returns the Acc Address (1 to 2044) 
// The Direction 0 (close) or 1 (throw)
// The Output Power true  (not checking this)
void notifyDccAccTurnoutOutput(uint16_t receivedAddress, uint8_t direction, uint8_t outputPower) {
  if (receivedAddress == accessoryAddress)
  {
    if (direction)
    {
      digitalWrite(LED_BUILTIN, HIGH);
    }
    else
    {
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

void setup() { 
  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up 
  Dcc.pin(0, 2, 1);
  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init(MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  Dcc.process();
}
