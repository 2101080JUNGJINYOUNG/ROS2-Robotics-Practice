# 코드 간략 설명

## pub (publish)

라인 트레이서 실험에서 **영상 소스를 ROS2 이미지 토픽으로 퍼블리시하는 노드**입니다.  
아래는 pub 노드의 헤더 파일(`pub.hpp`)에 대한 간단한 설명입니다.  
(`pub.cpp`는 나중에 따로 섹션을 추가하면 됨)

---

### cam_pub_node.hpp

**파일 역할 (Role)**  
- `CamPubNode` 클래스를 선언하는 헤더 파일  
- 비디오 소스를 읽어서 `sensor_msgs::msg::CompressedImage` 타입으로 퍼블리시하는 ROS2 노드의 인터페이스 정의

---

#### 1. 생성자 기본 인자

```cpp
CamPubNode(
    const std::string & node_name   = "campub_7",
    const std::string & topic_name  = "image/compressed_7",
    const std::string & video_source = "/home/linux/simulation/7_lt_ccw_100rpm_in.mp4");
```
- node_name (기본값: `campub_7`)  
  ROS2에서 사용할 node 이름(Node name)입니다.

- topic_name (기본값: `image/compressed_7`)  
  압축 이미지를 publish 할 topic 이름(Topic name)입니다.

- video_source (기본값: `/home/linux/simulation/7_lt_ccw_100rpm_in.mp4`)  
  OpenCV VideoCapture 가 여는 video source path 입니다.

- CamPubNode() 처럼 인자 없이 호출하면  
  위 기본값(Node name, Topic name, Video source)이 그대로 자동 적용됩니다.

#### 2. private 멤버 및 함수 역할

```cpp
private:
    void publish_frame();  // 타이머 콜백에서 한 프레임씩 publish

    rclcpp::Publisher<sensor_msgs::msg::CompressedImage>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::QoS qos_profile_;

    cv::VideoCapture cap_;
    cv::Mat frame_;
    std::string video_source_;
};
```

- publish_frame()  
  타이머 주기로 호출되어 한 프레임을 읽어와 토픽으로 이미지를 publish 하는 함수이다.

- publisher_  
  sensor_msgs::msg::CompressedImage 메시지를 지정된 토픽으로 publish 하는 ROS2 publisher 이다.

- timer_  
  일정 주기로 publish_frame() 함수를 호출하도록 스케줄링 하는 타이머이다.

- qos_profile_  
  publisher 에 사용될 QoS 설정 정보를 저장하는 멤버이다.

- cap_  
  영상 파일 또는 카메라로부터 프레임을 읽어오는 OpenCV VideoCapture 객체이다.

- frame_  
  현재 읽어온 한 프레임 이미지를 담는 cv::Mat 버퍼이다.

- video_source_  
  사용할 영상 소스의 경로 또는 파이프라인 문자열을 저장하는 멤버이다.

---

### cam_pub_node.cpp

#### 1. CamPubNode 생성자

```cpp
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

    // 압축 이미지 publisher 생성
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
```
- 이 생성자는 node name, topic name, video source 를 인자로 받아 ROS2 node 를 초기화한다.
QoS 설정을 구성한 뒤 압축 이미지 publisher 를 생성하고, VideoCapture 로 video source 를 연다.
video source 를 열지 못하면 error 로그를 출력하고 예외를 던지며, 약 30 FPS(33ms 간격)로 publish_frame() 을 호출하는 타이머를 만든다.

```cpp
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

    // publish
    publisher_->publish(*msg);
    // RCLCPP_INFO(this->get_logger(), "Published compressed frame");
}
```
- 이 함수는 VideoCapture 에서 한 프레임을 읽어와 frame_ 에 저장하고, 프레임이 비어 있으면 error 로그를 남기고 node 를 종료한다.
정상적으로 프레임을 읽어온 경우 현재 시간을 기준으로 header 를 생성하고, cv_bridge 를 사용해 OpenCV Mat 을 CompressedImage 메시지로 변환한다.
마지막으로 publisher_ 를 통해 변환된 이미지를 지정된 topic 으로 publish 하는 역할을 한다.
