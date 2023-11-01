#pragma once

#include <Arduino.h>
#include <Romi32U4Motors.h>

class Chassis
{
protected:
    // Kinematic parameters -- TODO: These are grossly wrong!!!
    float wheel_track = 1; //cm
    float wheel_diam = 1; //cm
    float ticks_per_rotation = 100; // from the datasheet
    float cmPerEncoderTick = 3.1416 * wheel_diam / ticks_per_rotation;
    float robotRadius = wheel_track / 2.0;

public:
    uint8_t readyToPID = 0;

    Chassis(void);

    void init(void);
    bool loop(void);
    void update(void);

    void setMotorEfforts(int16_t left, int16_t right) 
        {leftMotor.setMotorEffort(left); rightMotor.setMotorEffort(right);}

    void setMotorTargetSpeeds(float leftTicksPerInterval, float rightTicksPerInterval);
};

extern Chassis chassis;