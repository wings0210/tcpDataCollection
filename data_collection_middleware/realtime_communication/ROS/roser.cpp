#include"ROS/roser.h"

realtime_msgs::realtime_tcp Roser::realtime_msg;

/*
void vehicle_status_Callback(const plusgo_msgs::ByWireVehicleState &msg){
    loc.soc=(double)msg.SOC;
}
*/
void call_back(const realtime_msgs::realtime_tcp &msg){
    locations loc;

    loc.longitude=msg.longitude;
    loc.latitude=msg.latitude;
    loc.azi=msg.azi;
    loc.height=msg.height;
    loc.speed=msg.speed;
    loc.accumulative_mileage=msg.accumulative_mileage;

    loc.trip_current_id[0]=msg.current_station_id[0];
    loc.trip_current_id[1]=msg.current_station_id[1];
    loc.trip_current_id[2]=msg.current_station_id[2];
    loc.trip_current_id[3]=msg.current_station_id[3];
    loc.trip_target_id[0]=msg.target_station_id[0];
    loc.trip_target_id[1]=msg.target_station_id[1];
    loc.trip_target_id[2]=msg.target_station_id[2];
    loc.trip_target_id[3]=msg.target_station_id[3];


    loc.veh_status=msg.veh_status;
    loc.veh_auto_button=msg.auto_button_status;
    loc.trip_distance=msg.trip_distance;
    loc.trip_left_mileage=msg.trip_left_mileage;

    Tcper::buffer_queue.push(loc);
}
void pnc_Callback(const plusgo_msgs::SystemInfoReport &msg){
     Roser::realtime_msg.trip_distance=static_cast<int>(std::round(msg.trip_distance));
     if(msg.trip_left_mileage>=0){
        Roser::realtime_msg.trip_left_mileage=static_cast<int>(std::round(msg.trip_left_mileage));
     }
     Roser::realtime_msg.veh_status=msg.veh_status;
     //Roser::realtime_msg.auto_button_status=(static_cast<int>(!msg.isAutoDriveMode));
     if(msg.veh_status == 4 ){
        Roser::realtime_msg.current_station_id=msg.route_stop;
     }else{
        Roser::realtime_msg.current_station_id=msg.current_stop;
     }
     Roser::realtime_msg.target_station_id=msg.route_stop;
     //待修改
     Roser::realtime_msg.accumulative_mileage=100.3;
}
void slam_Callback(const geometry_msgs::PoseStamped &msg){
     Roser::realtime_msg.longitude=msg.pose.position.y;
     Roser::realtime_msg.latitude=msg.pose.position.x;
     Roser::realtime_msg.height=msg.pose.position.z;
     Roser::realtime_msg.azi=msg.pose.orientation.z* (180.0 / M_PI)+180;
}
void vehicle_state_Callback(const plusgo_msgs::ByWireVehicleState &msg){
     Roser::realtime_msg.speed=msg.vehicle_speed;
     Roser::realtime_msg.auto_button_status=(static_cast<int>(!msg.autoflag));
     //Roser::realtime_msg.speed=msg.vehicle_speed/3.6;

}
Roser::Roser(int argc, char **argv){

    realtime_msg.longitude=124.56782;
    realtime_msg.latitude=24.67893;
    realtime_msg.azi=34.6;
    realtime_msg.height=13.2;
    realtime_msg.accumulative_mileage=0;
    realtime_msg.auto_button_status=0;
    realtime_msg.current_station_id="";
    realtime_msg.target_station_id="";
    realtime_msg.speed=5.0;
    realtime_msg.trip_distance=0;
    realtime_msg.trip_left_mileage=0;
    realtime_msg.veh_status=3;


//    realtime_msg.longitude=0;
//    realtime_msg.latitude=0;
//    realtime_msg.azi=0;
//    realtime_msg.height=0;
//    realtime_msg.accumulative_mileage=0;
//    realtime_msg.auto_button_status=1;
//    realtime_msg.current_station_id="";
//    realtime_msg.target_station_id="";
//    realtime_msg.speed=0;
//    realtime_msg.trip_distance=0;
//    realtime_msg.trip_left_mileage=0;
//    realtime_msg.veh_status=3;

   initialization(argc, argv);

//   ros::init(argc,argv,"transmit_node");
//   ros::NodeHandle handle;
   transmit(argc, argv);

}
Roser::~Roser(){

}
void Roser::initialization(int argc, char **argv){
    ros::init(argc,argv,"realtime_node");
    std::thread ros_thread([=](){

        ros::NodeHandle n;
        ros::Subscriber sub=n.subscribe("/realtime_upload",10,call_back);
        ros::AsyncSpinner init_spinner(1);
        init_spinner.start();
        //ros::spin();

        ros::waitForShutdown();
    });
    ros_thread.detach();
}
void Roser::transmit(int argc, char **argv){
    ros::init(argc,argv,"transmit_node");
    std::thread transmit_thread([=](){

        ros::NodeHandle handle;
        //ros::NodeHandle handle;

        ros::Subscriber s1=handle.subscribe("/pnc_system_Info",10,pnc_Callback);
        ros::Subscriber s2=handle.subscribe("/bywire_vehicle_state",10,vehicle_state_Callback);
        ros::Subscriber s3=handle.subscribe("/global_pose",10,slam_Callback);
        //ros::Subscriber s4=handle.subscribe("/bywire_vehicle_state",10,vehicle_status_Callback);
        ros::Publisher  pub=handle.advertise<realtime_msgs::realtime_tcp>("/realtime_upload", 10);
        ros::AsyncSpinner spinner(3);
        //ros::AsyncSpinner spinner(4);
        ros::Rate  rate(1);
        while(ros::ok()){
         spinner.start();
         pub.publish(Roser::realtime_msg);
         rate.sleep();
        }
        ros::waitForShutdown();
    });
    transmit_thread.detach();
}
void Roser::subscribe_msg(){

}
void Roser::publish_msg(){

}
