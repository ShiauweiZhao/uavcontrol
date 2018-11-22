#include "uavctrl/ctrlpx4.h"

int main(int argc, char **argv)
{
    ros::init(argc, argv, "offb_node");
    CtrlPx4 ctrlpx4;
    ROS_INFO("offb init");
    ros::Rate rate(12);

    //check connected with fcu
    while (ros::ok() && !ctrlpx4.connectState())
    {
        ros::spinOnce();
        rate.sleep();
    }
    while (ros::ok())
    {
        if (ctrlpx4.checkMocapDataValid())
        {
            ctrlpx4.setPublishPose();
            break;
        }
        ros::spinOnce();
    }
    while (ros::ok())
    {
    }
}