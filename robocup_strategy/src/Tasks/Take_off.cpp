
#include "Take_off.h"
Take_off::Take_off() {
    this->speed_to_pub.header.frame_id="base_link";
    this->offb_set_mode.request.custom_mode="OFFBOARD";
    this->arm_cmd.request.value= true;
    this->normal_attitude=1;
    if_open= false;
}
Take_off::~Take_off() =default;
//解锁无人机
void Take_off::open() {
    ROS_INFO("opening...");
    //尝试切换到Offboard模式
    if(Drone_state::get_instance()->get_state().mode!="OFFBOARD" /*&&
            (ros::Time::now()-last_request>ros::Duration(5.0))*/){
        if(Drone_state::get_instance()->get_set_mode_client().call(this->offb_set_mode)&&
        offb_set_mode.response.mode_sent){
            ROS_INFO("Offboard enabled");
        }
        //last_request=ros::Time::now();
    } else{
        //尝试解锁无人机
        if(!Drone_state::get_instance()->get_state().armed/*&&
                (ros::Time::now()-last_request>ros::Duration(5))*/){
            if(Drone_state::get_instance()->get_arming_client().call(arm_cmd)&&
            arm_cmd.response.success){
                ROS_INFO("Vehicle armed");
                if_open= true;
            }
            //last_request=ros::Time::now();
        }
    }
}
//控制无人机起飞
void Take_off::run() {
    ROS_INFO("take off...");
    ROS_INFO("normal attitude: %.2lf",this->normal_attitude);
    geometry_msgs::PoseStamped drone_local_pose=Drone_state::get_instance()->get_local_pose();
    //生成速度
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
    //把速度传回接口
    Drone_state::get_instance()->set_drone_speed(speed_to_pub);
    //检测是否起飞到一定高度
    if(Drone_state::get_instance()->get_local_pose().pose.position.z>normal_attitude)
    {
        task_state=TASK_FINISH;
    }
}

//属性
bool Take_off::get_if_open() {
    return if_open;
}