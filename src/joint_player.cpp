// Reads a text file containing joint positions and moves the robot to those positions
// Author: Balakumar Sundaralingam

#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <fstream>
#include <baxter_core_msgs/JointCommand.h>
#define MAX_ERR 0.01
using namespace std;
std::ifstream input_file;
int limb_chosen=0;//left=0,right=1.
int array_size=0;
ros::Publisher jCommand_pub;
baxter_core_msgs::JointCommand j_command;
int name_line=0;
int reached_state=0;

void jointCallback(sensor_msgs::JointState js_)
{
	int start=2;
	if(limb_chosen==0)
	{
		start=2;
	}
	else
	{
		start=9;
	}
	int reached_count=0;
	for(int i=0;i<array_size;i++)
	{
		//cout<<fabs(js_.position[start+i]-j_command.command[i]);
		if(fabs(js_.position[start+i]-j_command.command[i])<MAX_ERR)
		{
			reached_count++;
		}
	}
	if (reached_count==array_size)
	{
		reached_state=1;
	}
}
void readData()
{
	if(!input_file.eof())
	{
		if(name_line==0)
		{
			j_command.mode=1;
			j_command.names.resize(array_size);
			for(int i=0;i<array_size;i++)
			{
				getline(input_file,j_command.names[i],',');
				j_command.names[i].erase(std::remove(j_command.names[i].begin(), j_command.names[i].end(), '\n'), j_command.names[i].end());
				//cout<<j_command.names[i]<<endl;
			}
			name_line=1;
		}
		j_command.command.resize(array_size);
		for(int i=0;i<array_size;i++)
		{
			string temp;
			getline(input_file,temp,',');
			temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());
			j_command.command[i]=atof(temp.c_str());
			//cout<<j_command.command[i]<<endl;
		}
		if(j_command.command[0]==0&& j_command.command[1]==0)
		{
			ROS_INFO("end of file");
			exit(0);
		}
		
	}
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
	
	ros::init(argc,argv,"joint_player");
	ros::NodeHandle n;
	ros::Rate loop_rate(30);
	if(limb_chosen==0)
	{
		jCommand_pub=n.advertise<baxter_core_msgs::JointCommand>("/robot/limb/left/joint_command",1);
	}
	else
	{
		jCommand_pub=n.advertise<baxter_core_msgs::JointCommand>("/robot/limb/right/joint_command",1);	
	}
	ros::Subscriber joint_sub=n.subscribe("/robot/joint_states",1,jointCallback);

	input_file.open(filename.c_str());
	string temp;
	input_file>>array_size;
	char key_in='0';
	cout<<"Going through positions"<<endl;
	readData();

	while(ros::ok())
	{
		if(!input_file.eof()&&reached_state==1)
		{
			ROS_INFO("reading next position");
			reached_state=0;
			readData();

		}
		if(name_line=1)
		{
		//	cout<<"publishing";
			jCommand_pub.publish(j_command);	
		}
		ros::spinOnce();
		loop_rate.sleep();
	}
}