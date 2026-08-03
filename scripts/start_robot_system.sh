#!/bin/bash
echo "========================================="
echo " ROS2机器人系统启动脚本"
echo "========================================="

source /opt/ros/humble/setup.bash 2>/dev/null || echo "⚠️ ROS2环境未找到"
source ~/ros2_robot_ws/install/setup.bash 2>/dev/null || echo "⚠️ 工作空间未编译"

echo "启动所有节点..."
echo "环境加载完成"
echo "运行 ros2 node list 检查节点状态"
