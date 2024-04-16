cmake_minimum_required(VERSION 3.0.2)
project(realtime_msgs)



find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  geometry_msgs
  message_generation
)

add_message_files(
     FILES
     realtime_tcp.msg
)

generate_messages(
     DEPENDENCIES
     std_msgs
     geometry_msgs
) 



include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)




