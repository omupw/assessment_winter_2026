#include <iostream>
#include <opencv2/opencv.hpp>

//g++ -o bin/repairer src/main.cpp `pkg-config --cflags --libs opencv4`
int main() {
    cv::Mat image = cv::imread("../imgs/noisy.jpg");
    cv::Mat repairedImage;
    // 应用中值滤波
    cv::medianBlur(image, repairedImage, 5);

    cv::imshow("Repaired Image", repairedImage);
    cv::waitKey(0);
    // 保存处理后的图像
    cv::imwrite("../imgs/repaired.jpg", repairedImage);

    return 0;
}