
#ifndef ROBOCUP_STRATEGY_DRONE_STATE_H
#define ROBOCUP_STRATEGY_DRONE_STATE_H
#include <ros/ros.h>
//用于与px4交互
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
//用于和movebase交互
#include <geometry_msgs/PoseStamped.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TwistStamped.h>
namespace drone_state{
class Drone_state {
public:
    ros::NodeHandle nh;
    Drone_state* get_instance();
private:
    static Drone_state* instance;
    Drone_state();
    ~Drone_state();
    //订阅者
    ros::Subscriber local_pos_sub;
    ros::Subscriber move_base_vel_sub;
    ros::Subscriber px4_state_sub;
    //发布者
    ros::Publisher  drone_cmd_vel_pub;
    ros::Publisher  move_base_simple_goal_pub;
    //客户端
    ros::ServiceClient set_mode_client;
    ros::ServiceClient arming_client;
    //回调函数
    void local_pos_cb(const geometry_msgs::PoseStamped::ConstPtr &msg);
    void move_base_vel_cb(const geometry_msgs::Twist::ConstPtr &msg);
    void px4_state_cb(const mavros_msgs::State::ConstPtr &msg);
    void register_sub_and_pub();
    //飞机状态
    mavros_msgs::State drone_state;
    geometry_msgs::PoseStamped drone_local_pos;
    geometry_msgs::TwistStamped drone_pub_vel;
    geometry_msgs::Twist move_base_vel;
    geometry_msgs::PoseStamped move_base_simple_goal;
    //把对象内数据发到ros
    void pub_to_ros();
};
}

#endif //ROBOCUP_STRATEGY_DRONE_STATE_H
