#include <Romi32U4.h>
#include "Encoders.h"
#include "Wall_following_controller.h"
#include "IR_sensor.h"
#include "Sonar_sensor.h"

IRsensor SharpIR;
SonarSensor HCSR04;
void WallFollowingController::Init(void)
{
    SharpIR.Init();
    HCSR04.Init();
}

float WallFollowingController::Process(float target_distance)
{
  //assignment 2: write a PD controller that outputs speed as a function of distance error
  float Values[5], temp, medianReading; //Creation of Variables
  int j;
  float error = 0;
  
  /*for(int i = 0; i < 5; i++){ //Getting data array of distances
    Values[i] = SharpIR.ReadData();
  }

  for(int i = 1; i < 5; i++){ //sorting distances
    temp = Values[i];
    j = i - 1;
    while(temp < Values[j] && j >= 0){
      Values[j+1] = Values[j];
      j = j-1;
    }
    Values[j+1] = temp;
  }*/
  medianReading = SharpIR.ReadData();//Values[2]; //Finding median distance
  Serial.println(medianReading);
  E_distance = (medianReading - target_distance)*10; //Target speed will be Sensor - target distance because if we want the right
  float speed = E_distance * Kp + (E_distance-prev_e_distance) * Kd;
  prev_e_distance = error;
  return speed;                            // side to speed up (indicating too close), value must be negative
}