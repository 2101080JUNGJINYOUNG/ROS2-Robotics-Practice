#ifndef PUBPUB_CAM_PUB_NODE_HPP_
#define PUBPUB_CAM_PUB_NODE_HPP_

#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "std_msgs/msg/header.hpp"
#include "cv_bridge/cv_bridge.h"
#include "opencv2/opencv.hpp"

#include <string>

class CamPubNode : public rclcpp::Node
{
public:
    // node_name: 노드 이름
    // topic_name: 퍼블리시할 토픽 이름
    // video_source: 영상 파일 또는 카메라 파이프라인 문자열
    CamPubNode(
        const std::string & node_name   = "campub_7",
        const std::string & topic_name  = "image/compressed_7",
        const std::string & video_source = "/home/linux/simulation/7_lt_ccw_100rpm_in.mp4");

    ~CamPubNode() override = default;

private:
    void publish_frame();  // 타이머 콜백에서 한 프레임씩 퍼블리시

    rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::QoS qos_profile_;

    cv::VideoCapture cap_;
    cv::Mat frame_;
    std::string video_source_;
};

#endif  // PUBPUB_CAM_PUB_NODE_HPP_
