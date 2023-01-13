
#include "Drone_state.h"
using namespace drone_state;
Drone_state* Drone_state::instance= nullptr;

Drone_state *Drone_state::get_instance() {
    if(Drone_state::instance== nullptr)
    {
       Drone_state::instance=new Drone_state();
    }
    return instance;
}
//TODO 初始化函数
//注册订阅者和发布者
void Drone_state::register_sub_and_pub() {
    //注册订阅者
    this->local_pos_sub=nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose",
                                                           10,&Drone_state::local_pos_cb,this);
    this->move_base_vel_sub=nh.subscribe<geometry_msgs::Twist>("move_base/cmd_vel",
                                                         10,&Drone_state::move_base_vel_cb, this);
    this->px4_state_sub=nh.subscribe<mavros_msgs::State>("/mavros/state",
                                                   10,&Drone_state::px4_state_cb, this);
    //注册发布者
    this->drone_cmd_vel_pub=nh.advertise<geometry_msgs::TwistStamped>("/mavros/setpoint_velocity/cmd_vel",10);
    this->move_base_simple_goal_pub=nh.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",10);
    //注册客户端
    this->set_mode_client=nh.serviceClient<mavros_msgs::SetMode>("/mavros/set_mode");
    this->arming_client=nh.serviceClient<mavros_msgs::CommandBool>("/mavros/cmd/arming");
}
//回调函数将收到消息赋给对象
void Drone_state::local_pos_cb(const geometry_msgs::PoseStamped::ConstPtr &msg) {
    this->drone_local_pos=*msg;
}

void Drone_state::move_base_vel_cb(const geometry_msgs::Twist::ConstPtr &msg) {
    this->move_base_vel=*msg;
}

void Drone_state::px4_state_cb(const mavros_msgs::State::ConstPtr &msg) {
    this->drone_state=*msg;
}

void Drone_state::pub_to_ros() {
    //发布并显示速度
    ROS_INFO("pub_speed:(%.2lf,%.2lf,%.2lf)",drone_pub_vel.twist.linear.x,
             drone_pub_vel.twist.linear.y,drone_pub_vel.twist.linear.z);
    drone_cmd_vel_pub.publish(drone_pub_vel);
    //发布并显示move_base_goal
    ROS_INFO("pub_goal:(%.2lf,%.2lf)",move_base_simple_goal.pose.position.x,
             move_base_simple_goal.pose.position.y);
    move_base_simple_goal_pub.publish(move_base_simple_goal);
}
