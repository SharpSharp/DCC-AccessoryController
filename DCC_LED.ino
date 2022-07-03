/*  Basic sketch to domonstrate the use of the NmraDCC library
 *  as an Accessory contoller. Several DCC Accessory Addresses trigger
 *  the built in LED to show different effects using the Led3 library 
 */
 
#include <NmraDcc.h>
NmraDcc  Dcc;
#include <Led3.h>
Led3 myLed = Led3(13);

// This function is called whenever a normal DCC Turnout Packet is received
// It returns the Acc Address (1 to 2044) 
// The Direction 0 (close) or 1 (throw)
// The Output Power true (not checking this)
void notifyDccAccTurnoutOutput(uint16_t Addr, uint8_t Direction, uint8_t OutputPower) {
  if (Addr == 1)
  {
    if (Direction)
    {
      myLed.on();
    }
    else
    {
      myLed.off();
    }
  }
}

void setup() { 
  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up 
  Dcc.pin(0, 2, 1);
  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init(MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0);

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  Dcc.process();
  myLed.update();
}
