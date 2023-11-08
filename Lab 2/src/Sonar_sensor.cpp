#include <Romi32U4.h>
#include "Sonar_sensor.h"

void SonarSensor::Init(void)
{
    pinMode(pin_TRIG,OUTPUT);
    pinMode(pin_ECHO, INPUT);   
}

float SonarSensor::PrintData(void)
{
    Serial.println(ReadData());
}

float SonarSensor::ReadData(void)
{
    //assignment 1.2
    //read out and calibrate your sonar sensor, to convert readouts to distance in [cm]
    digitalWrite(pin_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(pin_TRIG, LOW);
    float duration = pulseIn(pin_ECHO, HIGH);
    /*
        Use the above and measure the duration vs the distance. Once it is measured, find line of best fit (Should NOT be inverse). 
        Then plug in values below and uncomment the below section. Delete extra return statement
    */

   /*
   float m = 1; //slope
   float b = 1; //b-intercept
   int sensorAngle = 45; //angle of sensor
   int sensorFromEdge = 2; //distance from edge of romi in cm
   float distSensor = m * duration + b; //sensor reading in cm
   float distRomi = float distRomi = distSensor * cos(sensorAngle) + sensorFromEdge; //triangle math to romi distance
   return distRomi;
   */

   return duration;
}