// 2번
// #include "rclcpp/rclcpp.hpp"
// #include "sensor_msgs/msg/compressed_image.hpp"
// #include "opencv2/opencv.hpp"
// #include <memory>
// #include <functional>
// #include <iostream>
// using std::placeholders::_1;
  
// cv::VideoWriter recorder("save.mp4",cv::VideoWriter::fourcc('X','2','6','4'),10,cv::Size(640,360));


// void mysub_callback(rclcpp::Node::SharedPtr node, const sensor_msgs::msg::CompressedImage::SharedPtr msg)
// {
//     cv::Mat frame = cv::imdecode(cv::Mat(msg->data), cv::IMREAD_COLOR);
//     cv::imshow("wsl",frame);
//     recorder.write(frame);  
//     cv::waitKey(1);
//     RCLCPP_INFO(node->get_logger(), "Received Image : %s,%d,%d", msg->format.c_str(),frame.rows,frame.cols);
// }
// int main(int argc, char* argv[])
// {
//     rclcpp::init(argc, argv);
//     auto node = std::make_shared<rclcpp::Node>("camsub_wsl_8");
//     auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)); //TCP
//     //auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort(); //UDP
//     std::function<void(const sensor_msgs::msg::CompressedImage::SharedPtr msg)> fn;
//     fn = std::bind(mysub_callback, node, _1);
//     auto mysub = node->create_subscription<sensor_msgs::msg::CompressedImage>("image/compressed_8",qos_profile,fn);
//     rclcpp::spin(node);
//     rclcpp::shutdown();
//     return 0;
// }



//1번
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "opencv2/opencv.hpp"
#include <memory>
#include <functional>
#include <iostream>
using std::placeholders::_1;
  

cv::VideoWriter recorder("save.mp4",cv::VideoWriter::fourcc('X','2','6','4'),30,cv::Size(640,360));
void mysub_callback(rclcpp::Node::SharedPtr node, const sensor_msgs::msg::CompressedImage::SharedPtr msg)
{
    cv::Mat frame = cv::imdecode(cv::Mat(msg->data), cv::IMREAD_COLOR);
    cv::imshow("gray",frame);
    cv::imshow("throshed",frame); 
    cv::imshow("wsl",frame);
    recorder.write(frame);  
    cv::waitKey(1);
    RCLCPP_INFO(node->get_logger(), "Received Image : %s,%d,%d", msg->format.c_str(),frame.rows,frame.cols);
}
int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("camsub_wsl_8");
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)); //TCP
    //auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort(); //UDP
    std::function<void(const sensor_msgs::msg::CompressedImage::SharedPtr msg)> fn;
    fn = std::bind(mysub_callback, node, _1);
    auto mysub = node->create_subscription<sensor_msgs::msg::CompressedImage>("image/compressed_8",qos_profile,fn);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}