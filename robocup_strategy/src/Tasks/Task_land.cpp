#include "Task_land.h"
Task_land::Task_land() {
    this->task_state=TASK_START;
}
Task_land::~Task_land() =default;

void Task_land::close() {
    this->land_set_mode.request.custom_mode="AUTO.LAND";
    if(Drone_state::get_instance()->get_set_mode_client().call(this->land_set_mode)&&
    land_set_mode.response.mode_sent){
        ROS_INFO("auto land mode enabled");
    }
}

void Task_land::run() {
    this->close();
}