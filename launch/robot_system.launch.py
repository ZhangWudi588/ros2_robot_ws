from launch import LaunchDescription
from launch_ros.actions import Node

def generate_launch_description():
    return LaunchDescription([
        Node(package='robot_nodes', executable='sensor_publisher', name='sensor_publisher', output='screen'),
        Node(package='robot_nodes', executable='sensor_subscriber', name='sensor_subscriber', output='screen'),
        Node(package='robot_nodes', executable='tf2_broadcaster', name='tf2_broadcaster', output='screen'),
        Node(package='robot_nodes', executable='tf2_listener', name='tf2_listener', output='screen'),
        Node(package='robot_nodes', executable='service_server', name='service_server', output='screen'),
    ])
