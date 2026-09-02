# 11장. CMake 사용법 — 공부하기

ROS2의 C++ 패키지는 내부적으로 전부 CMake로 빌드됩니다. 이 챕터에서 CMake의 동작 원리를 확실히 잡아두면, 18~20장 이후 모든 실습 폴더의 `CMakeLists.txt`를 스스로 읽고 수정할 수 있게 됩니다.

## 목차

- [1. CMake와 Make의 관계](#1-cmake와-make의-관계--설계자와-작업자)
- [2. CMake의 3단계 빌드 과정](#2-cmake의-3단계-빌드-과정)
- [3. CMakeLists.txt 기본 문법](#3-cmakeliststxt-기본-문법)
- [4. 외부 라이브러리(OpenCV) 쓰기](#4-외부-라이브러리opencv-쓰기-11-2-실습)
- [5. ROS2 패키지의 CMakeLists.txt](#5-ros2-패키지의-cmakeliststxt는-무엇이-다른가)
- [6. 스스로 확인하는 질문](#6-스스로-확인하는-질문)

## 1. CMake와 Make의 관계 — "설계자"와 "작업자"

C++ 코드를 실행 파일로 만들려면 컴파일과 링크가 필요한데, 파일이 여러 개일수록 "어떤 파일을 어떤 순서로, 어떤 옵션으로 컴파일할지" 관리가 복잡해집니다.

- `Make`(GNU Make)는 컴파일 규칙을 `Makefile`에 정의해두면 그대로 실행해주는 도구입니다. 문제는 운영체제·컴파일러가 다르면 `Makefile`을 매번 새로 써야 한다는 것입니다.
- `CMake`는 이 문제를 해결하는 상위 도구입니다. 사람이 이해하기 쉬운 `CMakeLists.txt`를 한 번만 작성해두면, CMake가 현재 시스템에 맞는 `Makefile`을 자동 생성해줍니다.

비유하면 CMake는 "이런 프로그램을 만들어줘"라고 지시하는 **설계자**이고, Make는 그 지시서를 보고 실제 컴파일 명령을 실행하는 **작업자**입니다.

## 2. CMake의 3단계 빌드 과정

1. **Configure(구성)**: `CMakeLists.txt`를 읽어 시스템 환경(컴파일러, 라이브러리 위치)을 확인하고, 결과를 `CMakeCache.txt`에 저장
2. **Generate(생성)**: Configure 결과를 바탕으로 실제 빌드 파일(리눅스에서는 보통 `Makefile`) 생성
3. **Build(빌드)**: 생성된 `Makefile`을 실행해 컴파일/링크 수행

```
mkdir build && cd build
cmake ..              # 1) Configure + 2) Generate
cmake --build .       # 3) Build (= make 와 동일한 효과)
```

`CMakeCache.txt`가 꼬이면 `build/` 폴더를 통째로 지우고 처음부터 다시 `cmake ..`를 실행하는 것이 가장 확실한 해결법입니다.

## 3. `CMakeLists.txt` 기본 문법

```cmake
cmake_minimum_required(VERSION 3.5)   # CMake 최소 요구 버전 지정
project(adder)                        # 프로젝트 이름 지정

add_executable(adder main.cpp)        # main.cpp를 컴파일해서 adder 실행 파일 생성
```

`adder` 프로젝트(정수 두 개를 더해 출력)처럼 단순한 프로그램은 이 세 줄만으로 충분합니다.

## 4. 외부 라이브러리(OpenCV) 쓰기 (11-2 실습)

```cmake
cmake_minimum_required(VERSION 3.5)
project(image_processing)

find_package(OpenCV REQUIRED)                       # 시스템에 설치된 OpenCV를 찾음
add_executable(image_processing main.cpp)           # 실행 파일 생성
target_link_libraries(image_processing ${OpenCV_LIBS})  # OpenCV 라이브러리 연결(링크)
```

- `find_package(OpenCV REQUIRED)` — OpenCV가 설치되어 있는지 찾고, 헤더/라이브러리 경로를 `OpenCV_LIBS` 등 변수에 채워 넣습니다. `REQUIRED`는 "못 찾으면 빌드 실패"라는 뜻입니다.
- `add_executable(이름 소스파일...)` — 어떤 소스를 컴파일해서 어떤 이름의 실행 파일을 만들지 지정합니다.
- `target_link_libraries(...)` — 컴파일된 오브젝트 파일에 외부 라이브러리를 연결합니다. 링크를 안 하면 OpenCV 함수 호출 코드는 컴파일은 되어도 링크 단계에서 "함수를 찾을 수 없다" 에러가 납니다.

레나(Lenna) 이미지를 그레이스케일/이진 영상으로 바꾸는 프로그램이 창 3개(원본/그레이스케일/이진화)를 띄우는 것은, `cvtColor`(색공간 변환)와 `threshold`(이진화)를 순서대로 적용한 결과를 각각 `imshow`로 띄우기 때문입니다. 이 이진화 개념은 25장 라인검출에서 라인 검출의 핵심 원리로 그대로 재사용됩니다.

## 5. ROS2 패키지의 CMakeLists.txt는 무엇이 다른가

이 챕터는 순수 C++ 프로젝트를 다뤘지만, 13장부터는 ROS2 패키지의 `CMakeLists.txt`를 보게 됩니다. 기본 원리(Configure→Generate→Build, `find_package`, `add_executable`, `target_link_libraries`)는 동일하고, 차이는 `find_package(rclcpp REQUIRED)`처럼 ROS2 라이브러리를 찾거나 `ament_target_dependencies` 같은 ROS2 전용 헬퍼가 추가된다는 점뿐입니다.

## 6. 스스로 확인하는 질문

- CMake와 Make의 역할 차이를 한 문장으로 설명할 수 있는가?
- Configure → Generate → Build 세 단계가 각각 무슨 일을 하는가?
- `find_package`와 `target_link_libraries`는 각각 무엇을 하며, 둘 중 하나가 빠지면 어떤 에러가 나는가?
