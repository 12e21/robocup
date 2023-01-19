

#ifndef ROBOCUP_STRATEGY_NAVIGATION_H
#define ROBOCUP_STRATEGY_NAVIGATION_H
#include "../src/Interface/Drone_state.h"
#include "Task_base.h"
#include "../src/Utils/Utils.h"
#include "math.h"

class Navigation: public Task_base{
public:
    Navigation();
    ~Navigation();
    void set_move_base_goal(geometry_msgs::PoseStamped move_base_goal_to_set);
    void run();
protected:
    geometry_msgs::PoseStamped mov_base_goal;
    geometry_msgs::TwistStamped speed_to_pub;
    double normal_attitude;
};


#endif //ROBOCUP_STRATEGY_NAVIGATION_H
