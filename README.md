 ROS2 机器人传感器监控系统

 📌 项目简介
这是一个基于 **ROS2 (Humble)** 的机器人传感器监控系统，涵盖了完整的机器人软件开发流程：自定义消息接口、传感器数据发布/订阅、TF2坐标变换、异步服务通信、多进程管理及Linux系统部署。

 🛠️ 技术栈
 操作系统: Ubuntu 22.04 (WSL2/Linux)
 ROS2版本: Humble Hawksbill
 编程语*: C++17
 编译调试: gcc, make, gdb, colcon
 版本控制: Git, GitHub
 通信协议: DDS, HTTP, WebSocket

 功能模块
 模块  说明  关键文件 
 自定义消息  传感器数据格式定义  `SensorData.msg` 
 自定义服务  坐标变换请求/响应  `GetTransform.srv` 
 消息工具类  封装发布/订阅/异步调用  `message_utils.hpp` 
 数据发布  模拟传感器数据发布  `sensor_publisher_node.cpp` 
 数据订阅  接收并显示数据 `sensor_subscriber_node.cpp` 
 TF2广播  底盘/雷达坐标变换  `tf2_broadcaster_node.cpp` 
 TF2监听  查询坐标变换  `tf2_listener_node.cpp` 
 异步服务  异步调用服务  `async_service_client_node.cpp` 
 服务端  响应变换请求  `service_server_node.cpp `


```bash
# 1. 编译项目
cd ~/ros2_robot_ws
colcon build

# 2. 加载环境
source install/setup.bash

# 3. 一键启动所有节点
./scripts/start_robot_system.sh

# 4. 验证运行
ros2 node list
ros2 topic list
ros2 topic echo /sensor_data
