# 03~05장. ROS2의 특징, DDS, 패키지와 노드 — 공부하기

세 챕터를 묶은 과제라서 배경지식도 세 갈래로 나눠서 정리합니다. 이 문서 하나로 3장(ROS2 특징), 4장(DDS), 5장(turtlesim 실습)을 모두 준비할 수 있습니다.

## 목차

- [3장. ROS1과 ROS2, 무엇이 다른가](#3장-ros1과-ros2-무엇이-다른가)
- [4장. DDS 동작 원리](#4장-dds-동작-원리)
- [5장. turtlesim으로 패키지/노드-실습](#5장-turtlesim으로-패키지노드-실습)
- [스스로 확인하는 질문](#스스로-확인하는-질문)

## 3장. ROS1과 ROS2, 무엇이 다른가

**통신 구조**

- ROS1(TCPROS + Master): `roscore`라는 중앙 서버(Master)가 반드시 떠 있어야 합니다. 모든 노드는 Master에 등록하고, Master가 통신할 노드들을 짝지어줍니다. 문제는 Master가 죽으면 새 노드가 기존 노드를 찾을 수 없다는 것(단일 장애점, Single Point of Failure)입니다.
- ROS2(DDS + 분산형): Master가 없습니다. DDS가 네트워크에 브로드캐스트로 "나 여기 있다"는 신호를 주기적으로 보내고, 다른 노드들이 자동으로 서로를 찾습니다(Dynamic Discovery). 노드 하나가 죽어도 나머지 통신에는 영향이 없습니다.

**실시간 시스템(Real-Time System)**

- **Hard Real-Time**: 마감을 못 지키면 시스템 실패로 간주 (예: 자동차 에어백)
- **Soft Real-Time**: 가끔 늦어도 성능 저하 정도로 넘어감 (예: 동영상 스트리밍)
- DDS는 QoS 설정으로 이런 실시간성 요구사항을 어느 정도 조절할 수 있게 설계되었습니다.

**라이선스(GPL vs Apache 2.0)**

- GPL: 이 코드를 가져다 쓴 파생 소프트웨어도 반드시 소스코드를 공개해야 하는 강한 카피레프트 라이선스
- Apache 2.0: 소스코드 공개 의무 없이 상업적으로도 자유롭게 사용·수정 가능
- ROS2가 기업 참여가 더 쉬운 Apache 2.0 계열을 채택한 것도 확산에 영향을 준 요인 중 하나입니다.

## 4장. DDS 동작 원리

DDS는 RTPS(Real-Time Publish-Subscribe) 프로토콜 기반으로 동작하는 미들웨어 표준입니다.

- **동적 검색(Dynamic Discovery)**: 별도의 브로커(중개 서버) 없이, 새 노드가 나타나면 자동으로 기존 노드들과 서로의 존재를 알게 됩니다.
- **브로커 방식과의 차이**: MQTT 같은 다른 pub/sub 프로토콜은 브로커가 모든 메시지를 중계하는 중앙집중형 구조인데, DDS는 브로커가 다운돼도 나머지 통신에 영향이 없어 실시간성이 중요한 로봇 제어에 채택되었습니다.
- **QoS(Quality of Service)**: 통신의 신뢰성, 지연, 버퍼 크기 등을 세밀하게 조절하는 설정.
  - `best_effort` — 전달 보장 (예: 관기분특 이룼래해서 처럼 구부하기)
  - `reliable` — 전달 보장 (예: 센서 값처럼 무조건 다 받아야 하는 경우)
  - 이 개념은 18~20장 코드의 `rclcpp::QoS(rclcpp::KeepLast(10))`에서 실제로 등장합니다.

## 5장. turtlesim으로 패키지/노드 실습

turtlesim은 ROS2에 기본 포함된 거북이 시뮬레이터로, 토픽/노드 개념을 눈으로 직접 보기 위한 교육용 도구입니다.

```
ros2 pkg list                          # 설치된 전체 패키지 목록
ros2 pkg executables turtlesim         # turtlesim 패키지 안의 실행 파일 목록
ros2 run turtlesim turtlesim_node      # 거북이 시뮬레이터 창 실행
ros2 run turtlesim turtle_teleop_key   # 키보드로 거북이를 조종하는 노드 실행
```

`turtlesim_node`와 `turtle_teleop_key`는 서로 다른 두 개의 노드입니다. `turtle_teleop_key`가 방향키 입력을 받아 속도 값을 토픽으로 발행하면, `turtlesim_node`가 이를 구독해 화면 속 거북이를 움직입니다. "입력을 처리하는 노드"와 "실제 동작(시각화)을 하는 노드"가 분리되어 있다는 것이 핵심이며, 이 구조는 24장(다이내믹셀), 25장(라인검출)에서도 반복됩니다.

```
ros2 topic list      # 현재 실행 중인 토픽 목록
ros2 service list    # 현재 실행 중인 서비스 목록
ros2 action list     # 현재 실행 중인 액션 목록
ros2 node list       # 현재 실행 중인 노드 목록
ros2 node info /turtlesim   # 특정 노드가 어떤 토픽/서비스/액션을 갖고 있는지 상세 조회
```

`rqt_graph`는 현재 실행 중인 노드와 토픽의 연결 관계를 그래프로 보여주는 시각화 도구입니다. 실행하면 `/teleop_turtle` → `/turtlesim` 화살표가 나오는데, 이 화살표가 토픽을 통한 pub/sub 연결입니다.

거북이로 8자를 그리는 실습은 `turtle_teleop_key`가 방향키뿐 아니라 G·B·V·C·D·E·R·T 같은 문자 키도 인식해서 속도/회전 값을 다르게 발행하도록 만들어져 있기 때문에 가능합니다. `ros2 topic echo /turtle1/cmd_vel`로 실제 발행 값을 같이 관찰하면 이해가 빠릅니다.

## 스스로 확인하는 질문

- ROS1과 ROS2 중 어느 쪽이 중앙 서버(Master) 없이 동작하는가, 그 이유는?
- DDS가 브로커 방식(MQTT)과 다른 점은 무엇인가?
- `turtlesim_node`와 `turtle_teleop_key`가 왜 별도의 노드로 분리되어 있는가?
