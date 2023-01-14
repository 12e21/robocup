#include "Interface/Drone_state.h"
#include "Tasks/Take_off.h"
int main(int argc,char **argv)
{
    ros::init(argc,argv,"general_strategy");
    Take_off p_task_off;

    while (ros::ok()){
/**************TAKE_OFF***************/
        if(Drone_state::get_instance()->task_id==TAKEOFF)
        {
            if(p_task_off.get_if_open()== true)
            {
                p_task_off.run();
            } else
            {
                p_task_off.open();
            }
            if(p_task_off.get_task_state()==TASK_FINISH)
            {
                return 0;
            }
        }

        Drone_state::get_instance()->pub_to_ros();
        Drone_state::get_instance()->update();
    }
    return 0;
}
