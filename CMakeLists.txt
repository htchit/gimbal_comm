 cmake_minimum_required(VERSION 2.8.3)
 project(gimbal_comm)

 
 ## Find catkin and any catkin packages
 find_package(catkin REQUIRED roscpp std_msgs serial message_generation message_runtime)
 
 add_message_files(FILES GimbalState.msg)
 generate_messages(DEPENDENCIES std_msgs)
 catkin_package()
 add_executable(gimbal_com src/gimbal_comm.cpp)

 include_directories(include ${catkin_INCLUDE_DIRS} ${PCL_INCLUDE_DIRS})
 ## Generate added messages and services


 add_dependencies(gimbal_com ${${PROJECT_NAME}}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

 target_link_libraries(gimbal_com ${catkin_LIBRARIES})

