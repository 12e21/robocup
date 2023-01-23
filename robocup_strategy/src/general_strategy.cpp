#include "Interface/Drone_state.h"
#include "Tasks/Take_off.h"
#include "Tasks/Navigation.h"
#include "Tasks/Task_hover.h"
#include "Tasks/Task_land.h"
#include <vector>
int main(int argc,char **argv)
{
    //初始化
    ros::init(argc,argv,"general_strategy");
    Take_off p_task_off;
    Navigation p_task_navigation;
    Task_hover p_task_hover;
    Task_land p_task_land;

    //测试用导航点
    //测试用导航点1
    geometry_msgs::PoseStamped target_pose1;
    target_pose1.header.frame_id="map";
    target_pose1.pose.position.x=4;
    target_pose1.pose.orientation.w=1;
    //测试用导航点2
    geometry_msgs::PoseStamped target_pose2;
    target_pose2.header.frame_id="map";
    target_pose2.pose.position.x=4;
    target_pose2.pose.position.y=4;
    target_pose2.pose.orientation.w=1;
    //测试用导航点3
    geometry_msgs::PoseStamped target_pose3;
    target_pose3.header.frame_id="map";
    target_pose3.pose.position.y=4;
    target_pose3.pose.orientation.w=1;
    //测试用导航点4
    geometry_msgs::PoseStamped target_pose4;
    target_pose4.header.frame_id="map";
    target_pose4.pose.orientation.w=1;
    //占位用空导航点
    geometry_msgs::PoseStamped blank_target_pose;
    //导航点数组
    std::vector<geometry_msgs::PoseStamped> targets_pose{target_pose1,target_pose2,
                                                         target_pose3,target_pose4,
                                                         blank_target_pose};
    //导航点总数
    int target_count=2;

    while (ros::ok()){
        //当前导航点
        geometry_msgs::PoseStamped current_nav_target=targets_pose.at(p_task_navigation.get_current_target_index());
        geometry_msgs::PoseStamped current_hover_target=targets_pose.at(p_task_hover.get_current_target_index());
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
            //设置导航点
            p_task_navigation.set_move_base_goal(current_nav_target);
            //ROS_INFO("navi target index: %d",p_task_navigation.get_current_target_index());
            p_task_navigation.run();
            if(p_task_navigation.get_task_state()==TASK_FINISH)
            {
                ROS_INFO("nav_goal reached!");
                Drone_state::get_instance()->task_id=HOVER;
            }
        }
/***************HOVER**************/
        if(Drone_state::get_instance()->task_id==HOVER)
        {
            p_task_hover.set_hover_point(current_hover_target);
            //ROS_INFO("hover target index: %d",p_task_hover.get_current_target_index());
            p_task_hover.run();
            if(p_task_hover.get_task_state()==TASK_FINISH)
            {
                ROS_INFO("see target!");
                if(p_task_hover.get_current_target_index()==target_count)
                {
                    Drone_state::get_instance()->task_id=LAND;
                } else
                {
                    Drone_state::get_instance()->task_id=NAVIGATION;
                }
            }
        }
/**************LAND***************/
        if(Drone_state::get_instance()->task_id==LAND)
        {
            p_task_land.run();
            ROS_INFO("auto land...");
        }


        Drone_state::get_instance()->pub_to_ros();
        Drone_state::get_instance()->update();
    }
    return 0;
}
