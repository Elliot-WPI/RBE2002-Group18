#include  "Position_estimation.h"
#include "Encoders.h"

Encoder RomiEncoders;
float x = 0;
float y = 0;
float theta = 0;
unsigned long time_prev = millis();
unsigned long time_now = 0;


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

void Position::UpdatePose(float target_speed_left, float target_speed_right)
{
    double w;
    double R;
    double V;
    time_now = millis();
    if(time_now - time_prev >= 50) //update every 50ms for practical reasons
    {    
        R = (target_speed_right+target_speed_left)/2*(target_speed_right-target_speed_left);
        w = (target_speed_left-target_speed_right)/l;
        theta = theta + w * 0.05;
        V = (target_speed_left+target_speed_right)/2;
        x = x + V*cos(theta)*0.05;
        y = y + V*sin(theta)*0.05;

        //assignment
    }
}

