/*
 * tag_listener_node.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: Fredrik Macintosh
 */


#include "ros/ros.h"
#include "visualization_msgs/Marker.h"
#include "tf/transform_listener.h"
#include <iostream>
#include <stdlib.h>
#include <vector>
#include <time.h>
// This program listens to the transforms of tags sent by the ar_track_alvin node. It's also blatantly copied from the ROS





// Function that creates the marker.

visualization_msgs::Marker makeMarker(const tf::StampedTransform tagTransform, std::string name,
		int id, int red, int green, int blue, int alpha)
{

	visualization_msgs::Marker marker;

	marker.header.frame_id = "map";
	marker.header.stamp = ros::Time();
	marker.ns = name;
	marker.id = id;
	marker.type = visualization_msgs::Marker::CUBE;
	marker.action = visualization_msgs::Marker::ADD;
	marker.pose.position.x = tagTransform.getOrigin().x();
	marker.pose.position.y = tagTransform.getOrigin().y();
	marker.pose.position.z = tagTransform.getOrigin().z();
	marker.pose.orientation.x = tagTransform.getRotation().x();
	marker.pose.orientation.y = tagTransform.getRotation().y();
	marker.pose.orientation.z = tagTransform.getRotation().z();
	marker.pose.orientation.w = tagTransform.getRotation().w();
	marker.scale.x = 0.5;
	marker.scale.y = 0.5;
	marker.scale.z = 0.5;
	marker.color.a = alpha; // alpha = opacity
	marker.color.r = red;
	marker.color.g = green;
	marker.color.b = blue;

	return marker;

}// end of makeMarker();




int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line.
   * For programmatic remappings you can use a different version of init() which takes
   * remappings directly, but for most command-line programs, passing argc and argv is
   * the easiest way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */

  ros::init(argc, argv,"tag_listener");


  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle subNodehandle;
  ros::NodeHandle pubNodehandle;
  ros::NodeHandle tfHandle;

  /**
   * The subscribe() call is how you tell ROS that you want to receive messages
   * on a given topic.  This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing.  Messages are passed to a callback function, here
   * called chatterCallback.  subscribe() returns a Subscriber object that you
   * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
   * object go out of scope, this callback will automatically be unsubscribed from
   * this topic.
   *
   * The second parameter to the subscribe() function is the size of the message
   * queue.  If messages are arriving faster than they are being processed, this
   * is the number of messages that will be buffered up before beginning to throw
   * away the oldest ones.
   */
  tf::TransformListener tagListener;

  ros::Publisher posePublisher = pubNodehandle.advertise<visualization_msgs::Marker>("tag_pose", 1000);



  ros::Rate loop_rate(10);

  while(ros::ok())
  {
	  tf::StampedTransform transform_marker_0;
	  printf("Next line of code waits for transform between /map and /ar_marker_0 \n"
			  "and might block the code for five seconds");
	  float timer = time(NULL);
	  tagListener.waitForTransform("/map", "/ar_marker_0", ros::Time(0), ros::Duration(5.0));
	  timer = time(NULL) - timer;
	  printf("Code was blocked for ");
	  printf("%4.2f", timer);
	  printf("seconds");

	  try {
		  tagListener.lookupTransform("/map", "/ar_marker_0", ros::Time(0), transform_marker_0);
	  }
	  catch (tf::TransformException ex){
		  ROS_ERROR("%s", ex.what());
		  ros::Duration(1.0).sleep();
	  }
	  visualization_msgs::Marker marker_0 = makeMarker(transform_marker_0, "marker_0", 0, 1, 0, 0, 1);
	  posePublisher.publish(marker_0);



  /**
   * ros::spinOnce() will pump ONE callback (hence the while).  With this version, all
   * callbacks will be called from within this thread (the main one).
   */
  	  ros::spinOnce();
  	  loop_rate.sleep();

  }

  return 0;
}

