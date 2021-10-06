
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <assert.h>
#include <vector>

cv::Mat src = cv::imread("/home/nvidia/Documents/image/apple.png");

int pos1 = 22;  int max1 = 180;
int pos2 = 161;  int max2 = 180;
int pos3 = 2;   int max3 = 100;
int pos4 = 29;  int max4 = 100;

void onChange(int pos, void* img);

int main()
{
    assert(src.channels()==3);
    
    cv::imshow( "result", src );

    cv::createTrackbar( "hsv_high", "result", &pos1, max1, onChange, &src );
    cv::createTrackbar( "hsv_low", "result", &pos2, max2, onChange, &src );
    cv::createTrackbar( "median", "result", &pos3, max3, onChange, &src );
    cv::createTrackbar( "morph", "result", &pos4, max4, onChange, &src );

    cv::waitKey(0);
    return 0;
}

void onChange(int pos, void* img)
{
    //转化成hsv
    cv::Mat hsv_img;
    cv::cvtColor( src, hsv_img, cv::COLOR_BGR2HSV );

    //颜色提取
    cv::Mat hsv_part1;
    cv::Mat hsv_part2;
    cv::inRange( hsv_img, cv::Scalar(0,43,46), cv::Scalar(pos1,255,255), hsv_part1 );
    cv::inRange( hsv_img, cv::Scalar(pos2,43,46), cv::Scalar(180,255,255), hsv_part2 );
    cv::Mat ones_mat = cv::Mat::ones(cv::Size( hsv_img.cols, hsv_img.rows), CV_8UC1); 
    cv::Mat hsv_result = 255 * (ones_mat - (ones_mat - hsv_part1 / 255).mul(ones_mat - hsv_part2 / 255));

    //中值滤波
    cv::Mat blur_img;
    cv::medianBlur( hsv_result, blur_img, pos3*2+1 );

    //形态学运算
    cv::Mat open_img;
    cv::Mat kernel = cv::getStructuringElement( cv::MORPH_RECT, cv::Size(pos4+1,pos4+1));
    cv::morphologyEx( blur_img , open_img, cv::MORPH_OPEN ,kernel);

    //轮廓提取
    std::vector< std::vector<cv::Point> > contour;
    std::vector< cv::Vec4i > hierachy;

    cv::findContours( open_img, contour, hierachy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE );

    //画出轮廓
    cv::Mat drawer = src.clone();
    for (int i = 0; i < contour.size(); i++) {
        cv::drawContours( drawer, contour, i, {255,255,255}, 1 );
    }

    //框出目标
    cv::Rect box = cv::boundingRect(cv::Mat(contour[0]));
    cv::rectangle( drawer, box, {0,255,0}, 3 );

    cv::imshow( "result", drawer );
}