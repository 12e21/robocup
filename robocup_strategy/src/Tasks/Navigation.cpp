
#include "Navigation.h"
//TODO 计算两点间二维距离函数，需移入杂项文件
double Navigation::calculate_distance(geometry_msgs::PoseStamped point1, geometry_msgs::PoseStamped point2) {
    double distance= sqrt(pow((point1.pose.position.x-point2.pose.position.x),2)+pow((point1.pose.position.y-point2.pose.position.y),2));
    return distance;
}
//构造&析构函数
Navigation::Navigation() {
    task_state=TASK_START;
    this->normal_attitude=1;
}
Navigation::~Navigation()=default;
//设置导航点并重设任务状态
// TODO 后续设置导航点尝试改成参数形式
void Navigation::set_move_base_goal(geometry_msgs::PoseStamped move_base_goal_to_set) {
    this->mov_base_goal=move_base_goal_to_set;
    Drone_state::get_instance()->set_move_base_simple_goal(this->mov_base_goal);
    task_state=TASK_START;
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
    if(drone_local_pose.pose.position.z<this->normal_attitude-0.05)
    {
        this->speed_to_pub.twist.linear.z=0.5;
    } else if(drone_local_pose.pose.position.z>normal_attitude+0.05)
    {
        this->speed_to_pub.twist.linear.z=-0.2;
    } else
    {
        this->speed_to_pub.twist.linear.z=0;
    }
    Drone_state::get_instance()->set_drone_speed(speed_to_pub);

    //检测是否到达位置
    double distance= calculate_distance(this->mov_base_goal,
                                        Drone_state::get_instance()->get_local_pose());
    if(distance<0.15)
    {
        this->task_state=TASK_FINISH;
    }
}