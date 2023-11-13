#ifndef SPEED_CONTROLLER
#define SPEED_CONTROLLER

#include <Romi32U4.h>

class SpeedController{
    private:
        const float Kp = 0.5; //Adapt the parameters until your robot moves at the speed you command it to drive
        const float Ki = 0.1; 
        float E_left = 0; 
        float E_right = 0;
        int counts = ((141 * 3.14/2)/(70 * 3.14)) * 1440; //assignment 1: convert degrees into counts
        float turnError = .98;//error for accurate turning  after 

    public:
        void Init(void);
        void Run(float, float); //degrees and direction: 0->left, 1->right
        boolean Turn(int,int);
        boolean Reverse(int, int);
        void Stop(void);
};

#endif