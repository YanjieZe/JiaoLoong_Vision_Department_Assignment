#include <iostream>
#include "findContour.h"


int main() {
    std::cout << "Processing..." << std::endl;
    bool car=0;
    bool applejudge=0;

    if(car==1) {
        std::string srcpath = "../car.jpg";
        cv::Mat src = cv::imread(srcpath);


        cv::Mat gray;
        cv::cvtColor(src, gray, cv::COLOR_BGR2GRAY);
        cv::blur(gray, gray, cv::Size(5, 5));

        cv::Mat sobelImg;
        cv::Sobel(gray, sobelImg, CV_8U, 1, 0, 3, 1, 0);

        cv::Mat thresImg;
        cv::threshold(sobelImg, thresImg, 0, 255, cv::THRESH_OTSU + cv::THRESH_BINARY);
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(17, 3));
        cv::morphologyEx(thresImg, thresImg, cv::MORPH_CLOSE, kernel);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(thresImg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);


        pictureShow("car", thresImg);
    }

    if(applejudge==1)
    {
        std::string srcpath="../apple.png";
        cv::Mat apple = cv::imread(srcpath);

//        cv::Mat gray;
//        cv::cvtColor(apple,gray,cv::COLOR_BGR2GRAY);
//        cv::threshold(gray,gray,125,255,cv::THRESH_BINARY);

        cv::Mat red;
        std::vector<cv::Mat> s(3);
        cv::split(apple,s);
        red = s[2];
        cv::GaussianBlur(red,red,cv::Size(9,9),15);
        cv::threshold(red,red,200,255,cv::THRESH_BINARY);
        cv::Mat element = getStructuringElement(cv::MORPH_RECT, cv::Size(18, 18));
        cv::morphologyEx(red,red,cv::MORPH_OPEN,element);
//        cv::Sobel(red,red,CV_8U,2,2);
        cv::Canny(red,red,240,250);
//        cv::morphologyEx(red,red,cv::MORPH_CLOSE,element);


        std::vector<std::vector<cv::Point>> contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(red,contours,hierarchy,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_NONE);
        std::vector<std::vector<cv::Point>>::iterator it = contours.begin();

        //初步面积筛选
        for (; it != contours.end();)
        {
            double Area = contourArea(*it);
            if (Area < 5000)
            {
                it = contours.erase(it);
            }
            else
            {
                ++it;
                std::cout<<Area<<" ";
            }
        }
        std::cout << "筛选后总共轮廓个数为：" << (int)contours.size() << std::endl;
        std::cout << contours.size() << std::endl;

        //轮廓合并
        std::vector<std::vector<cv::Point>> allContours=mergeContours(contours);


        //椭圆拟合
        for(size_t i = 0; i < allContours.size(); i++)
        {
            //拟合的点至少为6
            size_t count = allContours[i].size();
            if( count < 6 )
                continue;

            //椭圆拟合
            cv::RotatedRect box = fitEllipse(allContours[i]);

            //如果长宽比大于30，则排除，不做拟合
            if( MAX(box.size.width, box.size.height) > MIN(box.size.width, box.size.height)*30 )
                continue;

            //画出拟合的椭圆
            cv::ellipse(apple, box, cv::Scalar(255,0,0), 3);
        }

//        cv::drawContours(apple,allContours,-1,cv::Scalar(255,255,0),3);

        pictureShow("apple",apple);
        cv::imwrite("../appleFind.jpg",apple);
    }
    return 0;
}
