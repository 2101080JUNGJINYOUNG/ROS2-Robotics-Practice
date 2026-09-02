# 코드 간략 설명

## sub (subscribe)

라인 트레이서 실험에서 **pub 노드가 퍼블리시한 압축 영상 토픽**을 구독해서,  
영상 처리(ROI 추출, 이진화, 라인 중심 검출)를 수행하고,  
결과를 OpenCV 창에 시각화하면서 **라인 중심 위치 에러(error)를 계산**하는 노드입니다.  

---

### vision.hpp

### 파일 역할 (Role)
- `CamSubNode` 클래스를 선언하는 **헤더 파일**  
- `sensor_msgs::msg::CompressedImage` 타입 토픽을 **구독(subscribe)** 하는 ROS2 노드 인터페이스 정의
- 콜백 함수 `mysub_callback()` 에서:
  - 압축 이미지를 OpenCV `Mat`으로 디코딩
  - ROI(하단 부)만 사용해서 이진화
  - 라인 중심을 찾고, 이전 프레임과 연결해 추적
  - 화면에 디버깅용 윈도우 3개를 띄우고, 에러 및 처리 시간을 로그로 출력

---

```cpp
class CamSubNode : public rclcpp::Node
{
public:
    CamSubNode();
    ~CamSubNode();

private:
    void mysub_callback(const sensor_msgs::msg::CompressedImage::SharedPtr msg);
    rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr subscription_;
};
```
- CamSubNode()  
  압축 이미지 토픽 구독 설정과 OpenCV 디버그 창 생성을 포함해 subscriber node를 초기화하는 기본 생성자이다.

- mysub_callback(const sensor_msgs::msg::CompressedImage::SharedPtr msg)  
  수신된 압축 이미지 메시지를 디코딩해 라인 검출과 에러 계산 및 화면 표시를 수행하는 콜백 함수이다.

- subscription_  
  지정된 토픽에서 sensor_msgs::msg::CompressedImage 메시지를 구독하여 mysub_callback()으로 전달하는 ROS2 구독 객체이다.

### cam_pub_node.cpp

#### 1. CamPubNode 생성자

```cpp
CamSubNode::CamSubNode() : Node("camsub_wsl")
{
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort();

    subscription_ = this->create_subscription<sensor_msgs::msg::CompressedImage>(
        "image/compressed_7", qos_profile, bind(&CamSubNode::mysub_callback, this, _1));


    namedWindow("Original", WINDOW_AUTOSIZE);
    namedWindow("Grayscale", WINDOW_AUTOSIZE);
    namedWindow("Binary with Bounding Box", WINDOW_AUTOSIZE);
}
```
- 이 생성자는 노드 이름을 "camsub_wsl"로 설정하고, image/compressed_7 토픽을 best-effort QoS로 구독하는 subscription_을 만든 뒤, 원본·그레이스케일·이진화 결과를 확인하기 위한 세 개의 OpenCV 디버그 창을 미리 생성하는 역할을 한다.

#### 2. 
```cpp
void CamSubNode::mysub_callback(const sensor_msgs::msg::CompressedImage::SharedPtr msg)
{
    struct timeval start, end1;
    gettimeofday(&start, NULL);

    Mat frame = imdecode(Mat(msg->data), IMREAD_COLOR);     // msg->data를 컬러 이미지로 디코딩
    if (frame.empty()) {
        RCLCPP_WARN(this->get_logger(), "frame empty!!!");
        return;
    }

    Mat gray, binary, resizedBinary;
    cvtColor(frame, gray, COLOR_BGR2GRAY);

    Rect roi(0, gray.rows * 3 / 4, gray.cols, gray.rows / 4);
    resizedBinary = gray(roi);

    threshold(resizedBinary, binary, 0, 255, THRESH_BINARY | THRESH_OTSU);

    Mat labelImage, stats, centroids;
    int nLabels = connectedComponentsWithStats(binary, labelImage, stats, centroids, 8, CV_32S);

    Mat colorBinary;
    cvtColor(binary, colorBinary, COLOR_GRAY2BGR);

    vector<Point> lineCenters;
    for (int i = 1; i < nLabels; i++) {
        int x = stats.at<int>(i, CC_STAT_LEFT);
        int y = stats.at<int>(i, CC_STAT_TOP);
        int width = stats.at<int>(i, CC_STAT_WIDTH);
        int height = stats.at<int>(i, CC_STAT_HEIGHT);

        Point center(centroids.at<double>(i, 0), centroids.at<double>(i, 1));
        lineCenters.push_back(center);

        rectangle(colorBinary, Rect(x, y, width, height), Scalar(255, 0, 0), 2);
        circle(colorBinary, center, 5, Scalar(255, 0, 0), -1);
    }

    static Point previousCenter(-1, -1);
    static bool firstFrame = true;
    const double MAX_DISTANCE = 50.0;
    double error = 0.0;

    if (firstFrame && !lineCenters.empty()) {
        Point centerOfImage(frame.cols / 2, frame.rows / 2);
        double minDistance = DBL_MAX;
        Point closestCenter;        // 가장 가까운 라인의 중심점을 저장

        for (size_t i = 0; i < lineCenters.size(); i++) {
            double distance = norm(lineCenters[i] - centerOfImage);
            if (distance < minDistance) {
                minDistance = distance;
                closestCenter = lineCenters[i];
            }
        }
        rectangle(colorBinary, Rect(closestCenter.x - 10, closestCenter.y - 10, 20, 20), Scalar(0, 0, 255), 2);
        circle(colorBinary, closestCenter, 5, Scalar(0, 0, 255), -1);

        error = centerOfImage.x - closestCenter.x;
        previousCenter = closestCenter;
        firstFrame = false;
    }
    else if (previousCenter.x != -1 && !lineCenters.empty()) {
        double minDistance = DBL_MAX;
        Point closestCenter;

        for (size_t i = 0; i < lineCenters.size(); i++) {
            double distance = norm(lineCenters[i] - previousCenter);
            if (distance < minDistance && distance < MAX_DISTANCE) {
                minDistance = distance;
                closestCenter = lineCenters[i];
            }
        }

        if (minDistance < MAX_DISTANCE) {
            rectangle(colorBinary, Rect(closestCenter.x - 10, closestCenter.y - 10, 20, 20), Scalar(0, 0, 255), 2);
            circle(colorBinary, closestCenter, 5, Scalar(0, 0, 255), -1);
            previousCenter = closestCenter;
        } 
        else {
            rectangle(colorBinary, Rect(previousCenter.x - 10, previousCenter.y - 10, 20, 20), Scalar(0, 0, 255), 2);
            circle(colorBinary, previousCenter, 5, Scalar(0, 0, 255), -1);
        }

        Point centerOfImage(frame.cols / 2, frame.rows / 2);
        error = centerOfImage.x - closestCenter.x;
    }


    imshow("Original", frame);
    imshow("Grayscale", gray);
    imshow("Binary with Bounding Box", colorBinary);

    waitKey(1);

    gettimeofday(&end1, NULL);
    double elapsedMs = (end1.tv_sec - start.tv_sec) * 1000.0 + (end1.tv_usec - start.tv_usec) / 1000.0;
    const int targetDelayMs = 30;
    int sleepMs = targetDelayMs - static_cast<int>(elapsedMs);
    if (sleepMs > 0) {
        usleep(sleepMs * 1000);
    }
    double totalTime = elapsedMs + (sleepMs > 0 ? sleepMs : 0);

    RCLCPP_INFO(this->get_logger(), "수신된 이미지: %s, %d x %d, 에러: %.2f, 시간: %.2f ms",
                msg->format.c_str(), frame.rows, frame.cols, error, totalTime);

    if (ctrl_c_pressed) {
        rclcpp::shutdown();
    }
}
```
- 이 콜백 함수는 수신한 압축 이미지를 디코딩한 뒤 하단 ROI를 이진화하여 연결 요소로 라인 중심들을 찾고, 이전 프레임과 가장 가까운 중심을 선택해 영상 중앙 기준 가로 방향 에러를 계산하며, 디버깅용 화면(원본·그레이스케일·이진+박스)을 출력하고, 목표 주기(약 30ms)에 맞춰 처리 시간과 에러를 로그로 남기다가 Ctrl+C 신호가 감지되면 노드를 종료하는 역할을 한다.
