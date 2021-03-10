#include<iostream>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/core/core.hpp>
#include<opencv/highgui.h>
#include<pthread.h>

using namespace std;
using namespace cv;

main(){

    VideoCapture cam(0);
    namedWindow("Control");
    namedWindow("Threshold Image");
    namedWindow("ROI");
    namedWindow("Gray Image");
    namedWindow("Original Image");

    Mat img;
    Mat img_gray;
    Mat thresh;
    Mat roi;
    Mat gt;
    Rect R(0,50,200,200);

namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

  int iLowH = 170;
 int iHighH = 179;

  int iLowS = 150;
 int iHighS = 255;

  int iLowV = 60;
 int iHighV = 255;
if(1){

  //Create trackbars in "Control" window
 createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 createTrackbar("HighH", "Control", &iHighH, 179);

  createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 createTrackbar("HighS", "Control", &iHighS, 255);

  createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
 createTrackbar("HighV", "Control", &iHighV, 255);
}
    while(1){

        cam.read(img);
        roi=img;
        cvtColor(img,img_gray,CV_RGB2HSV);
//        gt=img_gray;
//        GaussianBlur(img_gray,img_gray,Size(7,11),0,0);
//        threshold(img_gray,thresh,0,255,THRESH_BINARY_INV+THRESH_OTSU);
//        inRange(img_gray, Scalar(80,170,117), Scalar(140,255,255), thresh); //Threshold the image
        inRange(img_gray, Scalar(iLowH,iLowS,iLowV), Scalar(iHighH,iHighS,iHighV), thresh); //Threshold the image

        GaussianBlur(thresh,thresh,Size(5,5),0,0);
        vector<vector <Point> >contours ;
        vector<Vec4i> hierarchy ;
        findContours(thresh,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE,Point());

        size_t sc=0;
        int ic=-1;
        for(int i=0;i<contours.size();i++){
            if(contours[i].size()>sc){
                sc=contours[i].size();
                ic=i;
            }
        }
                vector<vector<int> >hull(contours.size());
                vector<vector<Point> >hullPoint(contours.size());
                vector<vector<Vec4i> >defects(contours.size());
                vector<vector<Point> >defectPoint(contours.size());
                vector<vector<Point> >contours_poly(contours.size());
                Point2f rect_point[4];
                vector<RotatedRect>minRect(contours.size());
                vector<Rect> boundRect(contours.size());



            for(size_t i=0;i<contours.size();i++){
                    if(contourArea(contours[i])>5000){
                        convexHull(contours[i],hull[i],true);
                        convexityDefects(contours[i],hull[i],defects[i]);
                        if(ic==i){
                            minRect[i]=minAreaRect(contours[i]);
                            for(size_t k=0;k<hull[i].size();k++){
                                int ind=hull[i][k];
                                hullPoint[i].push_back(contours[i][ind]);
                            }
//                            count =0;
                            for(size_t k=0;k<defects[i].size();k++){
                                if(defects[i][k][3]>13*256){
                                 /* int p_start=defects[i][k][0];   */
                                    int p_end=defects[i][k][1];
                                    int p_far=defects[i][k][2];
                                    defectPoint[i].push_back(contours[i][p_far]);
//                                    circle(roi,contours[i][p_end],3,Scalar(0,255,0),2);
//                                    count++;
                                }

                            }
                        }
                    }
            }

//         if(count==5){
//
//         }
      drawContours( img, contours, -1, Scalar(255,255,255), 1, 4, hierarchy, 0, Point() );
        imshow("Threshold Image",thresh);
        imshow("ROI",roi);
        imshow("Gray Image",img_gray);
        imshow("Original Image",img);
        if(waitKey(30)==27){
            return 0;
        }
    }
}

