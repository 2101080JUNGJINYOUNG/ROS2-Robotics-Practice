#include "rclcpp/rclcpp.hpp"
#include "subsub/vision.hpp"
#include <signal.h>

void ctrlc(int);

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    signal(SIGINT, ctrlc);
    auto node = std::make_shared<CamSubNode>();
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}