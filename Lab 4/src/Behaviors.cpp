#include <Romi32U4.h>
#include "Behaviors.h"
#include "Speed_controller.h"
#include "Position_estimation.h"

//sensors
Romi32U4ButtonA buttonA;

//motor-speed controller
SpeedController robot;

void Behaviors::Init(void)
{
    robot.Init();
}

void Behaviors::Stop(void)
{
    robot.Stop();
}

void Behaviors::Run(void)
{
    switch (robot_state)
    {
    case IDLE:
        if(buttonA.getSingleDebouncedRelease()){
            robot_state = DRIVE;
            robot.Stop();
        }
        else {
            robot_state = IDLE;
            robot.Stop();
        }
        break;
    
    case DRIVE:
        robot_state = DRIVE;
        //assignment
        robot.Curved(42,115, 6); //velocity left, velocity right, duration
        robot.Turn(90,1); //degrees, direction
        robot.Straight(30,10); //velocity, duration
        robot.Turn(90,1); //degrees, direction
        
        
        robot.Stop();
        robot_state = IDLE;
        break;
    }
}