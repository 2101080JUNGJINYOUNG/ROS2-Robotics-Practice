#include "rclcpp/rclcpp.hpp"                      // ros의 핵심 기능을 포함
#include "std_msgs/msg/int32.hpp"                 // 메세지 타입을 사용하기 위해 포함
#include <memory>
#include <chrono>
int main(int argc, char * argv[])
{
	rclcpp::init(argc, argv);                                                  // ros2 시스템 초기화
	auto node = std::make_shared<rclcpp::Node>("node_pub2");                   // node_pub2라는 이름으로 노드를 생성하고 스마트 포인터로 노드 관리함
	auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));                      // 최신 메시지 10개 까지만 버퍼에 유지
	auto mypub = node->create_publisher<std_msgs::msg::Int32>("topic_pub2", 
	qos_profile );                                                             // 퍼블리셔 생성
	std_msgs::msg::Int32 message;                                              // 발생할 메시지 객체 선언
	message.data = 0;                                                          // 멤버변수에 0을 넣어 초기화
	rclcpp::WallRate loop_rate(1.0);                                           //반복주파수를 저장하는 객체(단위 Hz)
	while(rclcpp::ok())                                                        // 컨트롤+c 누르면 꺼지는거
	{
		RCLCPP_INFO(node->get_logger(), "Publish: %d", message.data++);         // 출력
		mypub->publish(message);                                                // mypub에서 준비된 메시지를 전송
		//rclcpp::spin_some(node);
		loop_rate.sleep(); //반복주파수에서 남은 시간 만큼 sleep                   // 1초 - 위의 코드 시간만큼 잠
	}
	rclcpp::shutdown();                                                          // 루프가 종료되면 ros2 시스템 종료
	return 0;                                                                    // 0을 반환
}
