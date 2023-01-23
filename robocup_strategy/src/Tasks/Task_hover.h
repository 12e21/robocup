#ifndef ROBOCUP_STRATEGY_TASK_HOVER_H
#define ROBOCUP_STRATEGY_TASK_HOVER_H
#include "Task_base.h"
#include "../Interface/Drone_state.h"
#include "../src/Utils/Pid_controler.h"

class Task_hover: public Task_base{
public:
    //析构&构造函数
    Task_hover();
    ~Task_hover();
    //设置盘旋点并重设任务状态为开始
    void set_hover_point(geometry_msgs::PoseStamped hover_point_to_set);
    int get_current_target_index();
    //生成盘旋速度
    void hover_at_point();
    //是否检测到目标
    bool check_if_see_target();
    void run();
protected:
    geometry_msgs::TwistStamped speed_to_pub;
    geometry_msgs::PoseStamped hover_point;
    double hover_attitude;
    double xy_threshold;
    int current_target_index;
    Pid_controler x_pid_controler;
    Pid_controler y_pid_controler;
    Pid_controler z_pid_controler;
};


#endif //ROBOCUP_STRATEGY_TASK_HOVER_H
