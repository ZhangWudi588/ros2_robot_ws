#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/msg/sensor_data.hpp"
#include "robot_nodes/message_utils.hpp"

class SensorPublisherNode : public rclcpp::Node {
public:
    SensorPublisherNode() : Node("sensor_publisher") {
        utils_ = std::make_shared<robot_nodes::MessageUtils>(shared_from_this());
        publisher_ = utils_->create_publisher<robot_interfaces::msg::SensorData>("/sensor_data");
        timer_ = this->create_wall_timer(std::chrono::milliseconds(1000),
            [this]() { this->publish_sensor_data(); });
    }

private:
    void publish_sensor_data() {
        auto msg = robot_interfaces::msg::SensorData();
        msg.voltage = 5.0 + (rand() % 100 - 50) / 100.0;
        msg.current = 0.5 + (rand() % 40 - 20) / 100.0;
        msg.fault_type = msg.voltage < 4.5 ? "严重故障" : (msg.voltage < 4.8 ? "轻度故障" : "正常");
        msg.device_id = "sensor_01";
        publisher_->publish(msg);
        RCLCPP_INFO(this->get_logger(), "📤 发布: voltage=%.2fV, fault=%s",
            msg.voltage, msg.fault_type.c_str());
    }

    rclcpp::Publisher<robot_interfaces::msg::SensorData>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    std::shared_ptr<robot_nodes::MessageUtils> utils_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<SensorPublisherNode>());
    rclcpp::shutdown();
    return 0;
}
