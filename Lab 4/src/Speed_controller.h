#ifndef SPEED_CONTROLLER
#define SPEED_CONTROLLER

#include <Romi32U4.h>

class SpeedController{
    private:
        const float rightKp = 0.5; //Adapt the parameters until your robot moves at the speed you command it to drive
        const float leftKp = 0.7;
        const float Ki = 0.15; 
        float E_left = 0; 
        float E_right = 0;
        float turnError = .2;//error for accurate turning  after 
        int counts = ((140.0 * 3.14/2)/(70.0 * 3.14)) * 1440.0 * turnError; //assignment

    public:
        void Init(void);
        void Run(float, float); //speed left, speed right
        boolean Turn(int,int); //degrees, direction of rotation: 0->left, 1->right
        boolean Straight(int, int); //speed, duration
        boolean Curved(int,int,int); //speed left, speed right, duration
        void Stop(void);
};

#endif