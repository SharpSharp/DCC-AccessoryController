/*  Basic sketch to domonstrate the use of the NmraDCC library
 *  as an Accessory contoller. Two DCC Accessory Addresses trigger
 *  stepper motors controlling fairgound rides.
 */
 
#include <NmraDcc.h>
NmraDcc  Dcc;
#include <AccelStepper.h>

// Define some steppers and the pins they will use
AccelStepper stepper1(AccelStepper::FULL4WIRE, 4, 5, 6, 7);
AccelStepper stepper2(AccelStepper::FULL4WIRE, 8, 9, 10, 11);

#define MGRPin 12
#define rocketsPin 13

// This function is called whenever a normal DCC Turnout Packet is received
// It returns the Acc Address (1 to 2044) 
// The Direction 0 (close) or 1 (throw)
// The Output Power true (not checking this)
void notifyDccAccTurnoutOutput(uint16_t Addr, uint8_t Direction, uint8_t OutputPower) {
  switch (Addr)
  {
    case 111:
      stepper1.move(5000);
      break;
    case 222:
      stepper2.move(6000);
      break;
    case 999:
      stepper1.stop();
      stepper2.stop();
      break;
  }
}

void setup()
{
  stepper1.setMaxSpeed(200.0);
  stepper1.setAcceleration(50.0);
  stepper2.setMaxSpeed(220.0);
  stepper2.setAcceleration(50.0);

  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up 
  Dcc.pin(0, 2, 1);
  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init(MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0);

  pinMode(MGRPin, INPUT_PULLUP); 
  pinMode(rocketsPin, INPUT_PULLUP);
}

void loop()
{
  if (digitalRead(MGRPin)     == LOW) stepper1.move(5000);
  if (digitalRead(rocketsPin) == LOW) stepper2.move(6000);

  Dcc.process();
  
  stepper1.run();
  stepper2.run();
}
