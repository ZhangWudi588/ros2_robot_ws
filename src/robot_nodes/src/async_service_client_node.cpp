#include "rclcpp/rclcpp.hpp"
#include "robot_interfaces/srv/get_transform.hpp"
#include "robot_nodes/message_utils.hpp"
#include <memory>

class AsyncServiceClientNode : public rclcpp::Node {
public:
    AsyncServiceClientNode() : Node("async_service_client") {
        // 第1行：创建工具类实例
        utils_ = std::make_shared<robot_nodes::MessageUtils>(shared_from_this());

        // 第2行：每5秒调用一次异步服务
        timer_ = this->create_wall_timer(
            std::chrono::milliseconds(5000),
            [this]() { this->call_transform_service(); }
        );

        RCLCPP_INFO(this->get_logger(), " 异步服务客户端已启动");
    }

private:
    // 第3行：异步调用服务（核心！）
    void call_transform_service() {
        //  创建请求
        auto request = std::make_shared<robot_interfaces::srv::GetTransform::Request>();
        request->target_frame = "map";
        request->source_frame = "base_link";

        RCLCPP_INFO(this->get_logger(), " 异步调用服务: GetTransform");

        //   关键：异步发送，不阻塞主线程！
        utils_->async_call_service<robot_interfaces::srv::GetTransform>(
            "/get_transform_service",
            request,
            //  回调函数：服务响应后自动执行
            [this](robot_interfaces::srv::GetTransform::Response::SharedPtr response) {
                if (response->success) {
                    RCLCPP_INFO(this->get_logger(),
                        " 异步响应成功: x=%.3f, y=%.3f, z=%.3f",
                        response->transform.transform.translation.x,
                        response->transform.transform.translation.y,
                        response->transform.transform.translation.z
                    );
                } else {
                    RCLCPP_ERROR(this->get_logger(), " 异步响应失败: %s",
                        response->error_message.c_str());
                }
            }
        );

        // 第4行：请求已发送，主线程继续执行
        RCLCPP_INFO(this->get_logger(), " 请求已发送，主线程继续运行");
    }

    std::shared_ptr<robot_nodes::MessageUtils> utils_;
    rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char** argv) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<AsyncServiceClientNode>());
    rclcpp::shutdown();
    return 0;
}
