#include "rclcpp/rclcpp.hpp"
#include "robot_nodes/message_utils.hpp"

class Tf2ListenerNode : public rclcpp::Node {
public:
    Tf2ListenerNode() : Node("tf2_listener") {
        utils_ = std::make_shared<robot_nodes::MessageUtils>(shared_from_this());
        timer_ = this->create_wall_timer(std::chrono::milliseconds(1000),
            [this]() { this->check_transform(); });
    }

private:
    void check_transform() {
        auto transform = utils_->get_transform("map", "laser_link");
        RCLCPP_INFO(this->get_logger(), "🔍 激光雷达位置: x=%.3f, y=%.3f, z=%.3f",
            transform.transform.translation.x,
            transform.transform.translation.y,
            transform.transform.translation.z);
    }

    std::shared_ptr<robot_nodes::MessageUtils> utils_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Tf2ListenerNode>());
    rclcpp::shutdown();
    return 0;
}
