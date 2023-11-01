#include <Arduino.h>
#include <MaxBotix.h>

MaxBotixSerial mbSer;

void setup() 
{
  Serial.begin(115200);
  Serial.print("Welcome!\n");
  
  mbSer.init();
}

void loop()
{
  float distance = 0;
  if(mbSer.getDistance(distance))
  {
    Serial.println(distance);
  }
}
