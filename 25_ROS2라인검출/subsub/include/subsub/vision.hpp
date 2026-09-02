#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/compressed_image.hpp"
#include "opencv2/opencv.hpp"
#include <memory>
#include <functional>

using namespace cv;
using namespace std;

class CamSubNode : public rclcpp::Node
{
public:
    CamSubNode();
    ~CamSubNode();

private:
    void mysub_callback(const sensor_msgs::msg::CompressedImage::SharedPtr msg);
    rclcpp::Subscription<sensor_msgs::msg::CompressedImage>::SharedPtr subscription_;
};