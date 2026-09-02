# 25장. ROS2 라인 검출 (Line Tracer)

카메라 영상을 이용해 라인을 검출하고 그에 맞춰 주행하는 라인 트레이서 실습입니다. 영상을 ROS2 토픽으로 발행하는 `pubpub` 노드와, 그 영상을 받아 라인을 검출·처리하는 `subsub` 노드로 구성되어 있습니다.

## pubpub/ — 영상 퍼블리셔

- `cam_pub_node.hpp`/`.cpp`: OpenCV `VideoCapture`로 영상 소스(파일 또는 Jetson 카메라의 GStreamer 파이프라인)를 읽어 `sensor_msgs::msg::CompressedImage`로 약 30FPS(33ms 주기)에 맞춰 압축 발행하는 노드
- 노드 이름, 토픽 이름, 영상 소스 경로를 모두 생성자 인자로 받아 재사용 가능하도록 설계

## subsub/ — 라인 검출/처리

- `vision.hpp`/`.cpp`, `main.cpp`: `pubpub`이 발행한 압축 이미지 토픽을 구독해 라인 검출 로직을 수행하는 노드

## liner_추가예제/ — 보조 예제

- 별도의 `liner` 패키지(`pub.cpp`) — 라인 트레이싱 관련 보조 퍼블리셔 예제

## 데모 영상

- [In Line Tracer (7_lt_ccw_100rpm)](https://www.youtube.com/watch?v=McKT8FgOp5I)
- [Out Line Tracer (5_lt_cw_100rpm)](https://youtu.be/BwHTx4GQWgU)

> 원본 README 전문은 [원본_README.md](./원본_README.md)에 그대로 보존했습니다.
