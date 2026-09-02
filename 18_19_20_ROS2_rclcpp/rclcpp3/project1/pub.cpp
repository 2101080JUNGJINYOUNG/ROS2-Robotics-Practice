#include"rclcpp/rclcpp.hpp" // ROS 2 C++ 클라이언트 라이브러리 헤더 파일 포함
#include"std_msgs/msg/string.hpp" // 표준 String 메시지 타입 헤더 파일 포함
#include"rclcpp/time_source.hpp" // ROS 2 시간 관리를 위한 헤더 파일 포함
#include<memory> // std::shared_ptr 같은 스마트 포인터 사용을 위한 헤더 파일 포함
#include<chrono> // 시간 관련 기능을 사용하기 위한 헤더 파일 포함
using namespace std::chrono_literals; // 100ms와 같은 시간 리터럴 사용을 위함
void callback(rclcpp::Node::SharedPtr node,
rclcpp::Publisher<std_msgs::msg::String>::SharedPtr mypub) // 100ms마다 호출되어 메시지를 발행하는 콜백 함수
{
    static auto message = std_msgs::msg::String(); // 발행할 String 메시지 객체 생성 및 초기화
    message.data = "Hello World!!";
    RCLCPP_INFO(node->get_logger(), "Publish: %s", message.data.c_str()); // ROS 2 로깅 시스템을 사용하여 현재 발행하는 데이터를 출력
    mypub->publish(message); // 준비된 메시지 객체를 mytopic에 발행
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node=std::make_shared<rclcpp::Node>("node_pub1");
    auto qos_profile=rclcpp::QoS(rclcpp::KeepLast(10));
    auto mypub=node->create_publisher<std_msgs::msg::String>("topic_pub1", qos_profile);
    std::function<void()> fn = std::bind(callback, node, mypub); // 콜백 함수
    auto timer = node->create_wall_timer(100ms, fn); // 타이머 생성
    rclcpp::spin(node); // 대기
    rclcpp::shutdown(); // 종료
    return 0;
}
