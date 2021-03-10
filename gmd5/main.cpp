#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/core/core.hpp"
#include<GL/freeglut.h>
#include<GL/glut.h>
#include<pthread.h>
#include<process.h>
#include<windows.h>

using namespace cv;
using namespace std;

class a{
    public:
    int *ac;
    char **av;
};


int c=0,score=0;
float h=0,w=0;

GLfloat x=0,y=0;
char desc;
void hgd()
 {
    VideoCapture cap(0); //capture the video from webcam

    if ( !cap.isOpened() )  // if not success, exit program
    {
         cout << "Cannot open the web cam" << endl;
         return;
    }

  int iLowH = 110;
 int iHighH = 143;

  int iLowS = 66;
 int iHighS = 255;

  int iLowV = 244;
 int iHighV = 255;

  //Create trackbars in "Control" window
if(0){
    namedWindow("Control", CV_WINDOW_AUTOSIZE); //create a window called "Control"

 createTrackbar("LowH", "Control", &iLowH, 179); //Hue (0 - 179)
 createTrackbar("HighH", "Control", &iHighH, 179);

  createTrackbar("LowS", "Control", &iLowS, 255); //Saturation (0 - 255)
 createTrackbar("HighS", "Control", &iHighS, 255);

  createTrackbar("LowV", "Control", &iLowV, 255);//Value (0 - 255)
 createTrackbar("HighV", "Control", &iHighV, 255);
}
//  int iLastX = -1;
// int iLastY = -1;

  //Capture a temporary image from the camera

    Mat imgT;
    uchar **imgThreshold=new uchar*[imgT.rows];
    for(int i=0;i<imgT.rows;i++)
        imgThreshold[i]=new uchar[imgT.cols];
    for(int i=0;i<imgT.rows;i++)
        imgThreshold[i]=imgT.ptr<uchar>(i);
    Mat imgO;
    uchar **imgOriginal=new uchar*[imgO.rows];
    for(int i=0;i<imgT.rows;i++)
        imgOriginal[i]=new uchar[imgO.cols];
    for(int i=0;i<imgT.rows;i++)
        imgOriginal[i]=imgO.ptr<uchar>(i);
    Mat imHSV;
    uchar **imgHSV=new uchar*[imHSV.rows];
    for(int i=0;i<imHSV.rows;i++)
        imgHSV[i]=new uchar[imHSV.cols];
    for(int i=0;i<imHSV.rows;i++)
        imgHSV[i]=imHSV.ptr<uchar>(i);
    Mat th;
    uchar **thresh=new uchar*[th.rows];
    for(int i=0;i<th.rows;i++)
        thresh[i]=new uchar[th.cols];
    for(int i=0;i<th.rows;i++)
        thresh[i]=th.ptr<uchar>(i);
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
   cvtColor(imgOriginal, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV

//    threshold(imgHSV,imgThresholded,0,255,THRESH_BINARY_INV+THRESH_OTSU);
   inRange(imgHSV, Scalar(iLowH,iLowS,iLowV), Scalar(iHighH,iHighS,iHighV), thresh); //Threshold the image
   inRange(imgHSV, Scalar(0,207,180), Scalar(10,255,255), imgThresholded); //Threshold the image
    vector< vector < Point > > contours;
    vector< vector < Point > > cr;
    vector< Vec4i> heirarchy;
    vector< Vec4i> hr;
    findContours(imgThresholded,contours,heirarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
    findContours(thresh,cr,hr,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE);
  //morphological opening (removes small objects from the foreground)
//  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
//  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
//
//   //morphological closing (removes small holes from the foreground)
//  dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
//  erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );


    for(int i=0;i<contours.size();i++){
     approxPolyDP(Mat(contours[i]),contours[i],3,true);
    }

  Moments oMoments = moments(imgThresholded);
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
   circle(imgOriginal,Point(posX,posY),8,Scalar(0,255,0),2);

  }

    c=cr.size();

    drawContours(imgOriginal,contours,-1,Scalar(0,255,0));
    drawContours(imgOriginal,cr,-1,Scalar(0,255,255));
    imshow("Thresholded Image", imgThresholded); //show the thresholded image
    imshow("Original", imgOriginal); //show the original image

        if (waitKey(30) == 27){
            cout << "esc key is pressed by user" << endl;
            break;
       }
    }
   return;
}
void render(){

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0,0.0,0.0);
    glVertex2f(0, 0); // center of circle
    float radius=0.35,twicePi=2*3.14;
    for (int i = 0; i <= 50; i++)   {
        glVertex2f (radius * cos(i * twicePi / 50),radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0,1.0,0.0);
    glVertex2f(0, 0); // center of circle
    radius=0.25;
    for (int i = 0; i <= 50; i++)   {
        glVertex2f (radius * cos(i * twicePi / 50),radius * sin(i * twicePi / 50));
    }
    glEnd();
//    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
//    glColor3f(0.0,0.0,1.0);
//    glVertex2f(0, 0); // center of circle
//    radius=0.5;
//    for (int i = 0; i <= 50; i++)   {
//        glVertex2f (radius * cos(i * twicePi / 50),radius * sin(i * twicePi / 50));
//    }
//    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0,1.0,0.0);
    glVertex2f(0, 0); // center of circle
    radius=0.15;
    for (int i = 0; i <= 50; i++)   {
        glVertex2f (radius * cos(i * twicePi / 50),radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0,0.0,1.0);
    glVertex2f(0, 0); // center of circle
    radius=0.05f;
    for (int i = 0; i <= 50; i++)   {
        glVertex2f (radius * cos(i * twicePi / 50),radius * sin(i * twicePi / 50));
    }
    glEnd();
//        for(int i=0;i<xy.size();i++){
if(c!=0){

        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
            for (int i = 0; i <= 50; i++)   {
                glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
            }
        glEnd();
        //For scores
//            if(x>0.8 || y>0.8){
//                score=score+5;
//            }
//            else if(x>=0.55 || y>=0.55){
//                score=score+10;
//            }
//            else if(x>=0.25 || y>=0.25){
//                score=score+20;
//            }
//            else if(x>=0.1 || y>=0.1){
//                score=score+40;
//            }
//            if(x==0 && y==0){
//                score=score+100;
//            }
}
//        }

    glTranslatef(x,y,0.0f);
if(desc=='y'){
    glBegin(GL_LINES);
//    glLineWidth(5);
    glColor3f(0.0,0.0,1.0);
    glVertex2f(-0.05f,0.0f);
    glVertex2f(0.05f,0.0f);
    glVertex2f(0.0f,0.05f);
    glVertex2f(0.0f,-0.05f);
    glEnd();
//    glBegin(GL_QUADS);
//    glVertex2f(-0.1f,-0.1f);
//    glVertex2f(-0.1f,0.1f);
//    glVertex2f(0.1f,0.1f);
//    glVertex2f(0.1f,-0.1f);
//    glEnd();
}
    glutSwapBuffers();
}
void key(unsigned char c, int x, int y){
    if(c=='q'){
//        cout<<score;
//        pthread_exit(NULL);
        exit(0);
    }
}
void * crender(void * abc){
    a *b=(a *)abc;
    glutInit(b->ac,b->av);
//    glutInitWindowSize(h,w);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE |GLUT_RGBA);
    glutInitWindowSize(h,w);
    glutCreateWindow("GMD");
    glutDisplayFunc(render);
    glutIdleFunc(render);
    glutKeyboardFunc(key);
    glutMainLoop();
}

int main(int argc,char **argv)
{
    pthread_t t;
    a abc;
    abc.ac=&argc;
    abc.av=argv;
    cout<<"NEED A CROSSHAIR?";
    cin>>desc;
    pthread_create(&t,NULL,crender,(void *)(&abc));
    hgd();
    pthread_join(t,NULL);
}
