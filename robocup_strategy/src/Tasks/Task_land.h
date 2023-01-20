#ifndef ROBOCUP_STRATEGY_TASK_LAND_H
#define ROBOCUP_STRATEGY_TASK_LAND_H
#include "Task_base.h"
#include "../Interface/Drone_state.h"
class Task_land :public Task_base{
public:
    Task_land();
    ~Task_land();
    void run();
protected:
    mavros_msgs::SetMode land_set_mode;
    void close();
};


#endif //ROBOCUP_STRATEGY_TASK_LAND_H
