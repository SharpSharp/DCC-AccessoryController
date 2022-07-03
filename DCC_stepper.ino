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
  
void setup()
{
  stepper1.setMaxSpeed(200.0);
  stepper1.setAcceleration(50.0);
  stepper2.setMaxSpeed(220.0);
  stepper2.setAcceleration(50.0);

  pinMode(MGRPin, INPUT_PULLUP); 
  pinMode(rocketsPin, INPUT_PULLUP);
}

void loop()
{
  if (digitalRead(MGRPin)     == LOW) stepper1.move(5000);
  if (digitalRead(rocketsPin) == LOW) stepper2.move(6000);
  
  stepper1.run();
  stepper2.run();
}
