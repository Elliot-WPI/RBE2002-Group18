#include <Romi32U4.h>
#include "chassis.h"

int totalErrorLeft = 0;
int totalErrorRight = 0;

float RomiChassis::SpeedLeft(void)
{
    // !!! ATTENTION !!!
    //MotorControl(); //gets current and previous values of encoder
    int changeInLeft = count_left-prev_count_left; //determines the difference
    float velocity = (2*PI*R_wheel/N_wheel)*(changeInLeft)/interval*1000; //determines velocity
    return velocity; //[mm/s]
}

float RomiChassis::SpeedRight(void)
{
    //MotorControl(); //gets current and previous values of encoder
    int changeInRight = count_right-prev_count_right; //determines the difference
    int velocity = (2*PI*R_wheel/N_wheel)*(changeInRight)/interval*1000; //determines velocity
    return velocity; //[mm/s]
}

void RomiChassis::UpdateEffortDriveWheels(int left, int right)
{ 
    motors.setEfforts(left,right);
}

void RomiChassis::UpdateEffortDriveWheelsPI(int target_speed_left, int target_speed_right)
{
  // !!! ATTENTION !!!
  float currSpeedRight = SpeedRight(); //get current speed
  float currSpeedLeft = SpeedLeft(); //get current speed
  int diffLeft = target_speed_left - currSpeedLeft; //difference between current speed and target speed
  int diffRight = target_speed_right - currSpeedRight; //difference between current speed and target speed
  totalErrorLeft = totalErrorLeft + diffLeft; //total error
  totalErrorRight = totalErrorRight + diffRight; //total error
  float u_left = Kp * diffLeft + Ki * totalErrorLeft; //total effort for the left
  float u_right = Kp * diffRight + Ki * totalErrorRight; //total effort for the right
  motors.setEfforts(u_left,u_right); //set motor efforts
  SerialPlotter(diffLeft, diffRight, u_left, u_right, currSpeedLeft, currSpeedRight);
}

void RomiChassis::SerialPlotter(float a, float b, float c, float d, float f, float g)
{
    // !!! ATTENTION !!!
    // USE this function for assignment 3!
    Serial.print(a);
    Serial.print('\t');
    Serial.print(b);
    Serial.print('\t');
    Serial.print(c);
    Serial.print('\t');
    Serial.print(d);
    Serial.print('\t');
    Serial.print(f);
    Serial.print('\t');
    Serial.print(g);
    Serial.println();
    

}

void RomiChassis::MotorControl(void)
{
  //Serial.println("in motor control");
  uint32_t now = millis();
  if(now - last_update >= interval)
  {    
    prev_count_left = count_left;
    prev_count_right = count_right;
    count_left = encoders.getCountsLeft();
    count_right = encoders.getCountsRight();
    previous_time = millis();
    UpdateEffortDriveWheelsPI(target_left, target_right);
    last_update = now;
  }
}

void RomiChassis::StartDriving(float left, float right, uint32_t duration)
{
  target_left = left; target_right = right;
  start_time = millis();
  last_update = start_time;
  end_time = start_time + duration; //fails at rollover
  E_left = 0;
  E_right = 0;
}

bool RomiChassis::CheckDriveComplete(void)
{
  return millis() >= end_time;
}

void RomiChassis::Stop(void)
{
  target_left = target_right = 0;
  motors.setEfforts(0, 0);
}