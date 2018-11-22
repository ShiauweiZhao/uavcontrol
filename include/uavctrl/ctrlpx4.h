#ifndef _CTRLPX4_H_
#define _CTRLPX4_H_
#include <ros/ros.h>
#include <std_msgs/UInt8.h>
#include <mavros_msgs/State.h>
#include <geometry_msgs/PoseStamped.h>
#include <mavros_msgs/SetMode.h>
#include <mavros_msgs/CommandBool.h>

class CtrlPx4
{
public:
  CtrlPx4();
  ~CtrlPx4();
  void setPublishPose();
  bool checkMocapDataValid();
  bool connectState();
  bool armingState();
  std::string stateMode();
  std::string setStateMode();
  void publishSetPoint();
  void offbModeArmed();

private:
  mavros_msgs::State currentState;
  geometry_msgs::PoseStamped mocapPose;
  std_msgs::UInt8 uavCommand;
  geometry_msgs::PoseStamped localPosPubMsg;
  mavros_msgs::CommandBool armCmd;
  mavros_msgs::SetMode offbSetMode;

private:
  void stateCallback(const mavros_msgs::State);
  void mocapCAllback(const geometry_msgs::PoseStamped);
  void uavCommandCallback(const std_msgs::UInt8);

private:
  ros::NodeHandle nh;

  ros::Subscriber stateSubscriber;
  ros::Subscriber uavCommandSubscriber;
  ros::Subscriber mocapPoseSubscriber;
  ros::Publisher localPositionPublish;

public:
  ros::ServiceClient armingClient;
  ros::ServiceClient setModeClient;
};

#endif
