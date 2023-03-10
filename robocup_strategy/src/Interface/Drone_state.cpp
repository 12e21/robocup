
#include "Drone_state.h"
//获取单例
Drone_state* Drone_state::instance= nullptr;
Drone_state* Drone_state::get_instance() {
    if(Drone_state::instance== nullptr)
    {
        Drone_state::instance=new Drone_state();
    }
    return instance;
}

//构造&析构函数
Drone_state::Drone_state()
     :nh("~"), loop_rate(10)
{
    //注册订阅和服务
    Drone_state::register_sub_and_pub();
    //速度初始化
    drone_pub_vel.twist.linear.x=0;
    drone_pub_vel.twist.linear.y=0;
    drone_pub_vel.twist.linear.z=0;
    //move_base目标初始化(防止报错)
    move_base_simple_goal.header.frame_id="map";
    move_base_simple_goal.pose.orientation.w=1;
    //任务ID
    task_id=TAKEOFF;
}
Drone_state::~Drone_state()=default;

//注册订阅者和发布者
void Drone_state::register_sub_and_pub() {
    //注册订阅者
    this->local_pos_sub=nh.subscribe<geometry_msgs::PoseStamped>("/mavros/local_position/pose",
                                                                 10,&Drone_state::local_pos_cb,this);
    this->px4_state_sub=nh.subscribe<mavros_msgs::State>("/mavros/state",
                                                         10,&Drone_state::px4_state_cb, this);
    this->move_base_vel_sub=nh.subscribe<geometry_msgs::Twist>("/move_base/cmd_vel",
                                                               10,&Drone_state::move_base_vel_cb, this);
    this->vision_info_sub=nh.subscribe<uav_robotcup::vision_information>("/robo_air/vision_information",
                                                                         10,&Drone_state::vision_info_cb, this);
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
void Drone_state::px4_state_cb(const mavros_msgs::State::ConstPtr &msg) {
    this->drone_state=*msg;
}
void Drone_state::move_base_vel_cb(const geometry_msgs::Twist::ConstPtr &msg) {
    this->move_base_vel=*msg;
}

void Drone_state::vision_info_cb(const uav_robotcup::vision_information::ConstPtr &msg) {
    this->vision_information=*msg;
}

//属性
//get
mavros_msgs::State Drone_state::get_state() {
    return this->drone_state;
}
ros::ServiceClient Drone_state::get_arming_client() {
    return arming_client;
}
ros::ServiceClient Drone_state::get_set_mode_client() {
    return set_mode_client;
}
geometry_msgs::PoseStamped Drone_state::get_local_pose() {
    return this->drone_local_pos;
}
geometry_msgs::Twist Drone_state::get_move_base_vel() {
    return this->move_base_vel;
}

uav_robotcup::vision_information Drone_state::get_vision_info() {
    return this->vision_information;
}
//set
void Drone_state::set_drone_speed(geometry_msgs::TwistStamped speed_to_pub) {
    this->drone_pub_vel=speed_to_pub;
}
void Drone_state::set_move_base_simple_goal(geometry_msgs::PoseStamped goal_to_pub) {
    this->move_base_simple_goal=goal_to_pub;
}
//更新
void Drone_state::update() {
    ros::spinOnce();
    loop_rate.sleep();
}
//发布速度
void Drone_state::pub_to_ros() {
    //发布并显示速度
    /*ROS_INFO("pub_speed:(%.2lf,%.2lf,%.2lf)",drone_pub_vel.twist.linear.x,
             drone_pub_vel.twist.linear.y,drone_pub_vel.twist.linear.z);*/
    drone_cmd_vel_pub.publish(this->drone_pub_vel);
    //发布并显示move_base的目标
    ROS_INFO("pub_goal:(%.2lf,%.2lf,%.2lf)",this->move_base_simple_goal.pose.position.x,
             this->move_base_simple_goal.pose.position.y,this->move_base_simple_goal.pose.position.z);
    move_base_simple_goal_pub.publish(this->move_base_simple_goal);
}

