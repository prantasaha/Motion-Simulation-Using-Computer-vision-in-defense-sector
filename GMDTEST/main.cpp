#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include<GL/freeglut.h>
#include<GL/glut.h>

using namespace cv;
using namespace std;

int c=0,score=0;
float h=0,w=0;

GLfloat x=0,y=0;
char desc;
int main()
 {
    cv::VideoCapture cap(0); //capture the video from webcam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return 0;
    }

  int iLowH = 110;
 int iHighH = 143;

  int iLowS = 66;
 int iHighS = 255;

  int iLowV = 244;
 int iHighV = 255;

  //Create trackbars in "Control" window
if(0){
    cv::namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

 cv::createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 cv::createTrackbar("HighH", "Control", &iHighH, 179);

  cv::createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 cv::createTrackbar("HighS", "Control", &iHighS, 255);

  cv::createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
 cv::createTrackbar("HighV", "Control", &iHighV, 255);
}
//  int iLastX = -1;
// int iLastY = -1;

  //Capture a temporary image from the camera

    cv::Mat imgThresholded;
    cv::Mat imgOriginal;
    cv::Mat imgHSV;
    cv::Mat thresh;
//    Rect r(150,150,200,200);
    while (true)
    {

        bool bSuccess = cap.read(imgOriginal); // read a new frame from video


         if (!bSuccess) //if not success, break loop
        {
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }


//    imgOriginal=imgOriginal(r);
   cv::cvtColor(imgOriginal, imgHSV, cv::COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

//    threshold(imgHSV,imgThresholded,0,255,THRESH_BINARY_INV+THRESH_OTSU);
   cv::inRange(imgHSV, cv::Scalar(iLowH,iLowS,iLowV), cv::Scalar(iHighH,iHighS,iHighV), thresh); //Threshold the image
   cv::inRange(imgHSV, cv::Scalar(0,207,180), cv::Scalar(10,255,255), imgThresholded); //Threshold the image
    vector< vector < cv::Point > > contours;
    vector< vector < cv::Point > > cr;
    vector< cv::Vec4i> heirarchy;
    vector< cv::Vec4i> hr;
    cv::findContours(imgThresholded,contours,heirarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
    cv::findContours(thresh,cr,hr,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);

    for(int i=0;i<contours.size();i++){
     cv::approxPolyDP(cv::Mat(contours[i]),contours[i],3,true);
    }

  cv::Moments oMoments = moments(imgThresholded);
  double dM01 = oMoments.m01;
  double dM10 = oMoments.m10;
  double dArea = oMoments.m00;


   // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero
  if (dArea > 100)
  {
   //calculate the position of the point
   float posX = dM10 / dArea;
   float posY = dM01 / dArea;
    h=imgThresholded.rows;
    w=imgThresholded.cols;
//    x=posX;
//    y=posY;
    x=1-(posX/(w/2));
    y=1-(posY/(h/2));
    x=x*1.955;
    y=y*1.955;
   cv::circle(imgOriginal,cv::Point(posX,posY),8,cv::Scalar(0,255,0),2);

  }

    c=cr.size();

    cv::drawContours(imgOriginal,contours,-1,cv::Scalar(0,255,0));
    cv::drawContours(imgOriginal,cr,-1,cv::Scalar(0,255,255));
//    drawContours(imgOriginal,hullpoint,ic,Scalar(0,255,255));
    cv::imshow("Thresholded Image", imgThresholded); //show the thresholded image
//
//   imgOriginal = imgOriginal + imgLines;
    I
   return 0;
}
