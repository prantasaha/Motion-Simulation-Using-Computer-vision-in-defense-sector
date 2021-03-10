#include <iostream>
//#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main()
{
    VideoCapture cap("C:\Users\Vibhuti\Videos\VID-20130401-WA0004.mp4");
    Mat img;
        namedWindow("test");
    while(true){
        cap.read(img);
        imshow("test",img);
    }
    waitKey(0);
}
