#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

//g++ -o bin/detector src/main.cpp `pkg-config --cflags --libs opencv4`
int main() {
    //导入图片并转换为hsv格式
    cv::Mat image = cv::imread("../imgs/test2.jpg");
    cv::Mat hsvImage;
    cv::cvtColor(image, hsvImage, cv::COLOR_BGR2HSV);

    //按阈值创建mask
    cv::Mat mask;
    cv::Scalar lower_green = cv::Scalar(40,65,65);
    cv::Scalar upper_green = cv::Scalar(85,255,255);
    cv::inRange(hsvImage, lower_green, upper_green, mask);

    //形态学去噪点
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::morphologyEx(mask, mask, cv::MORPH_OPEN, kernel);
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, kernel);

    //分水岭算法
    cv::Mat dist;
    cv::distanceTransform(mask, dist, cv::DIST_L2, 5);
    double max_val;
    cv::minMaxLoc(dist, nullptr, &max_val, nullptr, nullptr);
    cv::Mat peaks;
    cv::threshold(dist, peaks, 0.7*max_val, 255, cv::THRESH_BINARY);
    peaks.convertTo(peaks, CV_8U);
    cv::Mat markers;
    cv::connectedComponents(peaks,markers,8,CV_32S);
    markers = markers + 1;
    cv::Mat unknown = mask -peaks;
    markers.setTo(0,unknown ==255);
    cv::watershed(image, markers);

    //找轮廓
    std::vector<std::vector<cv::Point>> contours;
    double min_label,max_label;
    cv::minMaxLoc(markers, &min_label, &max_label);
    for (int label = 2; label <= max_label; label++) {
        cv::Mat ball_mask = (markers == label);
        std::vector<std::vector<cv::Point>> contour;
        cv::findContours(ball_mask, contour, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        if (!contour.empty()) {
            for (const auto& ct : contour) {
                contours.push_back(ct);
            }
        }
    }

    //画圆
    cv::Mat result = image.clone();
    cv::Point2f center;
    float radius;
    for (int i = 0; i < contours.size(); i++) {
        cv::minEnclosingCircle(contours[i], center, radius);
        if (radius > 10) {
            cv::circle(result, center, radius, cv::Scalar(0, 255, 0),2,cv::LINE_AA);
        }
    }
    cv::imshow("result", result);
    cv::waitKey(0);
    cv::imshow("mask", mask);
    cv::waitKey(0);
    cv::destroyWindow("result");
    cv::imwrite("../imgs/test2_result.jpg", result);
    return 0;
}