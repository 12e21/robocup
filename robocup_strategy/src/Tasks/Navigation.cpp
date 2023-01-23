
#include "Navigation.h"

//构造&析构函数
Navigation::Navigation() {
    task_state=TASK_START;
    this->normal_attitude=1;
    this->current_target_index=0;
}
Navigation::~Navigation()=default;
//设置导航点并重设任务状态
// TODO 后续设置导航点尝试改成参数形式
void Navigation::set_move_base_goal(geometry_msgs::PoseStamped move_base_goal_to_set) {
    this->mov_base_goal=move_base_goal_to_set;
    Drone_state::get_instance()->set_move_base_simple_goal(this->mov_base_goal);
    task_state=TASK_START;
}

int Navigation::get_current_target_index() {
    return this->current_target_index;
}
void Navigation::run() {
    //获取move_base发来的速度处理并回发给接口
    this->speed_to_pub.header.frame_id="base_link";
    this->speed_to_pub.twist=Drone_state::get_instance()->get_move_base_vel();
    //角速度设为0
    this->speed_to_pub.twist.angular.x=0;
    this->speed_to_pub.twist.angular.y=0;
    this->speed_to_pub.twist.angular.z=0;
    //垂直速度控制
    geometry_msgs::PoseStamped drone_local_pose=Drone_state::get_instance()->get_local_pose();
    double diff=this->normal_attitude-drone_local_pose.pose.position.z;
    this->speed_to_pub.twist.linear.z=this->y_pid_controler.diff_to_output(diff);
    Drone_state::get_instance()->set_drone_speed(speed_to_pub);

    //检测是否到达位置
    double distance= Utils::calculate_distance(this->mov_base_goal,
                                        Drone_state::get_instance()->get_local_pose());
    if(distance<0.15)
    {
        this->task_state=TASK_FINISH;
        this->current_target_index+=1;
    }
}