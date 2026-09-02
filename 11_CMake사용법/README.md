# 11장. CMake 사용법

> 📘 **배경 지식 정리**: 이 실습과 관련된 개념 설명은 [공부하기 문서](../공부하기/11_CMake사용법/README.md)에서 확인할 수 있습니다.

C++ 프로젝트의 빌드 시스템인 CMake의 동작 원리를 익히고, 직접 프로젝트를 구성해 빌드까지 해본 실습입니다.

## 11-1. CMake 기초

- CMake(설계자)와 GNU Make(작업자)의 역할 차이, `CMakeLists.txt`와 `CMakeCache.txt`의 역할
- Configure → Generate → Build로 이어지는 CMake의 3단계 빌드 과정
- Hello World 예제를 `cmake --build build` / `make`로 직접 빌드·실행
- 실습과제로 `adder` 프로젝트(정수 2개를 더해 출력하는 프로그램)를 `src/CMakeLists.txt`, `src/main.cpp` 구조로 작성하고 빌드해 결과(`a+b=30`) 확인

## 11-2. CMake + OpenCV

- 레나(Lenna) 이미지를 그레이스케일 / 이진 영상으로 변환하는 OpenCV C++ 프로그램을 작성하고 CMake로 빌드
- `CMakeLists.txt`의 각 항목(`cmake_minimum_required`, `project()`, `find_package(OpenCV REQUIRED)`, `add_executable`, `target_link_libraries` 등) 역할을 하나씩 정리
- 실행 결과로 원본/그레이스케일/이진화 3개의 창이 뜨는 것을 확인

## 자료

- [11_CMake사용법-1.pdf](./11_CMake사용법-1.pdf)
- [11_CMake사용법-2.pdf](./11_CMake사용법-2.pdf)
