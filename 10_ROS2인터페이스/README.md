# 10장. ROS2 인터페이스

⬆ [ROS2-Robotics-Practice로 돌아가기](../)  ·  📘 [공부하기](../공부하기/10_ROS2인터페이스/)

메시지, 토픽, 서비스, 액션, 인터페이스라는 용어를 명확히 구분하고, 실제 인터페이스 구조를 조회해본 실습입니다.

## 용어 정리

- **인터페이스**: `.msg` / `.srv` / `.action` 파일로 정의되는, 데이터 구조의 "설계도"
- **메시지**: 토픽 통신에 쓰이는 가장 기본적인 데이터 단위
- **토픽**: 1:다 비동기 publish/subscribe 통신
- **서비스**: 1:1 동기 request/response 통신
- **액션**: 시간이 오래 걸리는 작업을 위한 비동기 통신 — goal(목표) / feedback(중간 진행상황) / result(최종 결과)로 구성

## 실습 내용

turtlesim과 teleop_turtle 노드를 실행한 뒤 `ros2 topic list -t`로 토픽 타입을 확인하고, `ros2 interface show` 명령으로 다음 인터페이스들의 실제 필드 구조를 하나씩 조회했습니다.

- `rcl_interfaces/msg/ParameterEvent`
- `rcl_interfaces/msg/Log`
- `geometry_msgs/msg/Twist`
- `turtlesim/msg/Color`
- `turtlesim/msg/Pose`

## 자료

- [10_ROS2인터페이스.pdf](./10_ROS2인터페이스.pdf) — 실습 원본
