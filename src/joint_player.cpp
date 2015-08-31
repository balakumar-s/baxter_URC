// Reads a text file containing joint positions and moves the robot to those positions
// Author: Balakumar Sundaralingam

#include <ros/ros.h>
#include <sensor_msgs/JointState.h>
#include <fstream>
#include <baxter_core_msgs/JointCommand.h>

using namespace std;
std::ifstream input_file;
int limb_chosen=0;//left=0,right=1.
int array_size=0;
ros::Publisher jCommand_pub;
baxter_core_msgs::JointCommand j_command;
int name_line=0;
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
				//cout<<j_command.names[i]<<endl;
			}
			name_line=1;
		}
		j_command.command.resize(array_size);
		for(int i=0;i<array_size;i++)
		{
			string temp;
			getline(input_file,temp,',');
			j_command.command[i]=atof(temp.c_str());
			//cout<<j_command.command[i]<<endl;
		}
		if(j_command.command[0]==0&& j_command.command[1]==0)
		{
			cout<<"end of file";
			exit(0);
		}
		else
		{
			jCommand_pub.publish(j_command);	
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
	if(limb_chosen==0)
	{
		jCommand_pub=n.advertise<baxter_core_msgs::JointCommand>("/robot/limb/left/joint_command",1);
	}
	else
	{
		jCommand_pub=n.advertise<baxter_core_msgs::JointCommand>("/robot/limb/right/joint_command",1);	
	}
	input_file.open(filename.c_str());
	string temp;
	input_file>>array_size;
	char key_in='0';
	cout<<"Press c to go to next position";
	while(ros::ok())
	{
		cin>>key_in;
		if(key_in=='c')
		{
			if(!input_file.eof())
			{
				readData();
			}
			else
			{
				cout<<"end of file";
				exit(0);
			}
		}
		else if(key_in=='e')
		{
			exit(0);
		}
	}
}