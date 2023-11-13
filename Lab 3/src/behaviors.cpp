#include <Romi32U4.h>
#include "Behaviors.h"
#include "Median_filter.h"
#include "IMU.h"
#include "Speed_controller.h"

Behaviors collisionBehavior;

//sensors
IMU_sensor LSM6;
Romi32U4ButtonA buttonA;

//median filter
MedianFilter med_x;
MedianFilter med_y;
MedianFilter med_z;

//motor-speed controller
SpeedController PIcontroller;

void Behaviors::Init(void)
{
    LSM6.Init();
    med_x.Init();
    med_y.Init();
    med_z.Init();
    PIcontroller.Init();
}

boolean Behaviors::DetectCollision(void)
{
    auto data_acc = LSM6.ReadAcceleration();
    data[0] = med_x.Filter(data_acc.X)*0.061;
    data[1] = med_y.Filter(data_acc.Y)*0.061;
    data[2] = med_z.Filter(data_acc.Z)*0.061;
    if((abs(data[0]) > threshold) || (abs(data[1]) > threshold)) return 1;
    else return 0;
}

boolean Behaviors::DetectBeingPickedUp(void)
{
    //assignment 2
}

void Behaviors::Stop(void)
{
    PIcontroller.Stop();
}

void Behaviors::Run(void)
{
    switch (robot_state)
    {
    case IDLE:
        if(buttonA.getSingleDebouncedRelease()){ //transition condition
            robot_state = DRIVE; 
            PIcontroller.Stop(); //action
        } 
        else { //transition condition
            robot_state = IDLE; 
            PIcontroller.Stop(); //action 
        }   
        break;
        
    case DRIVE:
        if(buttonA.getSingleDebouncedRelease()){ //transition condition
            robot_state = IDLE; //next state
            PIcontroller.Stop(); //action
        }
        else if(collisionBehavior.DetectCollision()){
            robot_state = REVERSE; // next state
            PIcontroller.Stop(); //action
        }
        else if (collisionBehavior.DetectBeingPickedUp()){
            robot_state = IDLE;// next state
            PIcontroller.Stop();
        }
        else{
            PIcontroller.Run(50,50);
        }
        break;
    
    case REVERSE:
        if(buttonA.getSingleDebouncedRelease()){ //transition condition
            robot_state = IDLE; //next state
            PIcontroller.Stop(); //action
        }
        else if (collisionBehavior.DetectBeingPickedUp()){
            robot_state = IDLE;// next state
            PIcontroller.Stop();
        }
        else{
            PIcontroller.Reverse(50,10);
        }
        break;

    case TURN:
        break;
    }
    Serial.println(robot_state);
}