#!/usr/bin/env python

import rospy
from std_msgs.msg import Int16MultiArray

# PI VARIABLES
Kp = 4
Ki = 0.8

# VARIABLES OF THE PI STATES
error_prev = 0
integral = 0

def sensor_callback(data):
    global error_prev, integral

    # SENSOR DATA OBTAINED
    sensor_data = data.data[:2]

    # PID COMPUTATION
    setpoint = [60, 60]  
    # setpoint is 60rpms but sensor_data measure the number of holes measures each 0.1s
    sensor_data=sensor_data*10*60/20

    error = setpoint - sensor_data 

    integral += error
    
    control_output = Kp * error + Ki * integral 

    # PUBLISH THE OUTPUTS in "pid" topic
    control_output_msg = Int16MultiArray()
    control_output_msg.data = [arr]
    pid_pub.publish(arr)

    # Keep the error each iteration
    error_prev = error

if name == 'main':
    rospy.init_node('pid_controller_node')
    rospy.Subscriber('sensor', Int16MultiArray, sensor_callback)
    pid_pub = rospy.Publisher('pid', Int16MultiArray, queue_size=10)
    rospy.spin()
