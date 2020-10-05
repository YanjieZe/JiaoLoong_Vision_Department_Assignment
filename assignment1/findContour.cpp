//
// Created by zyj on 10/4/20.
//

#include "findContour.h"
#include <iostream>


void pictureShow(std::string winname, cv::Mat src)
{
    cv::namedWindow(winname);
    cv::imshow(winname,src);
    cv::waitKey(0);
    cv::destroyAllWindows();
}


std::vector<std::vector<cv::Point>> mergeContours(std::vector<std::vector<cv::Point>> contours)
{
    std::vector<std::vector<cv::Point>> allContours(1);

    for (int i = 0; i < contours.size(); ++i)
    {
        for(int j=0;j < contours[i].size();++j)
        {
            allContours[0].push_back(contours[i][j]);
        }
    }
    return allContours;
}