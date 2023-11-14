#include <Romi32U4.h>
#include "Behaviors.h"
#include "Median_filter.h"
#include "IMU.h"
#include "Speed_controller.h"

Behaviors collisionBehavior;

int driveIgnoreCounter;
int driveIgnoreMax = 100;

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
    if(data[0] > 30){
    Serial.println("x");
    Serial.print(data[0]);
    }
    if(data[1] > 30){
    Serial.println("y");
    Serial.print(data[1]);
    }
    if(data[2] > zThreshold){return 0;}
    else if((data[0] > threshold) || (data[1] > threshold)) return 1;
    else return 0;
}

boolean Behaviors::DetectBeingPickedUp(void)
{
    auto data_acc = LSM6.ReadAcceleration();
    data[2] = med_z.Filter(data_acc.Z)*0.061; // if having trouble detecting being picked up delete this line and use data[2] collected in detect collision
    if(abs(data[2]) > zThreshold){return 1;}//please work
    else return 0;
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
    driveIgnoreCounter++;
        if(buttonA.getSingleDebouncedRelease()){ //transition condition
            robot_state = IDLE; //next state
            PIcontroller.Stop(); //action
            driveIgnoreCounter = 0;
        }
        else if(collisionBehavior.DetectCollision() && (driveIgnoreCounter > driveIgnoreMax)){
            robot_state = REVERSE; // next state
            PIcontroller.Stop(); //action
            driveIgnoreCounter = 0;
        }
        else if (collisionBehavior.DetectBeingPickedUp()){
            robot_state = IDLE;// next state
            PIcontroller.Stop();
            driveIgnoreCounter = 0;
        }
        else{
            PIcontroller.Run(80,80);
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
            robot_state = TURN;
        }
        break;

    case TURN:
        if(buttonA.getSingleDebouncedRelease()){ //transition condition
            robot_state = IDLE; //next state
            PIcontroller.Stop(); //action
        }
        else if (collisionBehavior.DetectBeingPickedUp()){
            robot_state = IDLE;// next state
            PIcontroller.Stop();
        }
        else{
            PIcontroller.Turn(90,1);
            robot_state = DRIVE;
        }
        break;
    }
}