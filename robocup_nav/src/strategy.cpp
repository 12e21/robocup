#include <ros/ros.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/CommandBool.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/State.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/TwistStamped.h>
//mavros_msgs中包含运行 MAVROS 包提供的服务和主题所需的所有自定义消息

geometry_msgs::PoseStamped local_pos;
void local_pos_cb(const geometry_msgs::PoseStamped::ConstPtr& msg){
    local_pos = *msg;
}
//创建一个简单的回调来保存自动驾驶仪的当前状态。这将允许我们检查连接、武装和外线标志
mavros_msgs::State current_state;
void state_cb(const mavros_msgs::State::ConstPtr& msg){
    current_state = *msg;
}
geometry_msgs::Twist mov_vel;
void mov_vel_cb(const geometry_msgs::Twist::ConstPtr&msg){
    mov_vel=*msg;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "offb_node");
    ros::NodeHandle nh;
    //实例化一个发布者来发布命令的本地位置和适当的客户端来请求武装和模式更改
    ros::Subscriber state_sub = nh.subscribe<mavros_msgs::State>
            ("mavros/state", 10, state_cb);
    // 订阅无人机当前位置（反馈消息）
    ros::Subscriber local_pos_sub = nh.subscribe<geometry_msgs::PoseStamped>
            ("mavros/local_position/pose", 10, local_pos_cb);
    ros::Subscriber mov_vel_sub=nh.subscribe<geometry_msgs::Twist>("move_base/cmd_vel",10,mov_vel_cb);
    //速度发布
    /*ros::Publisher local_twist_pub = nh.advertise<geometry_msgs::Twist>
            ("/mavros/setpoint_velocity/cmd_vel_unstamped", 10);*/
    ros::Publisher local_twist_pub=nh.advertise<geometry_msgs::TwistStamped>("/mavros/setpoint_velocity/cmd_vel",10);
    ros::ServiceClient arming_client = nh.serviceClient<mavros_msgs::CommandBool>
            ("mavros/cmd/arming");
    ros::ServiceClient set_mode_client = nh.serviceClient<mavros_msgs::SetMode>
            ("mavros/set_mode");

    //发布速率必须快于 2 Hz
    ros::Rate rate(20.0);

    //等待 MAVROS 和自动驾驶仪之间建立连接
    while(ros::ok() && !current_state.connected){
        ros::spinOnce();
        rate.sleep();
    }
    //设置悬停位置
    /*geometry_msgs::Twist twist;
    twist.linear.x=0;
    twist.linear.y=0;
    twist.linear.z=0;*/
    geometry_msgs::TwistStamped stwist;
    stwist.header.frame_id="base_link";
    stwist.twist.linear.x=0;
    stwist.twist.linear.y=0;
    stwist.twist.linear.z=0;



    //在进入Offboard模式之前开始流式传输设定值
    for(int i = 100; ros::ok() && i > 0; --i){
        local_twist_pub.publish(stwist);
        ros::spinOnce();
        rate.sleep();
    }
    //将自定义模式设置为OFFBOARD
    mavros_msgs::SetMode offb_set_mode;
    offb_set_mode.request.custom_mode = "OFFBOARD";

    mavros_msgs::CommandBool arm_cmd;
    arm_cmd.request.value = true;

    ros::Time last_request = ros::Time::now();

    while(ros::ok()){
        //尝试切换到Offboard模式,将服务调用间隔 5 秒，以免自动驾驶仪被请求淹没
        if( current_state.mode != "OFFBOARD" &&
            (ros::Time::now() - last_request > ros::Duration(5.0))){
            if( set_mode_client.call(offb_set_mode) &&
                offb_set_mode.response.mode_sent){
                ROS_INFO("Offboard enabled");
            }
            last_request = ros::Time::now();
        } else {
            //武装四边形以允许它飞行,服务调用间隔 5 秒，以免自动驾驶仪被请求淹没
            if( !current_state.armed &&
                (ros::Time::now() - last_request > ros::Duration(5.0))){
                if( arming_client.call(arm_cmd) &&
                    arm_cmd.response.success){
                    ROS_INFO("Vehicle armed");
                }
                last_request = ros::Time::now();
            }
        }
        stwist.twist=mov_vel;
        stwist.twist.angular.z=0;
        stwist.twist.angular.x=0;
        stwist.twist.angular.y=0;

        if(local_pos.pose.position.z<0.50)
        {
            stwist.twist.linear.z=0.5;
        } else if (local_pos.pose.position.z>0.60)
        {
            stwist.twist.linear.z=-0.2;
        } else
        {
            stwist.twist.linear.z=0;
        }
        local_twist_pub.publish(stwist);
        ros::spinOnce();
        rate.sleep();
    }

    return 0;
}
