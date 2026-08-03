#pragma once

#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/msg/sensor_data.hpp"
#include "robot_interfaces/srv/get_transform.hpp"
#include <tf2_ros/transform_listener.h>
#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_broadcaster.h>

namespace robot_nodes {

class MessageUtils {
public:
    explicit MessageUtils(rclcpp::Node::SharedPtr node) : node_(node) {
        tf_buffer_ = std::make_shared<tf2_ros::Buffer>(node_->get_clock());
        tf_listener_ = std::make_shared<tf2_ros::TransformListener>(*tf_buffer_);
        tf_broadcaster_ = std::make_shared<tf2_ros::TransformBroadcaster>(node_);
    }

    template<typename MessageT>
    rclcpp::Publisher<MessageT>::SharedPtr create_publisher(
        const std::string& topic_name,
        const rclcpp::QoS& qos = rclcpp::SensorDataQoS()) {
        return node_->create_publisher<MessageT>(topic_name, qos);
    }

    template<typename MessageT, typename CallbackT>
    rclcpp::Subscription<MessageT>::SharedPtr create_subscription(
        const std::string& topic_name,
        CallbackT&& callback) {
        return node_->create_subscription<MessageT>(
            topic_name, rclcpp::SensorDataQoS(), std::forward<CallbackT>(callback));
    }

    template<typename ServiceT, typename CallbackT>
    void async_call_service(const std::string& service_name,
        typename ServiceT::Request::SharedPtr request,
        CallbackT&& callback) {
        auto client = node_->create_client<ServiceT>(service_name);
        if (!client->wait_for_service(std::chrono::seconds(1))) {
            RCLCPP_ERROR(node_->get_logger(), "服务不可用: %s", service_name.c_str());
            return;
        }
        auto future = client->async_send_request(request);
        std::thread([callback, future]() {
            auto status = future.wait_for(std::chrono::seconds(5));
            if (status == std::future_status::ready) {
                callback(future.get());
            }
        }).detach();
    }

    geometry_msgs::msg::TransformStamped get_transform(
        const std::string& target_frame,
        const std::string& source_frame,
        const rclcpp::Time& time = rclcpp::Time(0)) {
        geometry_msgs::msg::TransformStamped transform;
        try {
            transform = tf_buffer_->lookupTransform(target_frame, source_frame, time);
        } catch (const tf2::TransformException& ex) {
            RCLCPP_ERROR(node_->get_logger(), "TF2错误: %s", ex.what());
        }
        return transform;
    }

    void broadcast_transform(const geometry_msgs::msg::TransformStamped& transform) {
        tf_broadcaster_->sendTransform(transform);
    }

private:
    rclcpp::Node::SharedPtr node_;
    std::shared_ptr<tf2_ros::Buffer> tf_buffer_;
    std::shared_ptr<tf2_ros::TransformListener> tf_listener_;
    std::shared_ptr<tf2_ros::TransformBroadcaster> tf_broadcaster_;
};

}
