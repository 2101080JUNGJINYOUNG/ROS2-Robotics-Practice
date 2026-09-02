# 2장. ROS2 개발환경 구축

⬆ [ROS2-Robotics-Practice로 돌아가기](../README.md)  ·  📘 [공부하기](../공부하기/02_ROS2개발환경구축/README.md)

ROS2(Foxy) 개발 환경을 실제로 구성하고 동작을 확인한 실습입니다.

## 실습 내용

- `source /opt/ros/foxy/setup.bash`로 ROS2 환경을 로드하고 `ros2 --help`, `colcon --help`로 기본 명령어 체계 확인
- `ros2 run demo_nodes_cpp talker` / `ros2 run demo_nodes_py listener`를 각각 실행해 Hello World 메시지가 노드 간에 송수신되는 것을 확인 (ROS2 통신의 가장 기본적인 동작 검증)
- `mkdir -p ~/ros2_ws/src` 로 워크스페이스를 만들고 `colcon build --symlink-install` 실행 → `build/ install/ log/ src/` 폴더가 생성되는 것을 확인
- `~/.bashrc`에 `ROS_DOMAIN_ID`, `ROS_NAMESPACE`(실습에서는 `jetson0`으로 지정), `RMW_IMPLEMENTATION` 같은 환경변수를 추가하고 적용까지 확인

## 배운 점

ROS2는 노드를 실행하기 전에 반드시 환경을 `source`해야 하고, 워크스페이스 단위로 빌드 산출물(`build/install/log`)이 분리되어 관리된다는 것, 그리고 `ROS_DOMAIN_ID`/`RMW_IMPLEMENTATION` 같은 환경변수로 통신 범위와 DDS 구현체를 제어할 수 있다는 것을 확인했습니다.

## 자료

- [02_ROS2개발환경구축-2.pdf](./02_ROS2개발환경구축-2.pdf) — 실습 원본
