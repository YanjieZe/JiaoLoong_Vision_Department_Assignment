//
// Created by zyj on 10/4/20.
//

#ifndef DETECTION_FINDCONTOUR_H
#define DETECTION_FINDCONTOUR_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>


void pictureShow(std::string winname, cv::Mat src);

std::vector<std::vector<cv::Point>> mergeContours(std::vector<std::vector<cv::Point>> contours);


#endif //DETECTION_FINDCONTOUR_H
