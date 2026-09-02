#include "rclcpp/rclcpp.hpp"                    // ros2 헤더파일 포함
#include "geometry_msgs/msg/vector3.hpp"        // 메시지 타입을 사용하기 위해 포함
int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);                                    // ros2 초기화
    auto node = std::make_shared<rclcpp::Node>("nodepub3");      // nodepub3라는 노드를 생성하고 스마트 포인터로 관리
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));        // qos 파일 설정 최신 10개까지 유지
    auto mypub = node->create_publisher<geometry_msgs::msg::Vector3>("topic_pub1_2", qos_profile ); // 퍼블리셔 생성
    
    geometry_msgs::msg::Vector3 message;               // Vector3 메시지 객체 선언
    rclcpp::WallRate loop_rate(1.0);                   // 주파수 1로 맞춤
    while(rclcpp::ok())
    {
        std::cout << "첫번째 실수값을 입력해주세요";      // 안내메시지 출력
        std::cin >> message.x;                          // 값을 받아서 저장
        std::cout << "두번째 실수값을 입력해주세요";      // 안내메시지 출력
        std::cin >> message.y;                          // 값을 받아서 저장
        std::cout << "세번째 실수값을 입력해주세요";      // 안내메시지 출력
        std::cin >> message.z;                          // 값을 받아서 저장
        RCLCPP_INFO(node->get_logger(), "Publish: %f,%f,%f", message.x,message.y,message.z);  // 안내메시지 출력
        mypub->publish(message);                        // 전송
        loop_rate.sleep();                     
    }
    rclcpp::shutdown();
    return 0;
}
