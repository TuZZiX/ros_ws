//path_client:
// illustrates how to send a request to the path_service service

#include <ros/ros.h>
#include <example_ros_service/PathSrv.h> // this message type is defined in the current package
#include <iostream>
#include <string>
#include <nav_msgs/Path.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>
#include <nav_msgs/Odometry.h>
#include <math.h>
using namespace std;
geometry_msgs::Pose g_current_pose;

geometry_msgs::Quaternion convertPlanarPhi2Quaternion(double phi) {
    geometry_msgs::Quaternion quaternion;
    quaternion.x = 0.0;
    quaternion.y = 0.0;
    quaternion.z = sin(phi / 2.0);
    quaternion.w = cos(phi / 2.0);
    return quaternion;
}
// for orientation feedback control
void odemCallback(const nav_msgs::Odometry& odom_msg) {
    double quat[4];
    std::vector<double> euler_r;
    g_current_pose = odom_msg.pose.pose;
}
int main(int argc, char **argv) {
    ros::init(argc, argv, "solver_client");
    ros::NodeHandle n;
    ros::Subscriber odem_subscriber = n.subscribe("/robot0/odom", 1, odemCallback);
    ros::ServiceClient client = n.serviceClient<example_ros_service::PathSrv>("path_service");
    geometry_msgs::Quaternion quat;
    
    while (!client.exists()) {
      ROS_INFO("waiting for service...");
      ros::Duration(1.0).sleep();
        ros::spinOnce();
    }
    ROS_INFO("connected client to service");
    example_ros_service::PathSrv path_srv;
    for (int i = 0; i < 10; ++i) {
        ros::spinOnce();
        ros::Duration(0.05).sleep();
    }
    ROS_INFO("Start position x: %f, y: %f, z:%f", g_current_pose.position.x, g_current_pose.position.y, g_current_pose.position.z);
    ROS_INFO("Start orientation x: %f, y: %f, z:%f, w:%f", g_current_pose.orientation.x, g_current_pose.orientation.y, g_current_pose.orientation.z, g_current_pose.orientation.w);
    //create some path points...this should be done by some intelligent algorithm, but we'll hard-code it here
    geometry_msgs::PoseStamped pose_stamped;
    geometry_msgs::Pose pose;
    pose = g_current_pose;
    pose.position.x += 3.0; // say desired x-coord is 1
    pose_stamped.pose = pose;
    path_srv.request.nav_path.poses.push_back(pose_stamped);
    
    // some more poses...
    quat = convertPlanarPhi2Quaternion(M_PI/2); // get a quaterion corresponding to this heading
    pose_stamped.pose.orientation = quat;   
    pose_stamped.pose.position.y+=3.0; // say desired y-coord is 1.0
    path_srv.request.nav_path.poses.push_back(pose_stamped);
    
    quat = convertPlanarPhi2Quaternion(0);
    pose_stamped.pose.orientation = quat;
    pose_stamped.pose.position.x+=4.5;
    //desired position is not updated...just the desired heading  
    path_srv.request.nav_path.poses.push_back(pose_stamped);

    quat = convertPlanarPhi2Quaternion(M_PI/2);
    pose_stamped.pose.orientation = quat;
    pose_stamped.pose.position.y+=2.2;
    //desired position is not updated...just the desired heading
    path_srv.request.nav_path.poses.push_back(pose_stamped);

    quat = convertPlanarPhi2Quaternion(M_PI);
    pose_stamped.pose.orientation = quat;
    pose_stamped.pose.position.x-=4.5;
    //desired position is not updated...just the desired heading
    path_srv.request.nav_path.poses.push_back(pose_stamped);

    quat = convertPlanarPhi2Quaternion(M_PI/2);
    pose_stamped.pose.orientation = quat;
    pose_stamped.pose.position.y+=7.0;
    //desired position is not updated...just the desired heading
    path_srv.request.nav_path.poses.push_back(pose_stamped);

    quat = convertPlanarPhi2Quaternion(M_PI);
    pose_stamped.pose.orientation = quat;
    pose_stamped.pose.position.x-=3.0;
    //desired position is not updated...just the desired heading
    path_srv.request.nav_path.poses.push_back(pose_stamped);
    client.call(path_srv);

    return 0;
}
