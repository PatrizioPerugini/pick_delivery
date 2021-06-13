#include "ros/ros.h"
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/LaserScan.h>
#include <tf2_msgs/TFMessage.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf2_ros/transform_listener.h>

#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include <iostream>
#include <fstream>
#include <geometry_msgs/PoseStamped.h>
#include <tf/tf.h>
#include <robot_navigation/new_goal.h>
#include <vector>
#include <queue>



//     std_msgs/Header header
//   uint32 seq
//   time stamp
//   string frame_id
// geometry_msgs/Pose pose
//   geometry_msgs/Point position
//     float64 x
//     float64 y
//     float64 z
//   geometry_msgs/Quaternion orientation
//     float64 x
//     float64 y
//     float64 z
//     float64 w

using namespace std;


int T=1;
geometry_msgs::PoseStamped new_goal_message;
geometry_msgs::PoseStamped home;
tf2_ros::Buffer tfBuffer;

std::vector<float> target_position(2,0);
std::vector<float> old_position(2,0);
std::vector<float> current_position(2,0);



size_t n=10;
int message_published = 0;
int cruising=0;

//change here
queue<std::vector<float>> q;
std::vector<float> store_position(3,0);
int arrived=0;



// home.header.seq=n;
// home.header.stamp=ros::Time::now();
// home.header.frame_id="map"; //boh
// home.pose.position.x=50.84;
// home.pose.position.y=11.87;
// home.pose.position.z=0;
// home.pose.orientation.x=0;
// home.pose.orientation.y=0;
// home.pose.orientation.z=0;
// home.pose.orientation.w=0.02;
//     //salvo la posizione nella quale devo andare 
// target_position[0]=50.84;
// target_position[1]=11.87;





ros::Subscriber sub;

void setGoal_CallBack(const robot_navigation::new_goal& new_goal){
    //cambiato questo primo if
    //if(cruising==1){
        
        store_position[0]=new_goal.x;
        store_position[1]=new_goal.y;
        store_position[2]=new_goal.theta;
       
           
        q.push(store_position);
        printf("nuovo obiettivo messo in coda : %f \n",store_position[0]);
       // printf("nuova posizione storata in coordinata y : %f \n",store_position[1]);
                
      

   // }
    
    
    
    if(cruising!=1){ 
        if(q.empty()){ 

            //setto la nuova posizione nella quale il robot dovrà andare
            new_goal_message.header.seq=n;
            new_goal_message.header.stamp=ros::Time::now();
            new_goal_message.header.frame_id="map"; 
        
            new_goal_message.pose.position.x=new_goal.x;
            new_goal_message.pose.position.y=new_goal.y;
            new_goal_message.pose.position.z=0;
            
            new_goal_message.pose.orientation.x=0;
            new_goal_message.pose.orientation.y=0;
            new_goal_message.pose.orientation.z=0;
            new_goal_message.pose.orientation.w=new_goal.theta;
            //salvo la posizione nella quale devo andare 
            target_position[0]=new_goal_message.pose.position.x;
            target_position[1]=new_goal_message.pose.position.y;

            ROS_INFO("nuovo target ricevuto, inzio navigazione");

            cruising=1;
            arrived=0;
            message_published=1;
        }

        else{
            vector<float> to_send=q.front();
            float xs=to_send[0];
            float ys=to_send[1];
            float ts=to_send[2];
            printf("coordinata x letta dalla codaè %f \n", xs );
            printf("coordinata y letta dalla codaè %f \n" , ys );
           
           
            new_goal_message.header.seq=n;
            new_goal_message.header.stamp=ros::Time::now();
            new_goal_message.header.frame_id="map"; 
        
            new_goal_message.pose.position.x=xs;
            new_goal_message.pose.position.y=ys;
            new_goal_message.pose.position.z=0;
            
            new_goal_message.pose.orientation.x=0;
            new_goal_message.pose.orientation.y=0;
            new_goal_message.pose.orientation.z=0;
            new_goal_message.pose.orientation.w=ts;
            //salvo la posizione nella quale devo andare 
            target_position[0]=new_goal_message.pose.position.x;
            target_position[1]=new_goal_message.pose.position.y;

            ROS_INFO("Avvio navigazione verso coordinata in testa alla coda");

            cruising=1;
            arrived=0;
            message_published=1;
            
            
            q.pop();
           //verificare qusta cosa per aumentare velocità
            int c=1;
            while(c==1){
                if(q.empty()){
                    c=0;
                }
                else if(q.front()[0]!=xs){
                    c=0;
                }
                else{
                    printf("elimino duplicati \n");
                    q.pop();
                }
            }

        }
    }

}


void position_CallBack(const tf2_msgs::TFMessage& tf){
    int transform_ok;
    
    //verifico se esiste la trasformata 
    transform_ok=tfBuffer.canTransform("map","base_link",
                                        ros::Time(0));
    if(transform_ok!=0){
        //prendo trasformta posa corrente
        geometry_msgs::TransformStamped transformStamped;
        transformStamped=tfBuffer.lookupTransform("map","base_link",
                                        ros::Time(0));
    //salvo posa corrente
    current_position[0]=transformStamped.transform.translation.x;
    current_position[1]=transformStamped.transform.translation.y;
    }
}

void check1_callBack(const ros::TimerEvent& event){
    if(cruising!=0){
        //fa un check se si sta muovendo
        float distance;
        distance=sqrt(pow(current_position[0]-old_position[0],2)+pow(current_position[1]-old_position[1],2));
        if(distance < 0.5){
            ROS_INFO("I'm stuck");
            
        }
        if(sqrt(pow(current_position[0]-target_position[0],2)+pow(current_position[1]-target_position[1],2))<1.5){
            ROS_INFO("robot arrivato");
           sleep(3);

            cruising=0;
        }
    }
}

void check2_callBack(const ros::TimerEvent& event){
    //check per vedere se è passato troppo tempo (in realta potrebbe essere omesso per la nostra applicazione)
    if(cruising!=0){
        float distance=sqrt(pow(current_position[0]-target_position[0],2)+pow(current_position[1]-target_position[1],2));
        if(distance > 0.5){
            ROS_INFO("robot in arrivo");
           
        }
    }  
}



int main(int argc,char **argv){
    ros::init(argc,argv,"Set_Goal"); // init del nodo 
    ros::NodeHandle n;
    ros::Publisher pub=n.advertise<geometry_msgs::PoseStamped>("/move_base_simple/goal",1000);
    tf2_ros::TransformListener tfListener(tfBuffer);//da inizializzare
    
    //non credo utile 
    ros::Rate loop_rate(T);
    
  //  ros::Subscriber sub=n.subscribe("new_goal",1000,setGoal_CallBack);
    sub=n.subscribe("new_goal",1000,setGoal_CallBack);
    ros::Subscriber sub_tf=n.subscribe("tf",1000,position_CallBack);

    //due righe di ros timer

    ros::Timer timer1 = n.createTimer(ros::Duration(0,5),check1_callBack);
    ros::Timer timer2 = n.createTimer(ros::Duration(50),check2_callBack);

    int count = 0;
    while(ros::ok){
        
        if(message_published!=0){
            pub.publish(new_goal_message);
            message_published=0;
        }
       
        ros::spinOnce();
        loop_rate.sleep();
        ++count;
    }

}