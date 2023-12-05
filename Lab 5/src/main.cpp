#include <Arduino.h>
#include <Romi32U4.h>
#include "chassis.h"
//sensors
#include "Encoders.h"
//behaviors
#include "Speed_controller.h"
//camera
#include <Wire.h>
#include "openmv.h"

enum ROBOT_STATE {ROBOT_IDLE, FIND_TAGS, ROBOT_DRIVING};
ROBOT_STATE robot_state = ROBOT_IDLE;

Romi32U4ButtonA buttonA; 
SpeedController PIcontroller;

RomiChassis chassis;

OpenMV camera;

AprilTagDatum tag;

int area;
int cx;

void setup() {
  Serial.begin(115200);
  delay(1000);

  Wire.begin();
  Wire.setClock(100000ul);

  Serial.println(F("Welcome."));

  PIcontroller.Init();
}

void loop() {
  switch(robot_state)
  {
    case ROBOT_IDLE:{
      Serial.println("idle");
      if(buttonA.getSingleDebouncedRelease()) robot_state = FIND_TAGS;
      break;
    }

    case FIND_TAGS:{
      uint8_t tagCount = camera.getTagCount();
      if(tagCount)
      {
        Serial.println("in tagCount");
        camera.readTag(tag);
        Serial.println(tag.id);
        if(tag.id == 1){
          Serial.println("in if statement");
          area = tag.w*tag.h;
          cx = tag.cx;
          Serial.println(area);
          robot_state = ROBOT_DRIVING;
          Serial.println("robot driving");
          break;
        }
      }
      if(buttonA.getSingleDebouncedRelease()) 
      {
        //PIcontroller.Stop();
        robot_state = ROBOT_IDLE;
      }
      Serial.println("got to break");
      Serial.println(robot_state);
      break;
  }
    case ROBOT_DRIVING:{
      Serial.println("in driving");
      while((cx >= 85 || cx <= 75) || (area >= 1250 || area <= 1200)){
        if(cx != -1){
          if(buttonA.getSingleDebouncedRelease()) 
          {
            chassis.Stop();
            robot_state = ROBOT_IDLE;
          }
          chassis.UpdateEffortDriveWheelsPI(cx, area);
          camera.readTag(tag);
          cx = tag.cx;
          area = tag.w*tag.h;
          Serial.print("Value = ");
          Serial.println(cx);
        }
        else{
          chassis.Stop();
          robot_state = FIND_TAGS;
          break;
        }
      }
      chassis.Stop();
      robot_state = FIND_TAGS;
      if(buttonA.getSingleDebouncedRelease()) 
      {
        //PIcontroller.Stop();
        robot_state = ROBOT_IDLE;
      }
    }
    default:
      break;
  }
}

/*
uint8_t FindAprilTags()
{
    uint8_t tagCount = camera.getTagCount();
    if(tagCount) 
    {
      Serial.println(tagCount);
      AprilTagDatum tag;
      if(camera.readTag(tag))
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
    }

    return tagCount;
}

void loop() 
{ 
  delay(1); //calm things down for a sec...
  FindAprilTags();
}
*/