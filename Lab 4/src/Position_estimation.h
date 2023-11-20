#ifndef POSITION_ESTIMATION
#define POSITION_ESTIMATION

#include <Romi32U4.h>

class Position{
    private:
        float x, y, theta;
        unsigned long time_prev, time_now;
        const float l = 140; //assignment
        
    public:
        struct pose_data {
            float X;
            float Y;
            float THETA;
        };
        void Init(void);
        void UpdatePose(float,float);
        void updatePoseStraight(float, float);
        void updatePoseTurn(float, float);
        pose_data ReadPose(void);
        void PrintPose(void);
        void Stop(void);
};

#endif