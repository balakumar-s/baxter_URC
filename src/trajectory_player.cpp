// Reads a text file containing joint trajectory way points and timestep and performs the trajectory
// Author: Balakumar Sundaralingam

#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <fstream>
#include <baxter_core_msgs/JointCommand.h>
#define MAX_ERR 0.02
using namespace std;
std::ifstream input_file;
int limb_chosen=0;//left=0,right=1.
int array_size=0;
ros::Publisher jCommand_pub;
baxter_core_msgs::JointCommand j_command;
int name_line=0;
int reached_state=0;
int timestep=5;
double current_time=0;
double old_time=0;
void jointCommand()
{
	current_time=ros::Time::now().toSec();
	if ((current_time-old_time)>timestep)
	{
		ROS_INFO("time step reached");
		old_time=current_time;
		reached_state=1;
	}
	else
	{
		reached_state=0;
	}
}
void readData()
{
	if(!input_file.eof())
	{
				
		j_command.command.resize(array_size);
		for(int i=0;i<array_size;i++)
		{
			string temp;
			getline(input_file,temp,',');
			temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());
			j_command.command[i]=atof(temp.c_str());
			//cout<<j_command.command[i]<<endl;
		}
		if(j_command.command[0]==0.00 && j_command.command[1]==0.00)
		{
			ROS_INFO("end of file");
			exit(0);
		}
		
	}
}

int main(int argc,char* argv[])
{
	std::string filename="temp.txt";
	if(argc<3)
	{
		cout<<"Usage is -f filename"<<std::endl;
		exit(0);
	}
	else
	{
		filename=argv[2];
	}
	
	ros::init(argc,argv,"Trajectory_player");
	ros::NodeHandle n;
	ros::Rate loop_rate(30);
	jCommand_pub=n.advertise<baxter_core_msgs::JointCommand>("/robot/limb/left/joint_command",1);
	

	input_file.open(filename.c_str());
	string temp;
	getline(input_file,temp,',');
	temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());
	array_size=atof(temp.c_str());
	getline(input_file,temp);
	temp.erase(std::remove(temp.begin(), temp.end(), '\n'), temp.end());
	timestep=atof(temp.c_str());
	cerr<<"time: "<<timestep<<" "<<endl;
	cout<<"Going through waypoints"<<endl;
	readData();
	old_time=ros::Time::now().toSec();
	// Initialize jointcommand values:left_e0,left_e1,left_s0,left_s1,left_w0,left_w1,left_w2
	j_command.mode=1;
	j_command.names.resize(7);
	j_command.names[0]="left_e0";
	j_command.names[1]="left_e1";
	j_command.names[2]="left_s0";
	j_command.names[3]="left_s1";
	j_command.names[4]="left_w0";
	j_command.names[5]="left_w1";
	j_command.names[6]="left_w2";
	while(ros::ok())
	{
		if(!input_file.eof() && reached_state==1)
		{
			ROS_INFO("reading next position");
			readData();
		}
		jointCommand();
		jCommand_pub.publish(j_command);	
		ros::spinOnce();
		loop_rate.sleep();
	}
	
}
