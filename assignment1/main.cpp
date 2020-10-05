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
        cv::GaussianBlur(gray, gray, cv::Size(5, 5),15);

        cv::Mat sobelx,sobely,sobelImg;
        cv::Sobel(gray,sobelx,CV_8U,1,0,3,1,1);
        cv::convertScaleAbs(sobelx,sobelx,3);
        cv::Sobel(gray,sobely,CV_8U,0,1,3,1,1);
        cv::convertScaleAbs(sobely,sobely,3);
        cv::addWeighted(sobelx,0.6,sobely,0.5,0,sobelImg);


        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(17, 3));
        cv::morphologyEx(sobelImg, sobelImg, cv::MORPH_CLOSE, kernel);

        cv::threshold(sobelImg, sobelImg, 100, 255, cv::THRESH_BINARY);

        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(sobelImg, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);

        std::vector<std::vector<cv::Point>>::iterator it = contours.begin();
        //初步面积筛选
        for (; it != contours.end();)
        {
            double Area = contourArea(*it);
            if (Area < 6000 || Area > 7000)
            {
                it = contours.erase(it);
            }
            else
            {
                ++it;
                std::cout<<"Area:"<<Area<<std::endl;
            }
        }

        cv::RotatedRect box=cv::minAreaRect(contours[0]);
        cv::Point2f points[4];
        box.points(points);
        cv::Mat source = src.clone();
        for (int i = 0; i < 4; i++)
        {
            line(src, points[i], points[(i + 1) % 4], cv::Scalar(255, 255, 0), 3);
            std::cout<<"the "<<i<<" point:"<<points[i]<<std::endl;
        }
//        cv::drawContours(src,contours,-1,cv::Scalar(0,255,0),3);


//        pictureShow("process", sobelImg);
//        pictureShow("car",src);
//        cv::imwrite("../carplateFind.jpg",src);



        // cut the region of the plate
        cv::Mat plate;
        float x1,x2,y1,y2,width,height;
        x1 = points[1].x;
        x2 = points[3].x;
        y1 = points[1].y;
        y2 = points[3].y;
        width = x2 - x1;
        height = y2 - y1;

        cv::Rect2f square(x1,y1,width,height);
        plate = source(square);
        pictureShow("plate",plate);
//        cv::imwrite("../plateCut.jpg",plate);

//        cv::Point2f center;
//        center.x = (x2-x1)/2;
//        center.y = (y2-y1)/2;

        std::cout<<"Plate's size:"<<plate.size<<std::endl;

        //transform the plate
        cv::Mat plateAffined;
        cv::Point2f p0[4],p1[4];
        p0[0].x=0,p0[0].y=0;
        p0[1].x=5,p0[1].y=52;
        p0[2].x=161,p0[2].y=0;
        p0[3].x=171,p0[3].y=52;

        p1[0].x=0,p1[0].y=0;
        p1[1].x=0,p1[1].y=52;
        p1[2].x=171,p1[2].y=0;
        p1[3].x=171,p1[3].y=52;

        cv::Mat m=cv::getPerspectiveTransform(p0,p1);
        cv::warpPerspective(plate,plateAffined,m,cv::Size(172,50));
        std::cout<<"PlateAffined's size:"<<plateAffined.size<<std::endl;

        pictureShow("Affined",plateAffined);

        cv::Mat plateGray;
        cv::cvtColor(plateAffined,plateGray,cv::COLOR_BGR2GRAY);
        cv::threshold(plateGray,plateGray,125,255,cv::THRESH_BINARY);
        pictureShow("plateGray",plateGray);
        cv::imwrite("../plateFinnal.jpg",plateGray);
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
