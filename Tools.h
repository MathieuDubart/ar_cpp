//
// Created by Damien Dabernat on 02/11/2022.
//

#ifndef CPPOPENCVARPROJECT_TOOLS_H
#define CPPOPENCVARPROJECT_TOOLS_H


#include <opencv2/core/types.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/aruco.hpp>

class Tools {

public:
    static float euclideanDist(cv::Point2i &a, cv::Point2i &b);

    static void overlayImage(const cv::Mat &background, const cv::Mat &foreground,
                             cv::Mat &output, cv::Point2i location);

    static void createMarker(int id);

    static void warpImage(const cv::Mat &background, const cv::Mat &src, cv::Mat &dst, cv::Point2f location[]);

    static void warpOverlayImage(const cv::Mat &background, const cv::Mat &foreground,
                          cv::Mat &output, std::vector<cv::Point2f> location);
};


#endif //CPPOPENCVARPROJECT_TOOLS_H
