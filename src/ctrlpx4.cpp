#include "uavctrl/ctrlpx4.h"

void CtrlPx4::stateCallback(const mavros_msgs::State msg)
{
    currentState = msg;
}

void CtrlPx4::uavCommandCallback(const std_msgs::UInt8 msg)
{
    uavCommand = msg;
    if (uavCommand.data = 0)
        ROS_INFO("command 0");
    if (uavCommand.data = 1)
        ROS_INFO("command 1");
    if (uavCommand.data = 2)
        ROS_INFO("command 2");
    if (uavCommand.data = 3)
        ROS_INFO("command 3");
}

void CtrlPx4::mocapCAllback(const geometry_msgs::PoseStamped msg)
{
    mocapPose = msg;
}

bool CtrlPx4::checkMocapDataValid()
{
    if (mocapPose.pose.position.z != -100)
        return true;
    else
        return false;
}
void CtrlPx4::setPublishPose()
{
    localPosPubMsg = mocapPose;
    ROS_INFO("localpos z %f", localPosPubMsg.pose.position.z);
}
CtrlPx4::CtrlPx4()
{
    mocapPose.pose.position.z = -100;
    stateSubscriber = nh.subscribe<mavros_msgs::State>("mavros/state", 10, &CtrlPx4::stateCallback, this);
    uavCommandSubscriber = nh.subscribe<std_msgs::UInt8>("mul/command", 10, &CtrlPx4::uavCommandCallback, this);
    mocapPoseSubscriber = nh.subscribe<geometry_msgs::PoseStamped>("mavros/mocap/pose", 10, &CtrlPx4::mocapCAllback, this);
    localPositionPublish = nh.advertise<geometry_msgs::PoseStamped>("mavros/setpoint_position/local", 20);

    setModeClient = nh.serviceClient<mavros_msgs::SetMode>("mavros/set_mode");
    armingClient = nh.serviceClient<mavros_msgs::CommandBool>("mavros/cmd/arming");
    ROS_INFO("ctrlpx4");
}
CtrlPx4::~CtrlPx4()
{
}
bool CtrlPx4::connectState()
{
    return currentState.connected;
}
std::string CtrlPx4::stateMode()
{
    return currentState.mode;
}
std::string CtrlPx4::setStateMode()
{
    currentState.mode = "OFFBOARD";
}

bool CtrlPx4::armingState()
{
    return currentState.armed;
}

void CtrlPx4::publishSetPoint()
{
    localPosPubMsg = mocapPose;
    localPosPubMsg.pose.orientation.w = 1;
    localPosPubMsg.pose.orientation.x = 0;
    localPosPubMsg.pose.orientation.y = 0;
    localPosPubMsg.pose.orientation.z = 0;
    CtrlPx4::localPositionPublish.publish(localPosPubMsg);
}
void CtrlPx4::offbModeArmed()
{
    offbSetMode.request.custom_mode = "OFFBOARD";
    armCmd.request.value = true;
    ros::Time last_request = ros::Time::now();

    if (currentState.mode != "OFFBOARD" && (ros::Time::now() - last_request > ros::Duration(5.0)))
    {
        if (setModeClient.call(offbSetMode) &&
            offbSetMode.response.mode_sent)
        {
            ROS_INFO("Offboard enabled");
        }
        last_request = ros::Time::now();
    }
    else
    {
        if (!currentState.armed &&
            (ros::Time::now() - last_request > ros::Duration(5.0)))
        {
            if (armingClient.call(armCmd) &&
                armCmd.response.success)
            {
                ROS_INFO("Vehicle armed");
            }
            last_request = ros::Time::now();
        }
    }
}