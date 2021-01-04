#!/usr/bin/env python
import rospy
#import rospkg
import serial
#import re

#from serial import *
from gazebo_msgs.msg import ModelState 
from gazebo_msgs.srv import SetModelState
from geometry_msgs.msg import Twist
from geometry_msgs.msg import Pose



def open():

	rospy.init_node('doorlisten', anonymous=True)
	serial_port = serial.Serial('/dev/ttyACM0', 9600)
	targetTopic = '/gazebo/set_model_state'

	model = ModelState()
	model.model_name = 'unit_cylinder'

	while True:
		line = serial_port.readline()
		
		vdoor = line.split()
		if not (len(vdoor)==0):
			print('val open :', vdoor[0])	
		
		vel = Twist()
		pose = Pose()

		publisher = rospy.Publisher(targetTopic, ModelState, queue_size=10)

		if (vdoor[0] == "O"):
			rospy.loginfo('inside')
			
			pose.position.x= 0.0
	    		pose.position.y= 0.0
	    		pose.position.z= 0.0
	     		pose.orientation.x = 0.0
	   		pose.orientation.y = 0.0
			pose.orientation.z = 5.0
			pose.orientation.w = 5.0

			vel.linear.x= 0.0
	    		vel.linear.y= 0.0
	    		vel.linear.z= 0.0
	     		vel.angular.x = 0.0
	   		vel.angular.y = 0.0
			vel.angular.z = 0.0
	
		else:
			rospy.loginfo('inside')

			pose.position.x= 0.0
	    		pose.position.y= 0.0
	    		pose.position.z= 0.6
	     		pose.orientation.x = 0.0
	   		pose.orientation.y = 0.0
			pose.orientation.z = 0.0
			pose.orientation.w = 5.0

			vel.linear.x= 0.0
	    		vel.linear.y= 0.0
	    		vel.linear.z= 0.0
	     		vel.angular.x = 0.0
	   		vel.angular.y = 0.0
			vel.angular.z = 0.0

			print('val close :', vdoor[0])

		model.pose = pose
		model.twist = vel
		publisher.publish(model)

if __name__ == '__main__':
 	try:
      		open()
 	except rospy.ROSInterruptException:
     		pass


