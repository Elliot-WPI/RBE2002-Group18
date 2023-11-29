#include <Arduino.h>
#include <Wire.h>
#include <openmv.h>

OpenMV camera;

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial1.begin(115200);

  Serial.println(F("Welcome."));
}

uint8_t FindAprilTags(void)
{
  AprilTagDatum tag;
  if(camera.checkUART(tag))
  {
    Serial.print(F("Tag [cx="));
    Serial.print(tag.cx);
    Serial.print(F(", cy="));
    Serial.print(tag.cy);
    Serial.print(F(", w="));
    Serial.print(tag.w);
    Serial.print(F(", h="));
    Serial.print(tag.h);
    Serial.print(F(", id="));
    Serial.print(tag.id);
    Serial.print(F(", rot="));
    Serial.print(tag.rot);
    Serial.println(F("]"));
  }

  return 0;
}

void loop() 
{ 
  delay(1); //calm things down for a sec...
  FindAprilTags();
}
