#include <Romi32U4.h>
#include "chassis.h"

int totalErrorCx = 0;
int totalErrorArea = 0;

void RomiChassis::UpdateEffortDriveWheelsPI(int current_cx, int current_area)
{
  // !!! ATTENTION !!!
  int diffcx = target_cx - current_cx; //difference between current speed and target speed
  int diffarea = target_area - current_area; //difference between current speed and target speed
  totalErrorCx = totalErrorCx + diffcx;
  totalErrorArea = totalErrorArea + diffarea;
  float uCx = KpCx * diffcx + KiCx * totalErrorCx; //total effort for the left
  float uArea = KpArea * diffarea + KiArea * totalErrorArea;
  float targetLeft = uArea - uCx;
  float targetRight = uArea + uCx;
  if(targetLeft > 300){
    targetLeft = 300;
  }
  if(targetRight > 300){
    targetRight = 300;
  }
  if(targetLeft < -300){
    targetLeft = -300;
  }
  if(targetRight < -300){
    targetRight = -300;
  }
  Serial.println(targetLeft);
  Serial.println(targetRight);
  Serial.println();
  motors.setEfforts(targetLeft,targetRight); //set motor efforts
}

void RomiChassis::Stop(void)
{
  current_area = current_cx = totalErrorCx = totalErrorArea = 0;
  motors.setEfforts(0, 0);
}