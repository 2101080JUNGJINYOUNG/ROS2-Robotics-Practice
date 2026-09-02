#include "pubpub/cam_pub_node.hpp"

using namespace std::chrono_literals;

CamPubNode::CamPubNode(
    const std::string & node_name,
    const std::string & topic_name,
    const std::string & video_source)
: Node(node_name),
  qos_profile_(10), 
  video_source_(video_source)
{
    // QoS 설정 (기본: RELIABLE)
    qos_profile_ = rclcpp::QoS(rclcpp::KeepLast(10));
    // 필요하면 best_effort()로 변경 가능:
    // qos_profile_ = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort();

    // 압축 이미지 퍼블리셔 생성
    publisher_ = this->create_publisher<sensor_msgs::msg::CompressedImage>(
        topic_name, qos_profile_);

    // 비디오 소스 열기 (지금은 파일)
    cap_.open(video_source_);
    // 만약 Jetson 카메라 GStreamer 파이프라인을 쓰고 싶으면:
    // std::string src = "nvarguscamerasrc sensor-id=0 ! "
    //     "video/x-raw(memory:NVMM), width=(int)640, height=(int)360, "
    //     "format=(string)NV12 ! nvvidconv flip-method=0 ! video/x-raw, "
    //     "width=(int)640, height=(int)360, format=(string)BGRx ! "
    //     "videoconvert ! video/x-raw, format=(string)BGR ! appsink";
    // cap_.open(src, cv::CAP_GSTREAMER);

    if (!cap_.isOpened()) {
        RCLCPP_ERROR(this->get_logger(), "Could not open video source: %s", video_source_.c_str());
        throw std::runtime_error("Could not open video source");
    }

    // 약 30 FPS → 33ms 주기로 타이머 콜백 호출
    timer_ = this->create_wall_timer(
        33ms,
        std::bind(&CamPubNode::publish_frame, this));
}

void CamPubNode::publish_frame()
{
    cap_ >> frame_;
    if (frame_.empty()) {
        RCLCPP_ERROR(this->get_logger(), "Frame empty (video end or read error)");
        rclcpp::shutdown();
        return;
    }

    // 헤더 생성 (타임스탬프)
    std_msgs::msg::Header header;
    header.stamp = this->now();

    // OpenCV Mat → CompressedImage 메시지로 변환 (bgr8 → 압축)
    auto msg = cv_bridge::CvImage(header, "bgr8", frame_).toCompressedImageMsg();

    // 퍼블리시
    publisher_->publish(*msg);
    // RCLCPP_INFO(this->get_logger(), "Published compressed frame");
}
