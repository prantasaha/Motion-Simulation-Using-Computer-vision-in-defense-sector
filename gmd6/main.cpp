#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include<windows.h>
#include<conio.h>

using namespace cv;
using namespace std;

int main( ){

    VideoCapture cap(0);

    if ( !cap.isOpened() ){
         cout << "Cannot open the web cam" << endl;
         return -1;
    }
//    namedWindow("IMG");
//    namedWindow("ROI");
//
//    Mat img,roi;
//    Rect r(50,50,200,150);
//
//    while(1){
//        cap.read(img);
//        roi=img(r);
//        rectangle(img,Point(200,200),Point(250,250),Scalar(255,0,0));
//        imshow("IMG",img);
//        imshow("ROI",roi);
//        if(waitKey(30)==27){
//            destroyAllWindows();
//            break;
//        }
//    }
    Mat img,hsv,thresh;
    int largest_area=0,largest_contour_index=0,c=0;


    vector<vector <Point> >contours;
    vector<Vec4i>hierarchy;


    namedWindow("Original");
    namedWindow("Threshold");
    namedWindow("Control",CV_WINDOW_AUTOSIZE);

    int iLowH = 0;
    int iHighH = 10;

    int iLowS = 240;
    int iHighS = 255;

    int iLowV = 50;
    int iHighV = 255;


   createTrackbar("LowH", "Control", &iLowH, 255);
    createTrackbar("HighH", "Control", &iHighH, 255);

    createTrackbar("LowS", "Control", &iLowS, 255);
    createTrackbar("HighS", "Control", &iHighS, 255);

    createTrackbar("LowV", "Control", &iLowV, 255);
    createTrackbar("HighV", "Control", &iHighV, 255);


    do{

        bool S = cap.read(img);

        if (!S){
             cout << "Cannot read a frame from video stream" << endl;
             break;
        }

        cvtColor(img,hsv,CV_BGR2HSV);
        inRange(hsv,Scalar(iLowH,iLowS,iLowV),Scalar(iHighH,iHighS,iHighV),thresh);
//        erode( thresh, thresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
//        dilate( thresh, thresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
//
//
//        dilate( thresh, thresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
//        erode( thresh, thresh, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );


        findContours( thresh, contours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE );

        for(int i=0;i<contours.size();i++){

            approxPolyDP(Mat(contours[i]),contours[i],3,true);

        }


        drawContours(img,contours,-1,Scalar(0,225,100),2);

        imshow("Original",img);
        imshow("Threshold",thresh);


        if(waitKey(30)==27){

            destroyAllWindows();
            break;

        }
    }while(1);
    return 0;
}
