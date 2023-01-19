#ifndef ROBOCUP_STRATEGY_UTILS_H
#define ROBOCUP_STRATEGY_UTILS_H
#include "geometry_msgs/PoseStamped.h"

class Utils {
public:
    static double calculate_distance(geometry_msgs::PoseStamped point1,geometry_msgs::PoseStamped point2);
protected:

};


#endif //ROBOCUP_STRATEGY_UTILS_H
