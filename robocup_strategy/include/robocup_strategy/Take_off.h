
#ifndef ROBOCUP_STRATEGY_TAKE_OFF_H
#define ROBOCUP_STRATEGY_TAKE_OFF_H
#include "../Interface/Simple_drone_state.h"
#include "Task_base.h"

class Take_off: public Task_base{
public:
    Take_off();
    ~Take_off();
    void open();
    void run();
    bool get_if_open();
protected:
    //要回传的速度
    geometry_msgs::TwistStamped speed_to_pub;
    bool if_open;
    mavros_msgs::SetMode offb_set_mode;
    mavros_msgs::CommandBool arm_cmd;
    double normal_attitude;
    ros::Time last_request=ros::Time::now();

};

#endif //ROBOCUP_STRATEGY_TAKE_OFF_H
