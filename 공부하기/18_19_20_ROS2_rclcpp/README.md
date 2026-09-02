# 18~20장. ROS2 rclcpp — 공부하기

이 챕터부터는 직접 퍼블리셔/서브스크라이버 노드를 C++로 작성합니다. 06장(토픽 개념), 10장(인터페이스), 13장(패키지 구조), 15~17장(C++ 고급 문법)의 내용이 전부 여기서 실전 코드로 합쳐집니다.

## 목차

- [1. rclcpp란](#1-rclcpp란)
- [2. 기본 퍼블리셔 노드 뜯어보기](#2-가장-기본적인-퍼블리셔-노드-뜯어보기-rclcppproject1~3)
- [3. rclcpp2 — HZ 측정](#3-rclcpp2--hz-측정까지-포함된-발전된-실습)
- [4. rclcpp3 — pub/sub 완전 분리](#4-rclcpp3--퍼블리셔서브스크라이버-완전-분리--콜백-구조-project1-project2)
- [5. 빌드하기](#5-빌드하기)
- [6. 스스로 확인하는 질문](#6-스스로-확인하는-질문)

## 1. rclcpp란

`rclcpp`는 "ROS Client Library for C++"의 줄임말로, C++로 ROS2 노드를 작성할 때 쓰는 공식 라이브러리입니다. 노드 생성, 퍼블리셔/구독자 생성, 타이머, 로깅 등 ROS2 핵심 기능을 C++ API로 제공합니다. Python으로 같은 역할을 하는 라이브러리는 `rclpy`입니다.

## 2. 가장 기본적인 퍼블리셔 노드 뜯어보기 (rclcpp/project1~3)

```cpp
rclcpp::init(argc, argv);                                            // (1)
auto node = std::make_shared<rclcpp::Node>("node_pub2");             // (2)
auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));                // (3)
auto mypub = node->create_publisher<std_msgs::msg::Int32>("topic_pub2", qos_profile); // (4)
std_msgs::msg::Int32 message;                                        // (5)
message.data = 0;
rclcpp::WallRate loop_rate(1.0);                                     // (6)
while (rclcpp::ok())                                                 // (7)
{
    RCLCPP_INFO(node->get_logger(), "Publish: %d", message.data++);  // (8)
    mypub->publish(message);                                         // (9)
    loop_rate.sleep();                                               // (10)
}
rclcpp::shutdown();                                                  // (11)
```

1. **`rclcpp::init`** — ROS2 시스템 초기화. 모든 rclcpp 프로그램은 이 줄로 시작합니다.
2. **`std::make_shared<rclcpp::Node>`** — `"node_pub2"`라는 이름의 노드 생성. 15~17장에서 배운 대로 `shared_ptr`로 관리되어 메모리 해제를 직접 신경 쓸 필요가 없습니다.
3. **QoS 설정** — "최신 메시지 10개만 버퍼에 유지"(03~05장에서 배운 개념).
4. **`create_publisher`** — `"topic_pub2"` 토픽에 `Int32` 타입 메시지를 발행할 퍼블리셔 생성.
5. **메시지 객체** — `Int32`는 정수 하나(`data`)만 담는 단순한 표준 메시지 타입.
6. **`WallRate`** — 반복 주기(1.0 = 1Hz, 초당 1번).
7. **`while (rclcpp::ok())`** — Ctrl+C(SIGINT) 전까지 계속 반복.
8. **`RCLCPP_INFO`** — ROS2 전용 로그 매크로. 노드 이름·타임스탬프를 자동으로 붙여 출력.
9. **`publish`** — 준비된 메시지를 실제로 토픽에 발행.
10. **`loop_rate.sleep()`** — 1Hz 주기를 맞추기 위해 남은 시간만큼 대기.
11. **`shutdown`** — 루프 종료 시 ROS2 시스템 정리.

이 구조가 ROS2에서 가장 기본적인 "직접 루프를 돌며 발행하는" 퍼블리셔 패턴입니다.

## 3. rclcpp2 — HZ 측정까지 포함된 발전된 실습

기본 구조는 rclcpp와 같지만, 실행 결과에 `ros2 topic hz`로 측정한 실제 발행 주파수(예: 9.993Hz, 주기 0.0993초)를 기록해 "의도한 주기(1Hz 등)와 실제 측정값이 일치하는지" 검증하는 단계가 추가되었습니다. `WallRate` 설정값과 실측값이 미세하게 다른 이유는 루프 안 로그 출력·발행에 걸리는 처리 시간만큼 `sleep`이 짧아지기 때문입니다.

## 4. rclcpp3 — 퍼블리셔/서브스크라이버 완전 분리 + 콜백 구조 (project1, project2)

여기서부터 람다/`std::bind`/타이머 콜백(15~17장)이 실전에 등장합니다.

**퍼블리셔 (`pub.cpp`)**:

```cpp
void callback(rclcpp::Node::SharedPtr node,
              rclcpp::Publisher<std_msgs::msg::String>::SharedPtr mypub)
{
    static auto message = std_msgs::msg::String();
    message.data = "Hello World!!";
    RCLCPP_INFO(node->get_logger(), "Publish: %s", message.data.c_str());
    mypub->publish(message);
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("node_pub1");
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
    auto mypub = node->create_publisher<std_msgs::msg::String>("topic_pub1", qos_profile);
    std::function<void()> fn = std::bind(callback, node, mypub);   // (A)
    auto timer = node->create_wall_timer(100ms, fn);               // (B)
    rclcpp::spin(node);                                            // (C)
    rclcpp::shutdown();
    return 0;
}
```

앞의 기본 퍼블리셔와 가장 큰 차이는 "직접 while 루프를 돌지 않는다"는 것입니다.

- **(A)** `callback`은 원래 `(node, mypub)` 두 인자를 받는데, 타이머가 기대하는 콜백은 인자 없는 함수(`void()`)입니다. `std::bind`로 `node`와 `mypub`을 미리 고정해서 인자 없이 호출 가능한 `fn`을 만듭니다.
- **(B)** `create_wall_timer(100ms, fn)` — "100ms마다 `fn`을 자동 호출해달라"고 등록합니다.
- **(C)** `rclcpp::spin(node)` — 앞선 `while(rclcpp::ok())` 루프를 대신하는 표준 대기 함수. 등록된 타이머/구독 콜백이 호출될 시점이 되면 자동으로 호출해주고, 그 사이에는 대기합니다. 즉 "언제 콜백을 호출할지"의 제어권을 개발자가 아니라 rclcpp(프레임워크)가 가져가는 구조로, 01장에서 배운 IoC 개념이 실제 코드로 나타난 것입니다.

**서브스크라이버 (`sub.cpp`)**:

```cpp
void mysub_callback(rclcpp::Node::SharedPtr node, const std_msgs::msg::String::SharedPtr msg)
{
    RCLCPP_INFO(node->get_logger(), "Received message: %s", msg->data.c_str());
}

int main(int argc, char* argv[])
{
    rclcpp::init(argc, argv);
    auto node = std::make_shared<rclcpp::Node>("node_sub1");
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10));
    std::function<void(const std_msgs::msg::String::SharedPtr)> fn =
        std::bind(mysub_callback, node, std::placeholders::_1);
    auto mysub = node->create_subscription<std_msgs::msg::String>("topic_pub1", qos_profile, fn);
    rclcpp::spin(node);
    rclcpp::shutdown();
    return 0;
}
```

구독 콜백이 기대하는 형태는 "메시지 하나를 인자로 받는 함수"입니다. `mysub_callback`은 원래 `(node, msg)` 두 인자를 받으므로, `std::bind`로 `node`는 고정하고 `msg` 자리에는 `std::placeholders::_1`(나중에 채워질 자리)을 넣어 "메시지 하나만 받으면 되는" 형태로 바꿉니다. `create_subscription<타입>("토픽이름", qos, 콜백)`으로 같은 토픽(`"topic_pub1"`)을 구독하면, 퍼블리셔가 발행할 때마다 `mysub_callback`이 자동 호출됩니다.

이 pub/sub 분리 + 콜백 구조는 24장(다이내믹셀 원격조작), 25장(라인검출)에서도 반복되는 ROS2의 가장 표준적인 패턴이므로, 여기서 확실히 이해해두는 것이 중요합니다.

## 5. 빌드하기

각 프로젝트 폴더의 `CMakeLists.txt`는 11장·13장에서 배운 문법과 동일하되 `find_package(rclcpp REQUIRED)`, `find_package(std_msgs REQUIRED)`처럼 ROS2 라이브러리를 추가로 찾습니다. 워크스페이스 루트에서 `colcon build --packages-select <패키지명>`으로 빌드한 뒤, 터미널 두 개에서 각각 `ros2 run <패키지명> pub`, `ros2 run <패키지명> sub`를 실행하면 퍼블리셔가 발행한 메시지를 서브스크라이버가 받는 것을 직접 확인할 수 있습니다.

## 6. 스스로 확인하는 질문

- `while(rclcpp::ok())` 방식과 `rclcpp::spin()` 방식의 차이는?
- `std::bind`가 `pub.cpp`와 `sub.cpp`에서 각각 어떤 문제를 해결하는가?
- `create_wall_timer`와 `create_subscription`에 콜백을 등록할 때, 왜 콜백 함수의 "인자 개수"가 정확히 맞아야 하는가?
- QoS의 `KeepLast(10)`이 실제로 무엇을 의미하는가? (03~05장과 연결)
