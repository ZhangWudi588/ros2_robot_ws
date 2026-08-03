#include "rclcpp/rclcpp.hpp"
#include "robot_nodes/message_utils.hpp"
#include "geometry_msgs/msg/transform_stamped.hpp"

class Tf2BroadcasterNode : public rclcpp::Node {
public:
    Tf2BroadcasterNode() : Node("tf2_broadcaster") {
        utils_ = std::make_shared<robot_nodes::MessageUtils>(shared_from_this());
        timer_ = this->create_wall_timer(std::chrono::milliseconds(50),
            [this]() { this->broadcast_tf(); });
    }

private:
    void broadcast_tf() {
        geometry_msgs::msg::TransformStamped map_to_base;
        map_to_base.header.stamp = this->now();
        map_to_base.header.frame_id = "map";
        map_to_base.child_frame_id = "base_link";
        double t = this->now().seconds();
        map_to_base.transform.translation.x = 1.0 * sin(t * 0.1);
        map_to_base.transform.translation.y = 0.5 * cos(t * 0.1);
        map_to_base.transform.translation.z = 0.0;
        map_to_base.transform.rotation.w = 1.0;
        utils_->broadcast_transform(map_to_base);

        geometry_msgs::msg::TransformStamped base_to_laser;
        base_to_laser.header.stamp = this->now();
        base_to_laser.header.frame_id = "base_link";
        base_to_laser.child_frame_id = "laser_link";
        base_to_laser.transform.translation.x = 0.2;
        base_to_laser.transform.translation.y = 0.0;
        base_to_laser.transform.translation.z = 0.1;
        base_to_laser.transform.rotation.w = 1.0;
        utils_->broadcast_transform(base_to_laser);
    }

    std::shared_ptr<robot_nodes::MessageUtils> utils_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Tf2BroadcasterNode>());
    rclcpp::shutdown();
    return 0;
}
