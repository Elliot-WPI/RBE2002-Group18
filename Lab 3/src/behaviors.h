#ifndef BEHAVIORS
#define BEHAVIORS

#include <Romi32U4.h>

class Behaviors{
    private:
        int threshold = 400;
        int zThreshold = 1200;
        int threshold_pick_up = 1500;//redundant (ignore)
        int data[3] = {0};
        enum ROBOT_STATE {IDLE, DRIVE, REVERSE, TURN};
        ROBOT_STATE robot_state = IDLE; //initial state: IDLE
         
    public:
        void Init(void);
        void Stop(void);
        void Run(void);
        boolean DetectCollision(void);
        boolean DetectBeingPickedUp(void);
};

#endif