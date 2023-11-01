#include <robot.h>
#include <Chassis.h>
#include <HC-SR04.h>
#include <ir_codes.h>

HC_SR04 hc_sr04(0, 4);
void ISR_HC_SR04(void)
{
    hc_sr04.ISR_echo();
}

ROBOT_STATE robotState = ROBOT_IDLE;
Chassis chassis;

void initialize(void)
{
    chassis.init();

    hc_sr04.init(ISR_HC_SR04);
}

void idle(void)
{
    Serial.println("Idling!");
    chassis.setMotorEfforts(0, 0);
    robotState = ROBOT_IDLE;
}

String keyCodeString; // this may come in handy later
void handleKeyCode(int16_t keyCode)
{ 
    Serial.println(keyCode);

  if(keyCode == ENTER_SAVE) idle();

  switch(keyCode)
  {
    case UP_ARROW:
      chassis.setMotorTargetSpeeds(10, 10);
      robotState = ROBOT_DRIVING;
      break;
    case RIGHT_ARROW:
      chassis.setMotorTargetSpeeds(10, -10);
      robotState = ROBOT_RIGHT;
      break;
    case DOWN_ARROW:
      chassis.setMotorTargetSpeeds(-10, -10);
      robotState = ROBOT_DRIVING;
      break;
    case LEFT_ARROW:
      chassis.setMotorTargetSpeeds(-10, 10);
      robotState = ROBOT_LEFT;
      break;
  }
}

void processDistanceSensors(void)
{
    /** Check the distance sensor.
     * We return true only if there is a new reading, which is passed by reference.
     * It hardly needs to be done this way, but passing by reference is a useful tool,
     * so we start with a 'lightweight' example here.
     */
    float distanceReading = 0;
    bool hasNewReading = hc_sr04.getDistance(distanceReading);
    if(hasNewReading) handleNewDistanceReading(distanceReading);
}

void handleNewDistanceReading(float distanceReading)
{
#ifdef __DEBUG_RANGEFINDER__
    Serial.println(distanceReading);
#endif
    if(robotState == ROBOT_STANDOFF)
    {
      /**
       * TODO: Add bang-bang control with hysteresis
      */
    }
}
