# 25장. ROS2 라인 검출 (Line Tracer)

⬆ [ROS2-Robotics-Practice로 돌아가기](https://github.com/2101080JUNGJINYOUNG/ROS2-Robotics-Practice)  ·  📘 [공부하기](../공부하기/25_ROS2라인검출/README.md)


카메라 영상을 이용해 라인을 검출하고 그에 맞춰 주행하는 라인 트레이서 실습입니다. 영상을 ROS2 토픽으로 발행하는 `pubpub` 노드와, 그 영상을 받아 라인을 검출·처리하는 `subsub` 노드로 구성되어 있습니다.


## 실습 목록

| 실습 | 내용 |
|---|---|
| [pubpub](./pubpub/) | 영상 퍼블리셔 — OpenCV VideoCapture로 영상 소스를 읽어 CompressedImage로 약 30FPS 압축 발행 |
| [subsub](./subsub/) | 라인 검출/처리 — pubpub이 발행한 압축 이미지를 구독해 ROI 추출·이진화·라인 중심 검출 및 에러 계산 |
| [liner_추가예제](./liner_추가예제/) | 보조 예제 — 별도 `liner` 패키지의 라인 트레이싱 보조 퍼블리셔 |

## 데모 영상

- [In Line Tracer (7_lt_ccw_100rpm)](https://www.youtube.com/watch?v=McKT8FgOp5I)
- [Out Line Tracer (5_lt_cw_100rpm)](https://youtu.be/BwHTx4GQWgU)

> 원본 README 전문은 [원본_README.md](./원본_README.md)에 그대로 보존했습니다.
