//Subcribes to joint states and stores them in a file
//Author: Balakumar Sundaralingam

#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <baxter_core_msgs/JointCommand.h>
std::ofstream record_file;
int count_=0;
sensor_msgs::JointState joint_state;
using namespace std;
void jointCallback(sensor_msgs::JointState js_)
{	
	joint_state=js_;
}
void writeData(sensor_msgs::JointState state_in)
{
	int siz_=sizeof(state_in.name)/sizeof(state_in.name[0]);
	siz_--;
	cout<<sizeof(state_in.name)/sizeof(state_in.name[0]);
	ROS_INFO("size:%d",siz_);
	if(count_==0)
	{
		record_file<<siz_<<std::endl;
		for(int i=0;i<siz_;i++)
		{
			record_file<<state_in.name[i]<<",";	
		}
		record_file<<std::endl;
		count_=1;
	}
	for(int j=0;j<siz_;j++)
	{
		record_file<<state_in.position[j]<<",";
	}
	record_file<<std::endl;
	std::cout<<"position recorded"<<std::endl;
}
int main(int argc,char** argv)
{
	ros::init(argc,argv,"recorder");
	ros::NodeHandle n;
	ros::Subscriber joint_sub=n.subscribe("/robot/joint_states",1,jointCallback);
	record_file.open("data.txt");
	std::cout<<"Type c and press enter to record position"<<std::endl;
	char key_in='0';
	while(ros::ok())
	{
		std::cin>>key_in;
		if(key_in=='c')
		{	

			writeData(joint_state);
		}
		ros::spinOnce();
	}
	record_file.close();
	return(0);
}