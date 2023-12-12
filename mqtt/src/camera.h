#include <Wire.h>
#include "openmv.h"
class Camera{
    private:
        const float height;
        const float px;
        const float py;
        const float fx;
        const float fy;
    public:
        int findX(AprilTagDatum);
        int findY(AprilTagDatum);
};
extern Camera romiCamera;
