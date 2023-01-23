

#ifndef ROBOCUP_STRATEGY_NAVIGATION_H
#define ROBOCUP_STRATEGY_NAVIGATION_H
#include "../src/Interface/Drone_state.h"
#include "Task_base.h"
#include "../src/Utils/Utils.h"
#include "../src/Utils/Pid_controler.h"
#include "math.h"

class Navigation: public Task_base{
public:
    Navigation();
    ~Navigation();
    //更改move_base的目标并重设任务状态为开始
    void set_move_base_goal(geometry_msgs::PoseStamped move_base_goal_to_set);
    int get_current_target_index();
    void run();
protected:
    geometry_msgs::PoseStamped mov_base_goal;
    geometry_msgs::TwistStamped speed_to_pub;
    double normal_attitude;
    int current_target_index;
    Pid_controler y_pid_controler;
};


#endif //ROBOCUP_STRATEGY_NAVIGATION_H
