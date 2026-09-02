# 24장. ROS2 + Dynamixel(다이내믹셀) 제어

⬆ [ROS2-Robotics-Practice로 돌아가기](https://github.com/2101080JUNGJINYOUNG/ROS2-Robotics-Practice)  ·  📘 [공부하기](../공부하기/24_ROS2다이내믹셀/README.md)


Dynamixel 서보 모터를 ROS2 노드로 직접 제어해본 실습입니다. 두 차례에 걸쳐 진행되었습니다.

## 다이내믹셀1/ — 기본 제어

- `dxl.hpp` / `dxl.cpp`: Dynamixel SDK를 감싼 제어 클래스 (포지션/속도 제어, 통신 초기화 등)
- `main.cpp`: 이 클래스를 이용해 실제 모터를 구동하는 진입점

## 다이내믹셀2/ — 키보드 원격 조작(Teleoperation)

- `jetson/pub.cpp`: GStreamer로 Jetson 카메라 영상을 읽어 `CompressedImage` 토픽으로 발행
- `jetson/sub.cpp`: 키보드 속도 명령(`geometry_msgs/Vector3`) 토픽을 구독해 `dxl.setVelocity()`로 실제 Dynamixel 모터를 구동
- `우분투/pub.cpp`: 키보드 입력(f/b/l/r/s)을 받아 가감속 로직을 적용한 속도 값을 `topic_dxlpub` 토픽으로 발행 — 실제 모터 구동은 이 키보드 입력에서 시작됨
- `우분투/sub.cpp`: Jetson이 발행한 카메라 영상을 구독해 화면에 표시하고 `save.mp4`로 녹화 (영상 처리나 모터 제어와는 별개의 단순 표시/녹화 노드)
- 즉 카메라 영상은 모니터링·녹화용으로만 쓰였고, 모터 구동은 키보드 입력 → 속도 토픽 발행 → 구독 노드의 `setVelocity()` 호출로 이어지는 원격 조작(teleoperation) 구조입니다.
- 실습 데모 영상은 추후 유튜브에 업로드되는 대로 이 섹션에 링크가 추가될 예정입니다.


## 실습 목록

| 실습 | 내용 |
|---|---|
| [다이내믹셀1](./다이내믹셀1/) | Dynamixel SDK 래핑 제어 클래스(`dxl.hpp/cpp`)로 포지션/속도 기본 제어, `main.cpp`에서 모터 구동 |
| [다이내믹셀2](./다이내믹셀2/) | 카메라 영상 발행(Jetson) + 키보드 입력 기반 속도 토픽 발행/구독으로 모터 원격 조작(teleoperation) |
