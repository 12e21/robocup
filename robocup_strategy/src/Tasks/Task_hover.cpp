#include "Task_hover.h"
Task_hover::Task_hover() {
    this->task_state=TASK_START;
    this->hover_attitude=1;
    this->xy_threshold=0.1;
    this->current_target_index=0;
}
Task_hover::~Task_hover()=default;

void Task_hover::set_hover_point(geometry_msgs::PoseStamped hover_point_to_set) {
    this->hover_point=hover_point_to_set;
    this->task_state=TASK_START;
}

int Task_hover::get_current_target_index() {
    return this->current_target_index;
}
void Task_hover::hover_at_point() {

    geometry_msgs::PoseStamped drone_local_pose=Drone_state::get_instance()->get_local_pose();

    this->speed_to_pub.header.frame_id="base_link";
    this->speed_to_pub.twist.angular.x=0;
    this->speed_to_pub.twist.angular.y=0;
    this->speed_to_pub.twist.angular.z=0;
    //垂直方向速度控制
    if(drone_local_pose.pose.position.z<this->hover_attitude-0.05)
    {
        this->speed_to_pub.twist.linear.z=0.5;
    } else if(drone_local_pose.pose.position.z>hover_attitude+0.05)
    {
        this->speed_to_pub.twist.linear.z=-0.2;
    } else
    {
        this->speed_to_pub.twist.linear.z=0;
    }
    //水平x轴方向速度控制
    if(drone_local_pose.pose.position.x<(this->hover_point.pose.position.x-this->xy_threshold))
    {
        this->speed_to_pub.twist.linear.x=0.2;
    } else if(drone_local_pose.pose.position.x>(this->hover_point.pose.position.x+this->xy_threshold))
    {
        this->speed_to_pub.twist.linear.x=-0.2;
    } else
    {
        this->speed_to_pub.twist.linear.x=0;
    }
    //水平y轴方向速度控制
    if(drone_local_pose.pose.position.y<(this->hover_point.pose.position.y-this->xy_threshold))
    {
        this->speed_to_pub.twist.linear.y=0.2;
    } else if(drone_local_pose.pose.position.y>(this->hover_point.pose.position.y+this->xy_threshold))
    {
        this->speed_to_pub.twist.linear.y=-0.2;
    } else
    {
        this->speed_to_pub.twist.linear.y=0;
    }
    Drone_state::get_instance()->set_drone_speed(this->speed_to_pub);
}
bool Task_hover::check_if_see_target() {
    if(Drone_state::get_instance()->get_vision_info().if_see_blackH== true)
    {
        return true;
    } else
    {
        return false;
    }
}

void Task_hover::run() {
    this->hover_at_point();
    if(this->check_if_see_target()== true)
    {
        this->task_state=TASK_FINISH;
        this->current_target_index+=1;
    }
}