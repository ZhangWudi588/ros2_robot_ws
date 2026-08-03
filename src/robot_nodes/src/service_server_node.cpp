#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/srv/get_transform.hpp"
#include "robot_nodes/message_utils.hpp"

class ServiceServerNode : public rclcpp::Node {
public:
    ServiceServerNode() : Node("service_server") {
        utils_ = std::make_shared<robot_nodes::MessageUtils>(shared_from_this());
        service_ = this->create_service<robot_interfaces::srv::GetTransform>("/get_transform_service",
            std::bind(&ServiceServerNode::handle_request, this, std::placeholders::_1, std::placeholders::_2));
    }

private:
    void handle_request(
        const robot_interfaces::srv::GetTransform::Request::SharedPtr request,
        robot_interfaces::srv::GetTransform::Response::SharedPtr response) {
        auto transform = utils_->get_transform(request->target_frame, request->source_frame);
        if (transform.header.frame_id.empty()) {
            response->success = false;
            response->error_message = "无法获取变换";
        } else {
            response->success = true;
            response->transform = transform;
        }
    }

    rclcpp::Service<robot_interfaces::srv::GetTransform>::SharedPtr service_;
    std::shared_ptr<robot_nodes::MessageUtils> utils_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<ServiceServerNode>());
    rclcpp::shutdown();
    return 0;
}
