#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/int32.hpp"
#include <memory>
#include <chrono>
#include <functional>
//#include <string>
using namespace std::chrono_literals;
void callback(rclcpp::Node::SharedPtr node,
rclcpp::Publisher<std_msgs::msg::Int32>::SharedPtr mypub)
{
        static int count=0;
        auto message = std_msgs::msg::Int32();
        message.data = count++;
        RCLCPP_INFO(node->get_logger(), "Publish: %d", message.data);
        mypub->publish(message);
}
int main(int argc, char* argv[])
{
        rclcpp::init(argc, argv);
        auto node = std::make_shared<rclcpp::Node>("mynode");
        auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
        auto pub = node->create_publisher<std_msgs::msg::Int32>("mytopic", qos_profile);
        std::function<void()> fn = std::bind(callback, node, pub);
        auto timer = node->create_wall_timer(100ms, fn);
        rclcpp::spin(node);
        rclcpp::shutdown();
        return 0;
}

