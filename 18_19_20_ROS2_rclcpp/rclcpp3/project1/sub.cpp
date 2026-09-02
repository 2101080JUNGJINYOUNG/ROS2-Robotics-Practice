#include"rclcpp/rclcpp.hpp" // ROS 2 C++ 클라이언트 라이브러리 헤더 파일 포함
#include"std_msgs/msg/string.hpp" // 표준 String 메시지 타입 헤더 파일 포함
#include<memory> // std::shared_ptr 같은 스마트 포인터 사용을 위한 헤더 파일 포함
#include<functional> // std::function 및 std::bind 사용을 위한 헤더 파일 포함

void mysub_callback(rclcpp::Node::SharedPtr node, const std_msgs::msg::String::SharedPtr msg) // 토픽으로부터 메시지를 수신할 때마다 호출되는 콜백 함수 정의
{
    RCLCPP_INFO(node->get_logger(), "Received message: %s", msg->data.c_str()); // ROS 2 로깅 시스템을 사용하여 수신된 메시지의 내용을 출력
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv); // ROS 2 시스템 초기화 
    auto node = std::make_shared<rclcpp::Node>("node_sub1"); // "node_sub1" 이름을 가진 ROS 2 노드 객체 생성
    auto qos_profile= rclcpp::QoS(rclcpp::KeepLast(10)); // QoS 설정
    
    std::function<void(const std_msgs::msg::String::SharedPtr)> fn = std::bind(mysub_callback, node, std::placeholders::_1); // 콜백 함수
    auto mysub= node->create_subscription<std_msgs::msg::String>("topic_pub1",qos_profile,fn); // std_msgs::msg::String 타입을 "topic_pub1" 토픽으로부터 구독할 Subscriber 객체 생성
    
    rclcpp::spin(node); // 노드가 종료될 때까지 메시지 이벤트를 처리하며 대기
    rclcpp::shutdown(); // ROS 2 시스템 정리 및 종료
    return 0; // 프로그램 종료
}
