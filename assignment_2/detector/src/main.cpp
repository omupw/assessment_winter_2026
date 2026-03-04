#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

int main() {
    //导入图片并转换为hsv格式
    cv::Mat image = cv::imread("../imgs/test1.jpg");
    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

    //按阈值创建mask
    cv::Mat mask;
    cv::Scalar lower_green = cv::Scalar(35,50,50);
    cv::Scalar upper_green = cv::Scalar(85,255,255);
    cv::inRange(hsvImage, lower_green, upper_green, mask);

    //形态学去噪点
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

    //找轮廓
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask, contours, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);

    //画圆
    cv::Mat result = image.clone();
    cv::Point2f center;
    float radius;
    for (int i = 0; i < contours.size(); i++) {
        cv::minEnclosingCircle(contours[i], center, radius);
        cv::circle(result, center, radius, cv::Scalar(0, 255, 0),2,cv::LINE_AA);
    }
    cv::imshow("result", result);
    cv::waitKey(0);
    cv::destroyWindow("result");
    cv::imwrite("../imgs/test1_result.jpg", result);
    return 0;
}