#include "rclcpp/rclcpp.hpp"
#include "pubpub/cam_pub_node.hpp"

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);

    // 기본 설정(노드 이름, 토픽, 비디오 소스)으로 노드 생성
    auto node = std::make_shared<CamPubNode>();

    // 필요하면 이렇게 직접 지정해서 만들 수도 있음:
    // auto node = std::make_shared<CamPubNode>(
    //     "campub_7",
    //     "image/compressed",   // 구독 노드와 맞추고 싶을 때
    //     "/home/linux/simulation/5_lt_cw_100rpm_out.mp4");

    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
