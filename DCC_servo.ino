#include <Servo.h>
#include <NmraDcc.h>

// constants in caps

const byte NUMBER_OF_PUSH_BUTTONS = 4;
const byte NUMBER_OF_ROUTE_BUTTONS = 2;
const byte NUMBER_OF_TURNOUTS = 5;
#define TURNOUT_MOVE_SPEED 30              // [ms] lower number is faster

class ServoTurnout
{
private:
  int dccAddress;
  int pin;
  int closedPosition;
  int thrownPosition;
  int linkedTurnout;
  Servo servo;
  int targetPosition;
  int currentPosition;
  unsigned long moveDelay;
  bool isClosed;

public:
  ServoTurnout(int dccAddress, int pin, int closedPosition, int thrownPosition, int linkedTurnout = -1)
    : dccAddress(dccAddress), pin(pin), closedPosition(closedPosition), thrownPosition(thrownPosition), linkedTurnout(linkedTurnout)
  {
  }
  
  void setup()
  {
    targetPosition = closedPosition;       // setup targets as closed positiion
    currentPosition = closedPosition;      // setup position to closed
    isClosed = true;                       // setup closed is true

    servo.attach(pin);                     // attach all the turnout servos
    servo.write(currentPosition);          // write the servos poistions
  }

  int getAddress()
  {
    return dccAddress;
  }

  void setTargetDirection(uint8_t direction)
  {
    if (direction)  setClosedTarget();
    else            setThrownTarget();
  }

  void moveServo();

  void setClosedTarget()
  {
    targetPosition = closedPosition;
  }

  void setThrownTarget()
  {
    targetPosition = thrownPosition;
  }

  void toggleTarget()
  {
    if (isClosed) setThrownTarget(); // set turnout target to thrown if it is closed
    else          setClosedTarget(); // set turnout target to closed if it is thrown
  }
};

// define the turnouts with their pins and end position angles.
ServoTurnout turnouts[NUMBER_OF_TURNOUTS] = {
  {40, 3, 50, 90, 2},
  {41, 4, 80, 150},
  {42, 5, 60, 120},
  {43, 6, 60, 120, 4},  // Turnout 3 is linked with turnout 4.
  {44, 7, 60, 120}
};

void ServoTurnout::moveServo()
{
  if (currentPosition != targetPosition) { // check that they are not at there taregt position
    if (millis() > moveDelay) {            // check that enough delay has passed
      moveDelay = millis() + TURNOUT_MOVE_SPEED;
      if (currentPosition < targetPosition) currentPosition++;
      if (currentPosition > targetPosition) currentPosition--;
      servo.write(currentPosition);
      if (currentPosition == closedPosition) {
        isClosed = true;
      }
      if (currentPosition == thrownPosition) {
        isClosed = false;
      }

      if (linkedTurnout > 0) {
        if (currentPosition == closedPosition) {
          turnouts[linkedTurnout].setClosedTarget();
        }
        if (currentPosition == thrownPosition) {
          turnouts[linkedTurnout].setThrownTarget();
        }
      }
    }
  }
}

NmraDcc  Dcc;

// This function is called whenever a normal DCC Turnout Packet is received
// It returns the Acc Address (1 to 2044) 
// The Direction 0 (close) or 1 (throw)
// The Output Power true  (make sure it is active)
void notifyDccAccTurnoutOutput(uint16_t receivedAddress, uint8_t direction, uint8_t outputPower)
{
 for (int i = 0; i < NUMBER_OF_TURNOUTS; i++)
 {
   if (receivedAddress == turnouts[i].getAddress())
   {
     turnouts[i].setTargetDirection(direction);
     break;
   }
  }
}


/* Setup servo arrays  */
int pushButtonPins[NUMBER_OF_PUSH_BUTTONS] = {7, 8, 9, 10}; // enter pin numbers. 1st number is for pushButtonPins[0], 2nd for pushButtonPins[1] etc
int routeButtonPins[NUMBER_OF_ROUTE_BUTTONS] = {11, 12};    // enter pin numbers. 1st number is for routeButtonPins[0], 2nd for routeButtonPins[1] etc

void setupDCCDecoder()
{
  // Setup which External Interrupt, the Pin it's associated with that we're using and enable the Pull-Up 
  Dcc.pin(0, 2, 1);

  // Call the main DCC Init function to enable the DCC Receiver
  Dcc.init(MAN_ID_DIY, 10, CV29_ACCESSORY_DECODER | CV29_OUTPUT_ADDRESS_MODE, 0);
}


void setup() {
  Serial.begin(9600);
  /* setup all the servos*/
  for (int thisTurnout = 0; thisTurnout < NUMBER_OF_TURNOUTS; thisTurnout++) {
      turnouts[thisTurnout].setup();
  }
  
  /* setup all the pushbuttons */
  for (int i = 0; i < NUMBER_OF_PUSH_BUTTONS;  i++) pinMode(pushButtonPins[i], INPUT_PULLUP);  // initailise all the push buttons with pullups 
  for (int i = 0; i < NUMBER_OF_ROUTE_BUTTONS; i++) pinMode(routeButtonPins[i], INPUT_PULLUP); // initailise all the route buttons with pullups 
  
  setupDCCDecoder();
}

void loop() {
  Dcc.process();
  /* loop through all the push buttons */
  for (int i = 0; i < NUMBER_OF_PUSH_BUTTONS; i++){
    if (digitalRead(pushButtonPins[i]) == LOW){ // is push button pressed
      turnouts[i].toggleTarget();
    }
  }
  
  /* read route buttons and move turnouts when pressed */
  if (digitalRead(routeButtonPins[0]) == LOW){ // is route button pressed
    turnouts[0].setClosedTarget(); // set turnout target to closed if it is thrown
    turnouts[1].setClosedTarget(); // set turnout target to closed if it is thrown
  }
  if (digitalRead(routeButtonPins[1]) == LOW){ // is route button pressed
    turnouts[0].setThrownTarget();  // set turnout target to thrown if it is thrown
    turnouts[1].setThrownTarget();  // set turnout target to thrown if it is thrown
  }
  
  /* loop through the turnout servos */
  for (int thisTurnout = 0; thisTurnout < NUMBER_OF_TURNOUTS; thisTurnout++) {
    turnouts[thisTurnout].moveServo();                          // move the servo 1 degree towards its target
  }
}
