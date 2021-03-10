#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace std;
using namespace cv;


int main(){

    VideoCapture cap("C:\\Users\\Vibhuti\\Desktop\\Bits\\all seasons of naruto\\Season 1\\Naruto  001 - Enter Naruto Uzumaki.mkv");

    namedWindow("GMD_Player",CV_WINDOW_AUTOSIZE);

    if(!cap.isOpened()){
        cout<<"error in opening the image";
    }

    int slide1=1;
    createTrackbar("GMD1","GMD_Player",&slide1,100);
    int slide2=10;
    createTrackbar("GMD2","GMD_Player",&slide2,100);

    namedWindow("GMDP");

    Mat frame,f2;

    while(1){
        bool S=cap.read(frame);
        if(!S){
            break;
        }

        int b=slide1;

        frame.convertTo(f2,-1,slide2/10,b);

        imshow("GMD_Player",frame);
        imshow("GMDP",f2);

        if(waitKey(30)==27){
            destroyAllWindows();
            break;
        }

    }
}
