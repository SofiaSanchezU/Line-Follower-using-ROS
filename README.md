# Line-Follower-using-ROS

Code developed for "S. Sanchez, - Construction of a mobile robot that follows a line controlled by ROS ".
Master's graduate "Calabria University".
For any questions or suggestions write to sofysan.1993@gmail.com

# Sumary.
The project consists of the construction of line-following robot able to move automatically and follow a detected black line that is drawn on the surface. The measure of the sensors used to detect the line and the encoders which are used for building the control are read by an ESP board, this information is sent to ROS via socket communication by a publisher node created on ESP. Then, in ROS a subscriber-publisher node receives the information related to the encoders sensors and process it to obtain the needed control actions. As ESP is development board that cannot afford complex programs it was incorporated a second board which are going to receive the values before explained using a subscriber node on it and execute all the logic to move the robot. On the first ESP it was also create another subscriber that will receive a message to know when the robot has to start to move and stop.

# File description
The repository contains the main file
1. **catkin_ws_2**: this folder contains the workspace generated for this project
2. **ESP_PART**: this folder cointein the code for the ISP module for ROS1 and ROS2
3. **Report**: shows the complete implementation of the project 

# Example to run the experiment  
**"ROS Simulation"**
1. To execute the project follow the guidelines in the report. 
