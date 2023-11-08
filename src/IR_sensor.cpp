#include <Romi32U4.h>
#include "IR_sensor.h"

void IRsensor::Init(void)
{
    pinMode(pin_IR, INPUT);
}

float IRsensor::PrintData(void)
{
    Serial.println(ReadData());
}

float IRsensor::ReadData(void)
{
  //assignment 1.1
  //read out and calibrate your IR sensor, to convert readouts to distance in [cm]
  float m = 3226; //slope
  float b = 111; //b-intercept
  int sensorAngle = 50; //angle
  int sensorFromEdge = 2; //dist from edge
  float distSensor = m/(analogRead(A0)-b); //change ADC to cm
  float distRomi = distSensor * cos(sensorAngle*PI/180) - sensorFromEdge; //calculate triangle to get side of robot
  return distRomi;
}