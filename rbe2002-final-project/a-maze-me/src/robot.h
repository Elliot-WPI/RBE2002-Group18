#pragma once

#include <Arduino.h>
#include <Chassis.h>

enum ROBOT_STATE
{   
    ROBOT_IDLE, 
    ROBOT_STANDOFF, 
    ROBOT_LEFT,
    ROBOT_RIGHT,
    ROBOT_DRIVING,
    ROBOT_LINING,
    ROBOT_CENTERING,    //for centering on an intersection
    ROBOT_WAITING,      //waiting for an observation
};

void initialize(void);

void idle(void);

void handleKeyCode(int16_t keyCode);

void processDistanceSensors(void);
void handleNewDistanceReading(float distanceReading);
