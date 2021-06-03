#include "std_msgs/String.h"
#include <sstream>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <signal.h>
#include <sstream>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include "ros/ros.h"
#include <nav_msgs/Odometry.h>

#include <geometry_msgs/TransformStamped.h>
#include <geometry_msgs/PoseStamped.h>

#include <robot_navigation/new_goal.h>
#include <vector>
#define fifo "fifo"



int main(int argc, char **argv)
{
 
 
  ros::init(argc, argv, "pub");

  
  ros::NodeHandle n;

  ROS_INFO("node initialized");

  ros::Publisher chatter_pub = n.advertise<robot_navigation::new_goal>("new_goal", 1000);
  float coords[3];
  int fd=open(fifo,O_RDONLY);
  if(fd==-1){
        printf("errore apertura fifo in apertura\n");
        return EXIT_FAILURE;
      }
  int i;
  for(i=0;i<3;i++){
      if(read(fd,&coords[i],sizeof(float))==-1){
            printf("errore apertura fifo in lettura\n");
            return EXIT_FAILURE;
      }
  }
  ROS_INFO("pubblico coordinate");
  robot_navigation::new_goal goal;
  goal.x=coords[0];
  goal.y=coords[1];
  goal.theta=coords[2];
  close(fd);
  while(ros::ok){

    ROS_INFO("stampo la coordinata x ricevuta dal web client:/n");
    printf("coordinata dal pub :%f\n",coords[0]);
    printf("coordinata dal pub :%f\n",coords[1]);
    printf("coordinata dal pub :%f\n",coords[2]);

    
      
      chatter_pub.publish(goal);
      ROS_INFO("DONE!");
      ros::spinOnce();
      sleep(7);
  }



  return 0;
}