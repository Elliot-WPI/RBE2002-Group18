#include <Romi32U4.h>
#include "chassis.h"
#include "openmv.h"
int findX(tag){
    return (tag.cx- px)*(Z/fx);
}
int findY(tag){
    return (tag.cx- px)*(Z/fx);
}
