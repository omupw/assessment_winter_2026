#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cassert>
#include "fun_adm.h"

class AdaptiveMedianFilterParallel : public cv::ParallelLoopBody {
private:
    const cv::Mat& src;
    const cv::Mat& padded;
    cv::Mat& dst;
    int maxWindowSize;
    int halfMax;
    int rows;
    int cols;

public:
    AdaptiveMedianFilterParallel(const cv::Mat& srcImg,
                                const cv::Mat& paddedImg,
                                cv::Mat& dstImg,
                                int maxWinSize)
        : src(srcImg), padded(paddedImg), dst(dstImg),
          maxWindowSize(maxWinSize), rows(srcImg.rows), cols(srcImg.cols) {
        halfMax = maxWinSize / 2;
    }

    virtual void operator()(const cv::Range& range) const override {
        // 预分配内存，避免每次循环重新分配
        std::vector<uchar> pixelsBuffer;
        pixelsBuffer.reserve(maxWindowSize * maxWindowSize);

        for (int i = range.start; i < range.end; ++i) {
            uchar* dst_row = dst.ptr<uchar>(i);

            for (int j = 0; j < cols; ++j) {
                int center_i = i + halfMax;
                int center_j = j + halfMax;
                uchar centerVal = padded.at<uchar>(center_i, center_j);

                int windowSize = 3;
                bool found = false;

                while (windowSize <= maxWindowSize && !found) {
                    int halfWin = windowSize / 2;

                    // 清空缓冲区
                    pixelsBuffer.clear();

                    // 收集窗口像素
                    for (int m = -halfWin; m <= halfWin; ++m) {
                        const uchar* padded_row = padded.ptr<uchar>(center_i + m);
                        for (int n = -halfWin; n <= halfWin; ++n) {
                            pixelsBuffer.push_back(padded_row[center_j + n]);
                        }
                    }

                    // 使用nth_element找中值
                    int midIndex = pixelsBuffer.size() / 2;
                    auto begin = pixelsBuffer.begin();
                    auto mid = begin + midIndex;
                    std::nth_element(begin, mid, pixelsBuffer.end());
                    uchar median = *mid;

                    // 找最小值和最大值
                    uchar minVal = *std::min_element(begin, pixelsBuffer.end());
                    uchar maxVal = *std::max_element(begin, pixelsBuffer.end());

                    // 阶段A
                    if (median > minVal && median < maxVal) {
                        // 阶段B
                        if (centerVal > minVal && centerVal < maxVal) {
                            dst_row[j] = centerVal;
                        } else {
                            dst_row[j] = median;
                        }
                        found = true;
                    } else {
                        windowSize += 2;
                    }
                }

                if (!found) {
                    dst_row[j] = centerVal;
                }
            }
        }
    }
};

/**
 * 并行自适应中值滤波（使用OpenCV并行框架）
 * @param src 输入图像
 * @param maxWindowSize 最大窗口尺寸
 * @param dst 输出图像
 * @return 处理时间（毫秒）
 */
double adaptiveMedianFilterParallel(const cv::Mat& src, int maxWindowSize, cv::Mat& dst) {
    assert(src.type() == CV_8UC1);
    assert(maxWindowSize % 2 == 1 && maxWindowSize >= 3);

    auto start = std::chrono::high_resolution_clock::now();

    int rows = src.rows;
    int cols = src.cols;
    dst = cv::Mat::zeros(rows, cols, CV_8UC1);

    int halfMax = maxWindowSize / 2;

    // 扩展边界
    cv::Mat padded;
    cv::copyMakeBorder(src, padded, halfMax, halfMax, halfMax, halfMax, cv::BORDER_REFLECT_101);

    // 创建并行处理对象
    AdaptiveMedianFilterParallel filter(src, padded, dst, maxWindowSize);

    // 并行处理
    cv::parallel_for_(cv::Range(0, rows), filter);

    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

int main() {
    AdaptiveMedianFilterParallel adaptiveMedianFilter()
}