#ifndef ROSER_H
#define ROSER_H
#include <string>
#include <ros/ros.h>
#include <ros/package.h>
#include <iostream>
#include <TCP/tcper.h>
#include <TOOLS/struct.h>
#include "realtime_tcp.h"
#include <cmath>
#include "plusgo_msgs/ByWireVehicleState.h"
#include "plusgo_msgs/basic_info.h"
#include "plusgo_msgs/SystemInfoReport.h"
#include<geometry_msgs/PoseStamped.h>

void call_back(const realtime_msgs::realtime_tcp &msg);
void pnc_Callback(const plusgo_msgs::SystemInfoReport &msg);
void slam_Callback(const geometry_msgs::PoseStamped &msg);
void vehicle_state_Callback(const plusgo_msgs::ByWireVehicleState &msg);
class Roser
{
public:
    Roser(int argc, char **argv);
    ~Roser();
    void initialization(int argc, char **argv);
    void transmit(int argc, char **argv);
    void publish_msg();
    void subscribe_msg();
    static realtime_msgs::realtime_tcp realtime_msg;
};








#endif
