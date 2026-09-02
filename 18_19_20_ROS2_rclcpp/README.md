# 18~20장. ROS2 rclcpp (C++ 클라이언트 라이브러리)

> 📘 **배경 지식 정리**: 이 실습과 관련된 개념 설명은 [공부하기 문서](../공부하기/18_19_20_ROS2_rclcpp/README.md)에서 확인할 수 있습니다.


`rclcpp`를 이용해 퍼블리셔/서브스크라이버 노드를 직접 작성하며 ROS2 C++ 개발의 기본기를 익힌 3회차 실습입니다. 회차가 진행될수록 QoS, 타이머 콜백, `std::bind`를 활용한 콜백 구성 등 점점 더 실전에 가까운 패턴으로 발전합니다.

## rclcpp/ (1회차 — Int32 퍼블리셔)

- `project1`~`project3`: `std_msgs::msg::Int32` 메시지를 `rclcpp::WallRate`로 1Hz마다 발행하는 기본 퍼블리셔 노드
- QoS는 `rclcpp::QoS(rclcpp::KeepLast(10))`로 최신 메시지 10개만 버퍼에 유지
- `while(rclcpp::ok())` 루프 안에서 직접 메시지를 카운트업하며 발행 — ROS2에서 가장 기본적인 퍼블리셔 구조

## rclcpp2/ (2회차)

- `project1`~`project3`: 1회차보다 발전된 형태의 퍼블리셔/구독 실습 (실행 결과로 HZ 측정치까지 기록 — 예: 발행 주파수 9.993Hz, 주기 0.0993s)

## rclcpp3/ (3회차 — pub/sub 분리 + 콜백/타이머)

- `project1`, `project2`: 퍼블리셔(`pub.cpp`)와 서브스크라이버(`sub.cpp`)를 완전히 분리한 노드 쌍
- 퍼블리셔는 `create_wall_timer(100ms, ...)`로 100ms마다 `"Hello World!!"` 문자열을 발행하고, 콜백 함수를 `std::bind`로 노드와 퍼블리셔를 바인딩해서 구성
- 서브스크라이버는 `create_subscription`으로 같은 토픽(`topic_pub1`)을 구독하고, 수신할 때마다 콜백에서 메시지를 로그로 출력


## 실습 목록

| 실습 | 내용 |
|---|---|
| [rclcpp/project1](./rclcpp/project1/) | Int32 퍼블리셔 기본형 — WallRate 1Hz로 카운트 값 발행 |
| [rclcpp/project2](./rclcpp/project2/) | Int32 퍼블리셔 변형 — QoS(KeepLast 10) 적용 확인 |
| [rclcpp/project3](./rclcpp/project3/) | Int32 퍼블리셔 변형 — 실행 결과(콘솔 로그) 스크린샷 포함 |
| [rclcpp2/project1](./rclcpp2/project1/) | 발행 주파수(Hz) 측정을 포함한 발전된 퍼블리셔/구독 실습 |
| [rclcpp2/project2](./rclcpp2/project2/) | 발행 주파수(Hz) 측정을 포함한 발전된 퍼블리셔/구독 실습 |
| [rclcpp2/project3](./rclcpp2/project3/) | 발행 주파수(Hz) 측정을 포함한 발전된 퍼블리셔/구독 실습 |
| [rclcpp3/project1](./rclcpp3/project1/) | 타이머 콜백(100ms) 기반 pub.cpp + sub.cpp 쌍 — std::bind 콜백 바인딩 |
| [rclcpp3/project2](./rclcpp3/project2/) | 타이머 콜백(100ms) 기반 pub.cpp + sub.cpp 쌍 — std::bind 콜백 바인딩 |

각 프로젝트 폴더에는 소스 코드와 `CMakeLists.txt`, 실행 결과 스크린샷(`images/`)이 포함되어 있습니다.
