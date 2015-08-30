//Subcribes to joint states and stores them in a file
//Author: Balakumar Sundaralingam

#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <sstream.h>
#include <stdlib.h>
#include <baxter_core_msgs/JointCommand.h>
std::ofstream record_file;
int count_=0;
void jointCallback(sensor_msgs::JointState js_)
{	
	int siz_=js_.string.size();
	
}

int main(int argc,char** argv)
{
	ros::init(argc,argv,"recorder");
	ros::NodeHandle n;
	ros::Subscriber joint_sub=n.subsribe("/robot/joint_states",1,jointCallback);
	record_file.open("data.txt");

	while(ros::ok())
	{
	ros::spinOnce();
	}
	return(0);
}