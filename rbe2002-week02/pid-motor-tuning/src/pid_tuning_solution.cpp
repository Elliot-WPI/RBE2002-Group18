/**
 * This code adds the ability to tune the gains and change the targets
 * */

#include <Chassis.h>
#include <Romi32U4Buttons.h>

#include "params.h"
#include "serial_comm.h"

Romi32U4ButtonA buttonA;
Romi32U4ButtonB buttonB;
Romi32U4ButtonC buttonC;

/**
 * You **must** name your Chassis object chassis!
*/
Chassis chassis;

void setup()
{
  Serial.begin(115200);
  //while(!Serial) {}  //IF YOU UNCOMMENT THIS LINE, THEN YOU _MUST_ OPEN THE SERIAL MONITOR TO GET PAST IT!
  Serial.println("setup()");

  chassis.init();

  Serial.println("/setup()");
}

void loop() 
{    
  chassis.loop();

  if(buttonA.getSingleDebouncedPress())
  {
    targetLeft = targetLeft < 40 ? 50 : 10;
  }

  if(buttonB.getSingleDebouncedPress())
  {
    targetLeft += 5;
  }

  if(buttonC.getSingleDebouncedPress())
  {
    targetLeft -= 5;
  }

  chassis.setMotorTargetSpeeds(targetLeft, 0); //add right motor when ready

  /* for reading in gain settings
  * CheckSerialInput() returns true when it gets a complete string, which is
  * denoted by a newline character ('\n'). Be sure to set your Serial Monitor to 
  * append a newline
  */
  if(CheckSerialInput()) 
  {
    ParseSerialInput();
    leftMotor.setPIDCoeffs(Kp, Ki, Kd); //just the left motor for the demo
  }
}

