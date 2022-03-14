
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "../include/pelco_d.hpp"
#include <serial/serial.h>
#include "std_msgs/Int8.h"
#include "std_msgs/Int16.h"
#include "gimbal_comm/GimbalState.h"

std_msgs::Int8 get_zoom;
std_msgs::Int16 get_angular_p;
std_msgs::Int16 get_angular_y;
gimbal_comm::GimbalState gimbalstate;
PELCO_D gimbal;
void get_zoom_vel(const std_msgs::Int8 &msg)
{
    get_zoom = msg;
    if (get_angular_p.data > 8)
    {
        get_angular_p.data = 8;
    }
    else if (get_angular_p.data < -8)
    {
        get_angular_p.data = -8;
    }
}

void get_angular_vel_p(const std_msgs::Int16 &msg)
{
    get_angular_p = msg;
    if (get_angular_p.data > 256)
    {
        get_angular_p.data = 256;
    }
    else if (get_angular_p.data < -256)
    {
        get_angular_p.data = -256;
    }
}

void get_angular_vel_y(const std_msgs::Int16 &msg)
{
    get_angular_y = msg;
    if (get_angular_y.data > 256)
    {
        get_angular_y.data = 256;
    }
    else if (get_angular_y.data < -256)
    {
        get_angular_y.data = -256;
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "serial_port");

    //-------------------------------------------------------------------------------
    ros::init(argc, argv, "gimbal_comm");
    ros::NodeHandle n;

    ros::Publisher state_pub = n.advertise<gimbal_comm::GimbalState>("/gimbal/state", 1000);

    ros::Subscriber zoom = n.subscribe("/gimbal/setpoint/zoom_vel", 1000, get_zoom_vel);
    ros::Subscriber angular_p = n.subscribe("gimbal/setpoint/angular_vel/p", 1000, get_angular_vel_p);
    ros::Subscriber angular_y = n.subscribe("gimbal/setpoint/angular_vel/y", 1000, get_angular_vel_y);

    serial::Serial sp;
    serial::Timeout to = serial::Timeout::simpleTimeout(200);

    sp.setPort("/dev/ttyUSB0");
    sp.setBaudrate(115200);
    sp.setTimeout(to);

    try
    {
        sp.open();
    }
    catch (serial::IOException &e)
    {
        ROS_ERROR_STREAM("Serial error!!!");
        return -1;
    }
    if (!sp.isOpen())
    {
        ROS_ERROR_STREAM("Serial error!!!");
        return -1;
    }
    ros::Rate loop_rate(5);
get_zoom.data=0;
    // uint8_t buffer[512];
    while (ros::ok())
    {

        if (get_angular_p.data >= 0)
        {
            gimbal.Up(get_angular_p.data);
            sp.write(gimbal.tx_buf, 7);
        }
        else
        {
            gimbal.Down(-get_angular_p.data);
            sp.write(gimbal.tx_buf, 7);
        }

        if (get_angular_y.data >= 0)
        {
            gimbal.Left(get_angular_y.data);
            sp.write(gimbal.tx_buf, 7);
        }
        else
        {
            gimbal.Right(-get_angular_y.data);
            sp.write(gimbal.tx_buf, 7);
        }

	if(get_zoom.data ==0)
	{
	    gimbal.ZoomStop();
	    sp.write(gimbal.tx_buf, 7);
	}
        else if (get_zoom.data > 0)
        {
            gimbal.ZoomOut(get_zoom.data);
            sp.write(gimbal.tx_buf, 7);
        }
        else
        {
            gimbal.ZoomIn(-get_zoom.data);
            sp.write(gimbal.tx_buf, 7);
        }

        if (sp.available() > 7)
        {
            sp.read(gimbal.rx_buf, 8);
            gimbal.Parse();
            gimbalstate.online = gimbal.online;
            gimbalstate.Zoom_Ratio = gimbal.Zoom_Ratio;
            gimbalstate.Pitch_deg = gimbal.Pitch_deg;
            gimbalstate.Yaw_deg = gimbal.Yaw_deg;
            state_pub.publish(gimbalstate);
        }

        else
        {
            gimbalstate.online = 0;
            gimbalstate.Zoom_Ratio = gimbal.Zoom_Ratio;
            gimbalstate.Pitch_deg = gimbal.Pitch_deg;
            gimbalstate.Yaw_deg = gimbal.Yaw_deg;
            state_pub.publish(gimbalstate);
        }

        ros::spinOnce();
        loop_rate.sleep();
    }
    sp.close();

    return 0;
}
// %EndTag(FULLTEXT)%
