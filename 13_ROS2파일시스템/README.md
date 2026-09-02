# 13장. ROS2 파일시스템

> 📘 **배경 지식 정리**: 이 실습과 관련된 개념 설명은 [공부하기 문서](../공부하기/13_ROS2파일시스템/README.md)에서 확인할 수 있습니다.

ROS2 패키지를 실제로 만들고 빌드하면서 파일시스템 구조를 익힌 실습입니다.

## 핵심 개념

- **빌드 시스템**(ament_cmake / ament_python)은 패키지 하나를 어떻게 빌드할지 정의하고, **빌드 툴**(colcon)은 워크스페이스 전체를 대상으로 빌드를 조율한다는 차이
- `ros2 pkg create`는 워크스페이스의 `src/` 안에서 실행: `ros2 pkg create <패키지명> --build-type <빌드타입> --dependencies <의존패키지>...`
- `colcon build`는 워크스페이스 루트(`~/ros2_ws`)에서 실행: `colcon build --symlink-install --packages-select <패키지명>`

## 실습 내용

- 실습과제로 `rclcpp`, `std_msgs`에 의존하는 `first_pkg` 패키지를 생성
- `tree -L 1`로 `CMakeLists.txt`, `include/`, `package.xml`, `src/`가 어떤 역할을 하는지 확인
- `colcon build` 실행 후 생성되는 `build/ install/ log/ src/` 폴더 각각의 역할 정리(빌드 중간 산출물 / 최종 설치 결과물 / 빌드 로그 / 원본 소스)

## 자료

- [13_ROS2파일시스템.pdf](./13_ROS2파일시스템.pdf) — 실습 원본
