cmake_minimum_required(VERSION 3.8)
project(robot_nodes)

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(tf2_ros REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(robot_interfaces REQUIRED)

include_directories(include)

#所有节点
add_executable(sensor_publisher src/sensor_publisher_node.cpp)
ament_target_dependencies(sensor_publisher rclcpp robot_interfaces robot_nodes)

add_executable(sensor_subscriber src/sensor_subscriber_node.cpp)
ament_target_dependencies(sensor_subscriber rclcpp robot_interfaces robot_nodes)

add_executable(tf2_broadcaster src/tf2_broadcaster_node.cpp)
ament_target_dependencies(tf2_broadcaster rclcpp tf2_ros geometry_msgs robot_nodes)

add_executable(tf2_listener src/tf2_listener_node.cpp)
ament_target_dependencies(tf2_listener rclcpp tf2_ros geometry_msgs robot_nodes)

add_executable(service_server src/service_server_node.cpp)
ament_target_dependencies(service_server rclcpp robot_interfaces robot_nodes)

install(TARGETS sensor_publisher sensor_subscriber tf2_broadcaster tf2_listener service_server
  DESTINATION lib/${PROJECT_NAME})

ament_package()
