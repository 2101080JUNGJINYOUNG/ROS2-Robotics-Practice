# 3~5장. ROS2의 특징, DDS, 패키지와 노드

⬆ [ROS2-Robotics-Practice로 돌아가기](https://github.com/2101080JUNGJINYOUNG/ROS2-Robotics-Practice)  ·  📘 [공부하기](../공부하기/03_04_05_ROS2특징과DDS/README.md)

세 개 챕터를 묶은 과제로, ROS1과 ROS2의 구조적 차이, DDS 미들웨어, 그리고 실제 turtlesim 패키지 실습을 다룹니다.

## 3장 — ROS2의 특징

- ROS1은 Master가 모든 노드를 중앙집중 관리하는 방식(TCPROS)이었지만, ROS2는 DDS 기반의 분산형 통신 방식으로 전환되어 Master 없이도 노드들이 서로를 찾아 통신
- 실시간 시스템의 Hard/Soft 구분, GPL과 Apache 2.0 라이선스 비교, 멀티프로세싱/분산컴퓨팅/네트워크통신 개념 정리

## 4장 — DDS(Data Distribution Service)

- DDS는 RTPS 프로토콜 기반으로 동작하며, 별도의 브로커 없이 노드들이 서로를 자동으로 찾는 동적 검색(Dynamic Discovery) 기능이 핵심
- MQTT처럼 브로커를 중개로 두는 방식과 달리, DDS는 P2P 방식이라 지연이 적고 실시간성이 중요한 로봇 제어에 적합해서 ROS2가 이를 채택

## 5장 — ROS2 패키지와 노드 (turtlesim 실습)

- `ros2 pkg list`, `ros2 pkg executables turtlesim`으로 패키지/실행파일 확인
- `ros2 run turtlesim turtlesim_node`로 거북이 시뮬레이터 실행, `ros2 run turtlesim turtle_teleop_key`로 방향키 조작
- `ros2 topic/service/action/node list`로 현재 실행 중인 통신 자원 확인
- `rqt_graph`로 `/turtlesim` ↔ `/teleop_turtle` 노드-토픽 연결을 시각적으로 확인
- `ros2 node info <노드명>`으로 각 노드의 Publisher/Subscriber/Service/Action 정보 조회
- 화살표 키(전후진·좌우회전)와 회전용 문자 키(G/B/V/C/D/E/R/T) 중 2개를 동시에 눌러 거북이로 8자 그리기 실습

## 자료

- [03_04_05장_과제.pdf](./03_04_05장_과제.pdf) — 과제 원본
