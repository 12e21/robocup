

#ifndef ROBOCUP_STRATEGY_NAVIGATION_H
#define ROBOCUP_STRATEGY_NAVIGATION_H
#include "../src/Interface/Drone_state.h"
#include "Task_base.h"
#include "math.h"

class Navigation: public Task_base{
public:
    Navigation();
    ~Navigation();
    void set_move_base_goal(geometry_msgs::PoseStamped move_base_goal_to_set);
    void run();
protected:
    geometry_msgs::PoseStamped mov_base_goal;
    //TODO 把计算距离函数移到杂项文件夹中
    double calculate_distance(geometry_msgs::PoseStamped point1,geometry_msgs::PoseStamped point2);
    geometry_msgs::TwistStamped speed_to_pub;
    double normal_attitude;
};


#endif //ROBOCUP_STRATEGY_NAVIGATION_H
