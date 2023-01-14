#include "Interface/Drone_state.h"
#include "Tasks/Take_off.h"
#include "Tasks/Navigation.h"
int main(int argc,char **argv)
{
    //初始化
    ros::init(argc,argv,"general_strategy");
    Take_off p_task_off;
    Navigation p_task_navigation;

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
                Drone_state::get_instance()->task_id=NAVIGATION;
            }
        }
/*************NAVIGATION*************/
        if(Drone_state::get_instance()->task_id==NAVIGATION)
        {
            //测试用导航点
            geometry_msgs::PoseStamped test_nav_target;
            test_nav_target.header.frame_id="map";
            test_nav_target.pose.position.x=4;
            test_nav_target.pose.orientation.w=1;
            //设置导航点
            p_task_navigation.set_move_base_goal(test_nav_target);
            p_task_navigation.run();
            if(p_task_navigation.get_task_state()==TASK_FINISH)
            {
                ROS_INFO("nav_goal reached!");
                return 0;
            }
        }

        Drone_state::get_instance()->pub_to_ros();
        Drone_state::get_instance()->update();
    }
    return 0;
}
