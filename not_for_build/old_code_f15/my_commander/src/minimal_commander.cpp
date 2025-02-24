//
//  minimal_commander.cpp
//  my_commander
//
//  Created by 田适霈 on 9/2/15.
//  Copyright (c) 2015 Shipei. All rights reserved.
//


#include <cmath>
#include <ros/ros.h>
#include <std_msgs/Float64.h>
#include <my_commander/interface_cmd.h>

#define CONTROL_SPEED   1000    //slow down the fucntion if you want, this will save your CPU resource and reduce jetter of plot, but reduce the limitation of max frequency

double amp = 0.0;  //set a default value to amplitude
double feq = 0.0;  //set a default value to frequency
bool start = false; //work or not


bool updateParameters(my_commander::interface_cmdRequest& request, my_commander::interface_cmdResponse& response)
{
    ROS_INFO("Parameters changed");
    amp = request.amp_cmd;
    feq = request.feq_cmd;
    start = request.start;
    response.success = true;
    if (start) {
        ROS_INFO("Commander start to run");
    } else {
        ROS_INFO("Commander stopped");
    }
    return true;
}


int main(int argc, char **argv) {
    std_msgs::Float64 g_vel_cmd;//message buffer to send out
    ros::init(argc, argv, "minimal_commander"); //name this node
    // when this compiled code is run, ROS will recognize it as a node called "minimal_commander"
    ros::NodeHandle nh; // node handle
    ros::ServiceServer service = nh.advertiseService("command_msgs", updateParameters);
    ros::Rate naptime(CONTROL_SPEED); //make the output value change by 1ms
    ros::Publisher my_publisher_object = nh.advertise<std_msgs::Float64>("vel_cmd", 1); //publish the output value to the topic "vel_cmd"
    ROS_INFO("Waiting for command");
    while (ros::ok()) { //keep runing this
        for (double count = 0; count < (CONTROL_SPEED / feq); count++) {    //calculate the times(number of ms) of value changes during a single wave
            ros::spinOnce();
            naptime.sleep();    //a single time span, just suppose your CPU is power enough so that we could ignore the time of doing following works
            if (start) {
                g_vel_cmd.data = amp * (std::sin(((M_PI * 2) / (CONTROL_SPEED / feq)) * count));   //calculate the current angle (rad), π×2 is one circle, do sin, and amplify it
                my_publisher_object.publish(g_vel_cmd); //ready to send
//                ROS_INFO("Velocity set to: %f", g_vel_cmd.data); //print value to console
            }
        }
    }
    return 0; // should never get here, unless roscore dies
}
