#ifndef __CHASSIS_H
#define __CHASSIS_H

#include <Romi32U4.h>

class RomiChassis{
    private:
        //Romi specific constants:
        // !!! ATTENTION !!!
        const float N_wheel = 1440; //how many counts equate to one wheel rotation?
        const float R_wheel = 70/2; //what is the radius of a Romi wheel in [mm]
        const float C_wheel = 2*PI*R_wheel; //circumference of wheel

        //declare variables for PI controller
        float current_cx = 0;
        float current_area = 0;
        int target_cx = 80;
        int target_area = 1225;
        float KpCx = 0.15;
        float KiCx = 0.05;
        float KpArea = 0.03;
        float KiArea = 0.01;
        float E_left = 0; //accumulated errors
        float E_right = 0;

        //encoder and motor objects
        Romi32U4Encoders encoders;
        Romi32U4Motors motors;

        //declare variables for for keeping track of counts and conversion to velocities
        uint32_t start_time = 0;
        uint32_t end_time = 0;
        float interval = 50; // in [ms]
        uint32_t last_update = 0;
        int count_left = 0;
        int count_right = 0;
        int prev_count_left = 0;
        int prev_count_right = 0;
        float previous_time = 0;


    public:
        void UpdateEffortDriveWheelsPI(int a, int b);
        void setSpeed(int a, int b);
        void Stop(void);
};

extern RomiChassis chassis;

#endif