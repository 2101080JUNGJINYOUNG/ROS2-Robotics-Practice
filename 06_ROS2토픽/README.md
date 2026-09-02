# 6장. ROS2 토픽

`geometry_msgs/Twist` 메시지와 `ros2 topic` 명령어들을 이용해 turtlesim을 직접 제어하며 토픽 통신을 실습했습니다.

## 실습 내용

- `ros2 topic echo /turtle1/cmd_vel`로 Twist 메시지의 6개 값(linear.x/y/z, angular.x/y/z)이 실제로 어떤 의미인지 확인 — linear는 전후 이동 속도, angular.z는 회전 속도
- `ros2 topic bw`로 토픽의 초당 대역폭(예: 42 B/s, 평균/최소/최대 크기) 확인
- `ros2 topic hz`로 토픽의 발행 주기(평균 rate, 최소/최대, 표준편차) 확인
- `ros2 node list`, `ros2 node info`, `ros2 topic list`, `ros2 topic list -t`(타입까지 포함)로 노드·토픽 정보를 다각도로 조회
- `ros2 topic pub --rate 1 /turtle1/cmd_vel geometry_msgs/msg/Twist "{linear: {x: 2.0, ...}, angular: {..., z: 1.8}}"` 명령으로 거북이가 원을 그리며 도는 것을 직접 확인

## 배운 점

토픽은 발행자(Publisher)와 구독자(Subscriber)가 서로를 몰라도 되는 비동기 1:다 통신이며, `ros2 topic pub`처럼 커맨드라인에서도 직접 메시지를 발행해 노드를 제어할 수 있다는 것을 실습으로 확인했습니다.

## 자료

- [06_ROS2토픽.pdf](./06_ROS2토픽.pdf) — 실습 원본
