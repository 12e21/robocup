
#include "Utils.h"

double Utils::calculate_distance(geometry_msgs::PoseStamped point1, geometry_msgs::PoseStamped point2) {
    double distance= sqrt(pow((point1.pose.position.x-point2.pose.position.x),2)+pow((point1.pose.position.y-point2.pose.position.y),2));
    return distance;
}
