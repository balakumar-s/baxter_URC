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
int data_read=0;
int limb_chosen=0;// left=0,right=1
sensor_msgs::JointState joint_state;
using namespace std;
void jointCallback(sensor_msgs::JointState js_)
{	
	joint_state=js_;
	data_read++;
}
void writeData(sensor_msgs::JointState state_in)
{
	int start=0;
	int end=0;
	int siz_=state_in.position.size();
	if(limb_chosen==0)
	{
		start=2;
		end=8;
	}
	else if(limb_chosen==1)
	{
		start=9;
		end=16;
	}
	
	//ROS_INFO("size:%d",siz_);
	if(count_==0)
	{
		record_file<<7<<std::endl;
		for(int i=start;i<=end;i++)
		{
			record_file<<state_in.name[i]<<",";	
		}
		record_file<<std::endl;
		count_=1;
	}
	for(int j=start;j<=end;j++)
	{
		record_file<<state_in.position[j]<<",";
	}
	record_file<<std::endl;
	std::cout<<"position recorded"<<std::endl;
}
int main(int argc,char* argv[])
{
	std::string filename="temp.txt";
	if(argc<4)
	{
		cout<<"Usage is -l/-r -f filename"<<std::endl;
		exit(0);
	}
	else
	{
		if(strcmp(argv[1],"-l")==0)
		{
			limb_chosen=0;
		}
		if(strcmp(argv[1],"-r")==0)
		{
			limb_chosen=1;
		}
		filename=argv[3];
	}
	ros::init(argc,argv,"recorder");
	ros::NodeHandle n;
	ros::Subscriber joint_sub=n.subscribe("/robot/joint_states",1,jointCallback);
	record_file.open(filename.c_str());
	char key_in='0';
	while(ros::ok())
	{
		if(data_read==1)
		{
			std::cout<<"Type c and press enter to record position"<<std::endl;
		}
		if(data_read>0)
		{
			std::cin>>key_in;	
		}
		if(key_in=='c'&& data_read>0)
		{	
			writeData(joint_state);
		}
		if(key_in=='e')
		{
			record_file.close();
			exit(0);
		}
		ros::spinOnce();
	}
	
	return(0);
}