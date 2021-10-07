#include <iostream>
#include <assert.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int main()
{
    cv::VideoCapture capture("../video.mp4");
    cv::Mat src;
    capture >> src;
    assert( !src.empty() );

    cv::VideoWriter writer( "../result.avi", cv::VideoWriter::fourcc('M','J','P','G'),50,cv::Size(src.cols,src.rows), true );
    while( true )
    {
        cv::Mat result = src.clone();
        assert( result.channels()==3 );
        
        cv::Mat channels[3];
        cv::split( result, channels);
        cv::Mat red_sub_blue = channels[2] - channels[0];
        cv::normalize( red_sub_blue, red_sub_blue, 0., 255., cv::NORM_MINMAX );
        cv::threshold( red_sub_blue, red_sub_blue, 100, 255, cv::THRESH_OTSU );
        cv::medianBlur( red_sub_blue, red_sub_blue , 3 );
        cv::Mat kernel = cv::getStructuringElement( cv::MORPH_RECT, cv::Size(5,5) );
        cv::morphologyEx( red_sub_blue, red_sub_blue, cv::MORPH_CLOSE, kernel );

        std::vector<std::vector<cv::Point>> contours;
        std::vector< cv::Vec4i > hierachy;
        cv::Scalar colorLists[3] = { {255,0,0},{0,255,0},{255,255,255} };
        cv::findContours( red_sub_blue, contours, hierachy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE );
        std::vector< std::vector< cv::Point > > contours_poly(contours.size());
        for (int i = 0; i < contours.size(); i++)
        {
            //cv::approxPolyDP(contours[i],contours_poly[i],13,true);
            cv::drawContours( result, contours, i, cv::Scalar(100,200,200),1);
            cv::RotatedRect box = cv::minAreaRect( contours[i]);
            cv::Point2f points[4]; box.points( points );
            double area = cv::contourArea(contours[i],false);
            //std::cout << area << std::endl;
            if ( area < 7000 && area > 6000) 
            {
                for (int i = 0; i < 4; i++)
                {
                    cv::line( result, points[i], points[(i+1)%4], colorLists[0],2 );
                }
            }
            if ( area < 4000 && area > 2000)
            {
                for (int i = 0; i < 4; i++)
                {
                    cv::line( result, points[i], points[(i+1)%4], colorLists[1],2 );
                }
            }
            if ( area < 200)
            {
                for (int i = 0; i < 4; i++)
                {
                    cv::line( result, points[i], points[(i+1)%4], colorLists[2],2 );
                }
            }
        }

        cv::imshow( "result", result );
        cv::waitKey(10);
        writer << result;
        capture >> src;
        if (src.empty()) break; 
    }
    writer.release();
    return 0;
}