ROS packages for Arduino serial interface

#Requirements
##Software
1. Ubuntu 10.4 LTS (can be installed on virtual machine)
2. ROS Hydro Medusa (Desktop-Full, instructions here: http://wiki.ros.org/hydro/Installation/Ubuntu)


##Hardware Requirements
1. PC
2. 4WD base
  1. Arduino compatible board (Leonardo)
  2. 2x X-bee (one connected to the PC and the other to the Arduino board)
  2. Two channel motor controller
  3. 2x Motor encoders (both sides)
  4. IR sensor
  5. Pan/tilt servo kit (2x 180 degree servo motors)


#Installing and using ROS
	$ Instructions here: http://wiki.ros.org/hydro/Installation/Ubuntu
	$ echo "source /opt/ros/hydro/setup.bash" >> ~/.bashrc


##Installing Arpi stack on Ubuntu
	$ mkdir -p ~/ros_workspace/ros_arpi
	$ mkdir -p ~/ros_workspace/ros_arpi/src
	$ cd ~/ros_workspace/ros_arpi/src
	$ git clone https://github.com/karlblum/arpi.git
	$ cd ~/ros_workspace/ros_arpi/
	$ catkin_make
	$ echo "source ~/ros_workspace/ros_arpi/devel/setup.bash" >> ~/.bashrc

Make sure all node files have executable permission
	$ chmod +x node.py
  
Running the whole stack:

	$ roslaunch ros_arduino_python arduino.launch
  
##Creating an empty ROS workspace
	$ mkdir -p ~/ros_workspace/ros_project/
	$ cd ~/ros_workspace/ros_project
	$ catkin_init_workspace

	
##Todo
	ROS ei saa vahest päringutele vastuseid kätte ja hangub. Arduino FW loop jookseb edasi aga ROS ei anna ka timeouti millegipärast.