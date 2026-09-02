# 18~20장. ROS2 rclcpp (C++ 클라이언트 라이브러리)

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

## 폴더 구조

- [rclcpp/](./rclcpp/), [rclcpp2/](./rclcpp2/), [rclcpp3/](./rclcpp3/) — 각 프로젝트 폴더에 소스 코드와 `CMakeLists.txt` 포함

> 원본 README에는 실행 결과 스크린샷이 포함되어 있었으나, 이미지가 다른 GitHub 계정(`user-attachments`)에 업로드되어 있어 이 저장소로 직접 가져오지 못했습니다. 코드와 텍스트 설명은 모두 그대로 보존했습니다.
