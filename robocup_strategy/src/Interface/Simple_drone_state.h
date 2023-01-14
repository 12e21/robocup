
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

    class Drone_state {
    public:
        ros::NodeHandle nh;
        ros::Rate loop_rate;
        static Drone_state* get_instance();

        //属性
        //get
        mavros_msgs::State get_state();
        geometry_msgs::PoseStamped get_local_pose();
        ros::ServiceClient get_arming_client();
        ros::ServiceClient get_set_mode_client();
        geometry_msgs::Twist get_move_base_vel();
        //set
        void set_drone_speed(geometry_msgs::TwistStamped speed_to_pub);
        void set_move_base_simple_goal(geometry_msgs::PoseStamped goal_to_pub);

        //更新
        void update();

        //把对象内数据发到ros
        void pub_to_ros();

    protected:
        static Drone_state* instance;
        Drone_state();
        ~Drone_state();

        //订阅发布者
        ros::Subscriber local_pos_sub;
        ros::Subscriber px4_state_sub;
        ros::Subscriber move_base_vel_sub;
        ros::Publisher  drone_cmd_vel_pub;
        ros::Publisher  move_base_simple_goal_pub;
        ros::ServiceClient set_mode_client;
        ros::ServiceClient arming_client;


        //回调函数
        void local_pos_cb(const geometry_msgs::PoseStamped::ConstPtr &msg);
        void px4_state_cb(const mavros_msgs::State::ConstPtr &msg);
        void move_base_vel_cb(const geometry_msgs::Twist::ConstPtr &msg);
        void register_sub_and_pub();

        //飞机状态
        mavros_msgs::State drone_state;
        geometry_msgs::PoseStamped drone_local_pos;
        geometry_msgs::TwistStamped drone_pub_vel;
        geometry_msgs::Twist move_base_vel;
        geometry_msgs::PoseStamped move_base_simple_goal;

    };

#endif //ROBOCUP_STRATEGY_DRONE_STATE_H
