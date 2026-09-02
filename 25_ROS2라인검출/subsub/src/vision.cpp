#include "subsub/vision.hpp"
#include <sys/time.h>
#include <unistd.h>

bool ctrl_c_pressed = false;
void ctrlc(int)
{
    ctrl_c_pressed = true;
}

using namespace std::placeholders;
using namespace std;
using namespace cv;

// 생성자 (구독자 생성, 발행자 생성)
CamSubNode::CamSubNode() : Node("camsub_wsl")
{
    auto qos_profile = rclcpp::QoS(rclcpp::KeepLast(10)).best_effort();

    subscription_ = this->create_subscription<sensor_msgs::msg::CompressedImage>(
        "image/compressed_7", qos_profile, bind(&CamSubNode::mysub_callback, this, _1));


    namedWindow("Original", WINDOW_AUTOSIZE);
    namedWindow("Grayscale", WINDOW_AUTOSIZE);
    namedWindow("Binary with Bounding Box", WINDOW_AUTOSIZE);
}

// 소멸자
CamSubNode::~CamSubNode()
{
    destroyAllWindows();
}

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
