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
void notifyDccAccTurnoutOutput(uint16_t receivedAddress, uint8_t direction, uint8_t outputPower) {
  if (direction)
  {
    switch (receivedAddress)
    {
      case 1:  
        myLed.on();
        break;
      case 2:
        myLed.setMode(Blink);
        myLed.configBlink(10, 500);
        break;
      case 3:
        myLed.setMode(Random);
        myLed.configRandom(1000, 3000, true);
        break;
      case 4:
        myLed.setMode(Welding);
        myLed.configWelding(1000, 6, 20, 100, 11);
        break;
      case 33:
        myLed.setMode(Random);
        myLed.configRandom(5000, 10000, true);
    }
  }
  else
  {
    myLed.off();
  }
}

void setup() {
  //Serial.begin(9600); 
  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up 
  Dcc.pin(0, 2, 1);

  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init(MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0);

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
 
}

void loop() {
  Dcc.process();
  myLed.update(); 
}
