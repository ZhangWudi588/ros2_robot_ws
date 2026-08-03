#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/msg/sensor_data.hpp"
#include "robot_nodes/message_utils.hpp"

class SensorSubscriberNode : public rclcpp::Node {
public:
    SensorSubscriberNode() : Node("sensor_subscriber") {
        utils_ = std::make_shared<robot_nodes::MessageUtils>(shared_from_this());
        utils_->create_subscription<robot_interfaces::msg::SensorData>("/sensor_data",
            std::bind(&SensorSubscriberNode::sensor_callback, this, std::placeholders::_1));
    }

private:
    void sensor_callback(const robot_interfaces::msg::SensorData::SharedPtr msg) {
        RCLCPP_INFO(this->get_logger(), " 收到: device=%s, voltage=%.2fV, fault=%s",
            msg->device_id.c_str(), msg->voltage, msg->fault_type.c_str());
    }

    std::shared_ptr<robot_nodes::MessageUtils> utils_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SensorSubscriberNode>());
    rclcpp::shutdown();
    return 0;
}
