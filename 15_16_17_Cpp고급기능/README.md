# 15~17장. C++ 고급 기능

ROS2 콜백 코드에서 자주 쓰이는 C++ 고급 문법(람다, `std::function`, `std::bind`, 스마트 포인터)을 익힌 과제 3개를 묶었습니다.

## 15장 — 람다식과 std::function

- 일반 함수 vs 람다식의 차이(익명성, 정의 위치, 캡처 기능)와 람다의 장점 정리
- 일반 함수 / 함수 객체(`operator()`) / 람다식을 모두 `std::function`이라는 하나의 타입으로 저장·호출하는 예제
- 함수 포인터보다 `std::function`이 더 안전하고 유연한 이유
- `std::function<int(int,int)>`을 매개변수로 받는 `calculate` 함수에 덧셈/뺄셈/곱셈 람다를 각각 전달하는 예제

## 16장 — std::bind와 시간 리터럴

- `std::bind` 선언의 `F&& f, Args&&... args`에서 `&&`가 의미하는 것(우측값 참조 + 전달 참조/perfect forwarding)
- `std::bind`로 `isGreaterThan` 함수의 두 번째 인자를 50으로 고정한 뒤 `std::count_if`에 전달해 벡터에서 50보다 큰 값의 개수를 세는 예제
- `using namespace std::chrono_literals;`를 이용한 `500ms`, `2s`, `1s + 250ms` 같은 시간 리터럴 계산

## 17장 — 스마트 포인터

- raw 포인터로 작성된 코드를 `unique_ptr` + `move`로 변환하는 실습
- 클래스 `CTest`를 `shared_ptr`(`make_shared`)로 변환해, 참조 카운트에 따라 생성자/소멸자가 호출되는 시점을 직접 확인

## 자료

- [15_Cpp고급기능-1.pdf](./15_Cpp고급기능-1.pdf)
- [16_Cpp고급기능-2.pdf](./16_Cpp고급기능-2.pdf)
- [17_Cpp고급기능-3.pdf](./17_Cpp고급기능-3.pdf)
