#include  "Position_estimation.h"
#include "Encoders.h"

Encoder RomiEncoders;
float x = 0;
float y = 0;
float theta = 0;
unsigned long time_prev = millis();
unsigned long time_now = 0;
float timeStep = 0.05;

void Position::Init(void)
{
    time_prev = millis();
    x = 0;
    y = 0;
    theta = 0;
}

void Position::Stop(void)
{
    time_prev = millis();
    x = 0; 
    y = 0;
    theta = 0;
}

Position::pose_data Position::ReadPose(void)
{
    return {x,y,theta};
}

void Position::PrintPose(void)
{
    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(theta);
}

void updatePoseTurn(float target_speed_left, float target_speed_right)
{
    float w;
    float R;
    float V;
    R = (target_speed_right+target_speed_left)/2*(target_speed_right-target_speed_left);
    w = (target_speed_left-target_speed_right)/0.142875;
    theta = theta + w * timeStep;
    V = (target_speed_left+target_speed_right)/2;
    x = x + V*cos(theta) * timeStep;
    y = y + V*sin(theta)*timeStep;
}
void updatePoseStraight(float target_speed_left, float target_speed_right)
{
    float V = (target_speed_left+target_speed_right)/2;
    x = x + V * cos(theta) + timeStep;
    y = y + V* sin(theta) + timeStep;
    
}
void Position::UpdatePose(float target_speed_left, float target_speed_right)
{
    
    time_now = millis();

    if(time_now - time_prev >= 50) //update every 50ms for practical reasons
    {    
    if(abs(RomiEncoders.ReadVelocityLeft()-RomiEncoders.ReadVelocityRight()) > 10)
    {
        updatePoseStraight(target_speed_left, target_speed_right);
    }
    else{    
        updatePoseTurn(target_speed_left, target_speed_right);
    }
    }
}

