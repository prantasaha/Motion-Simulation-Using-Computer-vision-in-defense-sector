#include <iostream>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include<opencv2/core/core.hpp>
#include<GL/glew.h>
#include<GL/freeglut.h>
#include<GL/glut.h>
#include<pthread.h>
#include<process.h>
#include<windows.h>
#include<math.h>
#include<cstdlib>
#include<ctime>
#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<stdarg.h>
#include<fstream>
using namespace cv;
using namespace std;

class a
{
public:
    int *ac;
    char **av;
};
void renderBitmapString(float x, float y, void *font, char *string)
{
    char *c;
    glRasterPos2f(x, y);
    for (c = string; *c != '\0'; c++)
    {
        glutBitmapCharacter(font, *c);
    }
}
void render();
int c=0,score=0,abc=1;
float h=0,w=0;
int no_shots=0,score_one=0,accuracy=0,no_miss=0,no_hits=0;
float xLocation1,yLocation,zLocation;
float yRotationAngle;
bool* keyStates = new bool[256]; // Create an array of boolean values of length 256 (0-255)
bool movingUp = false; // Whether or not we are moving up or down
float xLocation = 0.0f; // Keep track of our position on the y axis.
float xRotationAngle = 0.0f;
GLfloat x=0,y=0;
char desc;

void keyOperations(void)
{
    if (keyStates[GLUT_KEY_LEFT])   // If the left arrow key has been pressed
    {
        // Perform left arrow key operations
    }
}

void reshape(int width, int height)
{
    glViewport(0, 0, (GLsizei)width, (GLsizei)height); // Set our viewport to the size of our window
    glMatrixMode(GL_PROJECTION); // Switch to the projection matrix so that we can manipulate how our scene is viewed
    glLoadIdentity(); // Reset the projection matrix to the identity matrix so that we don't get any artifacts (cleaning up)

    gluPerspective(60, (GLfloat)width / (GLfloat)height, 1.0, 100.0); // Set the Field of view angle (in degrees), the aspect ratio of our window, and the new and far planes

    glMatrixMode(GL_MODELVIEW); // Switch back to the model view matrix, so that we can start drawing shapes correctly
}

void keyPressed(unsigned char key, int x, int y)
{
    keyStates[key] = true; // Set the state of the current key to pressed
}

void keyUp(unsigned char key, int x, int y)
{
    keyStates[key] = false; // Set the state of the current key to not pressed
}

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
    no_hits++;

    int iLowV = 244;
    int iHighV = 255;

    //Create trackbars in "Control" window
    if(0)
    {
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
    Mat imgThresholded;
    Mat imgOriginal;
    Mat imgHSV;
    Mat thresh;
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



        for(int i=0; i<contours.size(); i++)
        {
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

//
//    vector < vector < int > > hull(contours.size());
//    vector < vector < Point > > hullpoint(contours.size());
//    vector < vector < Vec4i > > defects (contours.size());

//   size_t ic=-1;
//   size_t sc=0;
//
//   for(int i=0;i>contours.size();i++){
//        if(sc<contours[i].size()){
//            sc=contours[i].size();
//            ic=i;
//        }
//   }
//
//   size_t xx=mu[ic].m01/mu[ic].m00;
//   size_t yy=mu[ic].m10/mu[ic].m00;
//
//   circle(imgOriginal,Point(xx,yy),3,Scalar(0,0,0));

//            for(size_t i=0;i<contours.size();i++){
//                    if(contourArea(contours[i])>10){
//                        convexHull(contours[i],hull[i],true);
//                        convexityDefects(contours[i],hull[i],defects[i]);
//                        if(ic==i){
//                            minRect[i]=minAreaRect(contours[i]);
//                            for(size_t k=0;k<hull[i].size();k++){
//                                int ind=hull[i][k];
//                                hullpoint[i].push_back(contours[i][ind]);
//                            }
////                            count =0;
//                            for(size_t k=0;k<defects[i].size();k++){
//                                if(defects[i][k][3]>13*256){
//                                  int p_start=defects[i][k][0];
//                                    int p_end=defects[i][k][1];
//                                    int p_far=defects[i][k][2];
//                                    defectPoint[i].push_back(contours[i][p_far]);
//                                    circle(imgOriginal,contours[i][p_end],3,Scalar(0,255,0),2);
//                                    count++;
//                                }
//
//                            }
//                        }
//                    }
//
//
//   }

        //Calculate the moments of the thresholded image
//  Moments oMoments = moments(imgThresholded);
//
//  double dM01 = oMoments.m01;
//  double dM10 = oMoments.m10;
//  double dArea = oMoments.m00;
//
//
//   // if the area <= 10000, I consider that the there are no object in the image and it's because of the noise, the area is not zero
//  if (dArea > 10000)
//  {
//   //calculate the position of the ball
//   int posX = dM10 / dArea;
//   int posY = dM01 / dArea;
//
//   if (iLastX >= 0 && iLastY >= 0 && posX >= 0 && posY >= 0)
//   {
//    //Draw a red line from the previous point to the current point
//    line(imgLines, Point(posX, posY), Point(iLastX, iLastY), Scalar(0,0,255), 2);
//   }
//
//    iLastX = posX;
//   iLastY = posY;
//  }
        drawContours(imgOriginal,contours,-1,Scalar(0,255,0));
        drawContours(imgOriginal,cr,-1,Scalar(0,255,255));
//    drawContours(imgOriginal,hullpoint,ic,Scalar(0,255,255));
        //imshow("Thresholded Image", imgThresholded); //show the thresholded image
//
//imshow("Thresholded Image", imgThresholded);
//   imgOriginal = imgOriginal + imgLines;
        // imshow("Original", imgOriginal); //show the original image
//imshow("Original", imgOriginal);
        if (waitKey(30) == 27)
        {
            cout << "esc key is pressed by user" << endl;
            break;
        }
    }
    return;
}
void render()
{
    keyOperations();
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Clear the background of our window to black
    //glMatrixMode(GL_MODELVIEW);
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity();
 //     if(score_one==100)
   // {
   //     glutLeaveMainLoop();
   // }
    // Load the Identity Matrix to reset our drawing locations
    glTranslatef(0.0f, 0.0f, -1.0f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive
    //glTranslatef(0.0f, 0.0f, 0.0f); // Translate our object along the y axis
    //glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate our object around the y axis
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    char buffer[20]= {'\0'};
    //char buffer2[20]={'\0'};
    glColor3f(0.0f,0.0f,0.0f);
    sprintf(buffer,"%d",no_shots);
    //string s1="no. of shots:";
    //sprintf(buffer2,"%s",s1);
    //renderBitmapString(.55f, .5f, GLUT_BITMAP_TIMES_ROMAN_24, buffer2);
    renderBitmapString(.9f, .5f, GLUT_BITMAP_TIMES_ROMAN_24, buffer);
    glFlush(); // Flush the OpenGL buffers to the window
    if (movingUp) // If we are moving up
        yLocation -= 0.0010f; // Move up along our yLocation
    else  // Otherwis
        yLocation += 0.0010f; // Move down along our yLocation
    if (yLocation < -1.25f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 1.25f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up
    yRotationAngle += 0.005f; // Increment our rotation value
    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();

        abc=0;
    }
    if(abc==0&&c==0)
    {
        no_shots=(no_shots+1);
        if(x*x+y*y<=.25f)
        {
            score_one=score_one+10;
            no_hits++;
        }
        else if(x*x+y*y<=.0225f)
        {
            score_one=score_one+9;
            no_hits++;
        }
        else if(x*x+y*y<=.0625f)
        {
            score_one=score_one+8;
            no_hits++;
        }
        else if(x*x+y*y<=.1225f)
        {
            score_one=score_one+7;
            no_hits++;
        }
        else
        {
            no_miss++;
        }
        accuracy=accuracy+(score_one/no_shots);
        abc=1;
    }
    glTranslatef(x,y,0.0f);



// glFontBegin(&font);
    //glScalef(8.0, 8.0, 8.0);

//glFontTextOut("Test", 5, 5, 0);
    //glFontEnd();

    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render1()
{
    keyOperations();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
    glTranslatef(0.0f, 0.0f, -2.0f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive
    glTranslatef(0.0f, yLocation, 0.0f); // Translate our object along the y axis
    //glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate our object around the y axis
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glFlush(); // Flush the OpenGL buffers to the window
    if (movingUp) // If we are moving up
        yLocation -= 0.0010f; // Move up along our yLocation
    else  // Otherwis
        yLocation += 0.0010f; // Move down along our yLocation
    if (yLocation < -1.25f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 1.25f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up
    yRotationAngle += 0.005f; // Increment our rotation value
    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }

    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();

}
void render2()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations

    glTranslatef(0.0f, 0.0f, -2.0f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive

    glTranslatef(yLocation, 0.0f, 0.0f); // Translate our object along the y axis

    //glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate our object around the y axis
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.001f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.001f; // Move down along our yLocation

    if (yLocation < -2.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 2.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.005f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation   // Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render3()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations

    glTranslatef(0.0f, 0.0f, -2.0f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive

    glTranslatef(xLocation, yLocation, 0.0f); // Translate our object along the y axis

    //glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate our object around the y axis
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.0005f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.0005f; // Move down along our yLocation

    if (yLocation < -2.4f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 2.4f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up
    if (movingUp) // If we are moving up
        xLocation += 0.001f; // Move up along our yLocation
    else  // Otherwise
        xLocation -= 0.001f; // Move down along our yLocation

    if (xLocation > 2.4f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (xLocation <- 2.4f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.005f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation  ct 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render4()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations

    glTranslatef(0.0f, 0.0f, -2.0f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive

    glTranslatef(-xLocation, yLocation, 0.0f); // Translate our object along the y axis

    //glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate our object around the y axis
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.0005f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.0005f; // Move down along our yLocation

    if (yLocation < -2.4f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 2.4f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up
    if (movingUp) // If we are moving up
        xLocation += 0.001f; // Move up along our yLocation
    else  // Otherwise
        xLocation -= 0.001f; // Move down along our yLocation

    if (xLocation > 2.4f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (xLocation <- 2.4f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.005f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f;
    if (movingUp) // If we are moving up
        yLocation -= 0.0005f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.0005f; // Move down along our yLocation

    if (yLocation < -2.4f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 2.4f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up
    if (movingUp) // If we are moving up
        xLocation += 0.001f; // Move up along our yLocation
    else  // Otherwise
        xLocation -= 0.001f; // Move down along our yLocation

    if (xLocation > 2.4f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (xLocation <- 2.4f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.005f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f;// Subtract 360 degrees off of our rotation  btract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render5()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations

    glTranslatef(0.0f, 0.0f, -2.0f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive

    //glTranslatef(0.0f, yLocation, 0.0f); // Translate our object along the y axis

    glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate our object around the y axis
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.005f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.005f; // Move down along our yLocation

    if (yLocation < -3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.05f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render6()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations

    glTranslatef(0.0f, 0.0f, -2.0f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive

    //glTranslatef(0.0f, yLocation, 0.0f); // Translate our object along the y axis

    glRotatef(yRotationAngle, 1.0f, 0.0f, 0.0f); // Rotate our object around the y axis
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.005f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.005f; // Move down along our yLocation

    if (yLocation < -3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.05f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render7()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations

    //glTranslatef(0.0f, 0.0f, -2.50f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive

    glTranslatef(0.0f, 0.0f, yLocation); // Translate our object along the y axis

    //glRotatef(yRotationAngle, 0.0f, 0.0f, 1.0f); // Rotate our object around the y axis
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.0010f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.0010f; // Move down along our yLocation

    if (yLocation < -4.5f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > -.25f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.05f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation  ubtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render8()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations

    glTranslatef(0.0f, 0.0f, -2.50f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive

    glTranslatef(yLocation, 0.0f, xLocation); // Translate our object along the y axis

    //glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate our object around the y axis
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.0025f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.0025f; // Move down along our yLocation

    if (yLocation < -3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 2.5f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    if (movingUp) // If we are moving up
        xLocation -= 0.0005f; // Move up along our yLocation
    else  // Otherwise
        xLocation += 0.0005f; // Move down along our yLocation

    if (xLocation < -8.5f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (xLocation > .750f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.005f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render9()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations

    glTranslatef(0.0f, 0.0f, -2.50f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive

    glTranslatef(0.0f, yLocation, xLocation); // Translate our object along the y axis

    //glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate our object around the y axis
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.0015f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.0015f; // Move down along our yLocation

    if (yLocation < -3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 2.5f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    if (movingUp) // If we are moving up
        xLocation -= 0.001f; // Move up along our yLocation
    else  // Otherwise
        xLocation += 0.001f; // Move down along our yLocation

    if (xLocation < -8.5f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (xLocation > .750f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up


    yRotationAngle += 0.005f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation  / Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render10()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations

    glTranslatef(0.0f, 0.0f, -2.0f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive

    glTranslatef(-xLocation, yLocation, zLocation); // Translate our object along the y axis

    //glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate our object around the y axis
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.0005f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.0005f; // Move down along our yLocation

    if (yLocation < -3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up
    if (movingUp) // If we are moving up
        xLocation += 0.001f; // Move up along our yLocation
    else  // Otherwise
        xLocation -= 0.001f; // Move down along our yLocation

    if (xLocation > 3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (xLocation <- 2.4f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.005f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f;
    if (movingUp) // If we are moving up
        yLocation -= 0.0005f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.0005f; // Move down along our yLocation

    if (yLocation < -2.4f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 2.4f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up
    if (movingUp) // If we are moving up
        xLocation += 0.001f; // Move up along our yLocation
    else  // Otherwise
        xLocation -= 0.001f; // Move down along our yLocation

    if (xLocation > 2.4f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (xLocation <- 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.005f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f;// Subtr
    if (movingUp) // If we are moving up
        zLocation -= 0.001f; // Move up along our yLocation
    else  // Otherwise
        zLocation += 0.001f; // Move down along our yLocation

    if (zLocation < -10.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (zLocation > .750f) // Else if we have gone down too far
        movingUp = true; //rse our direction so we are moving up

    yRotationAngle += 0.005f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render11()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations

    glTranslatef(0.0f, 0.0f, -2.50f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive

    glTranslatef(xLocation, yLocation, zLocation); // Translate our object along the y axis

    //glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate our object around the y axis
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.0005f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.0005f; // Move down along our yLocation

    if (yLocation < -3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up
    if (movingUp) // If we are moving up
        xLocation += 0.001f; // Move up along our yLocation
    else  // Otherwise
        xLocation -= 0.001f; // Move down along our yLocation

    if (xLocation > 3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (xLocation <- 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.005f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f;
    if (movingUp) // If we are moving up
        yLocation -= 0.0005f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.0005f; // Move down along our yLocation

    if (yLocation < -3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up
    if (movingUp) // If we are moving up
        xLocation += 0.001f; // Move up along our yLocation
    else  // Otherwise
        xLocation -= 0.001f; // Move down along our yLocation

    if (xLocation > 3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (xLocation <- 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.005f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f;// Subtr
    if (movingUp) // If we are moving up
        zLocation -= 0.0008f; // Move up along our yLocation
    else  // Otherwise
        zLocation += 0.0008f; // Move down along our yLocation

    if (zLocation < -10.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (zLocation > .750f) // Else if we have gone down too far
        movingUp = true; //rse our direction so we are moving up

    yRotationAngle += 0.005f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render12()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
    glTranslatef(0.0f, 0.0f, -2.0f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive
    //glTranslatef(yLocation, yLocation, 0.0f); // Translate our object along the y axis
    // glRotatef(yRotationAngle, -.5f, .5f, 0.0f); // Rotate our object around the y axis
    glRotatef(0.0f, 0, 1, 0);
    glRotatef(yRotationAngle, 0.0, 0.0, 1.0);
    glTranslatef(.80, 0.0, 0.0);

    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.05f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.05f; // Move down along our yLocation

    if (yLocation < -3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.05f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render13()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
    glTranslatef(0.0f, 0.0f, -2.0f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive
    //glTranslatef(yLocation, yLocation, 0.0f); // Translate our object along the y axis
    // glRotatef(yRotationAngle, -.5f, .5f, 0.0f); // Rotate our object around the y axis
    glRotatef(0.0f, 0, 1, 0);
    glRotatef(yRotationAngle, 1.0, 0.0, 0.0);
    glTranslatef(.0, .50, .50);

    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.05f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.05f; // Move down along our yLocation

    if (yLocation < -3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.05f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render14()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
    glTranslatef(0.0f,0.0f, -2.0f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive
    //glTranslatef(yLocation, yLocation, 0.0f); // Translate our object along the y axis
    // glRotatef(yRotationAngle, -.5f, .5f, 0.0f); // Rotate our object around the y axis
    glRotatef(0.0f, 0, 1, 0);
    glRotatef(yRotationAngle, 0.0, -1.0, 0.0);
    glTranslatef(.70, 0.0, 0.0);

    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.05f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.05f; // Move down along our yLocation

    if (yLocation < -3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.05f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render15()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
    glTranslatef(0.0f, 0.0f, -2.50f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive
    glTranslatef(0.0, yLocation, 0.0f); // Translate our object along the y axis
    // glRotatef(yRotationAngle, -.5f, .5f, 0.0f); // Rotate our object around the y axis
    glRotatef(0.0f, 0, 1, 0);
    glRotatef(yRotationAngle, 0.0, 0.0, 0.0);
    glTranslatef(.50, 0.0, 0.0);

    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.0035f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.0035f; // Move down along our yLocation

    if (yLocation < -3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.05f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render16()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
    glTranslatef(0.0f, 0.0f, -2.50f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive
    glTranslatef(yLocation, yLocation, 0.0f); // Translate our object along the y axis
    // glRotatef(yRotationAngle, -.5f, .5f, 0.0f); // Rotate our object around the y axis
    glRotatef(0.0f, 0, 1, 0);
    glRotatef(yRotationAngle, 0.0, 0.0, 0.0);
    glTranslatef(yLocation, yLocation, 0.0);

    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.0035f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.0035f; // Move down along our yLocation

    if (yLocation < -3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.05f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render17()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
    glTranslatef(0.0f, 0.0f, -2.50f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive
    glTranslatef(0.0, yLocation, 0.0f); // Translate our object along the y axis
    // glRotatef(yRotationAngle, -.5f, .5f, 0.0f); // Rotate our object around the y axis
    glRotatef(0.0f, 0, 1, 0);
    glRotatef(yRotationAngle, 0.0, 0.0, 0.0);
    glTranslatef(yLocation, 0.0, 0.0);

    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.0035f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.0035f; // Move down along our yLocation

    if (yLocation < -3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.05f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render18()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
    glTranslatef(yLocation, 0.0f, -2.50f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive
    glTranslatef(0.0, yLocation, 0.0f); // Translate our object along the y axis
    // glRotatef(yRotationAngle, -.5f, .5f, 0.0f); // Rotate our object around the y axis
    glRotatef(yRotationAngle, 0, 1, 0);
    glRotatef(yRotationAngle, 0.0, 1.0, 0.0);
    glTranslatef(.50, 0.0, 0.0);

    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.0035f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.0035f; // Move down along our yLocation

    if (yLocation < -3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.05f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render19()
{
    keyOperations();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
    glTranslatef(0.0f,- yLocation, -2.50f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive
    glTranslatef(yLocation, 0.0, 0.0f); // Translate our object along the y axis
    // glRotatef(yRotationAngle, -.5f, .5f, 0.0f); // Rotate our object around the y axis
    glRotatef(yRotationAngle, 0, 1, 0);
    glRotatef(-yRotationAngle, 1, 0.0, 0.0);
    glTranslatef(-yLocation, 0.0f, 0.0);
    //glRotatef(yRotationAngle, 1.0, 0.0, 0.0);
    //glTranslatef(yLocation, 0.0, 0.0);

    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();

    //glutWireCube(2.0f); // Render the primitive

    glFlush(); // Flush the OpenGL buffers to the window

    if (movingUp) // If we are moving up
        yLocation -= 0.0035f; // Move up along our yLocation
    else  // Otherwise
        yLocation += 0.0035f; // Move down along our yLocation

    if (yLocation < -3.0f) // If we have gone up too far
        movingUp = false; // Reverse our direction so we are moving down
    else if (yLocation > 3.0f) // Else if we have gone down too far
        movingUp = true; // Reverse our direction so we are moving up

    yRotationAngle += 0.05f; // Increment our rotation value

    if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
        yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
    if(c!=0)
    {
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();
    }
    glTranslatef(x,y,0.0f);
    if(desc=='y')
    {
        glBegin(GL_LINES);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}
void render20()
{


    int max=10,min=-10;
    float p,q;
    srand(time(NULL));
    p=(min+(rand()%(int)(max-min+1)));
    p=p/10;
    q=(min+(rand()%(int)(max-min+1)));
    q=q/10;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // Clear the background of our window to black
    glClear(GL_COLOR_BUFFER_BIT); //Clear the colour buffer (more buffers later on)
    glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
    glTranslatef(p, q, -1.5f); // Push eveything 5 units back into the scene, otherwise we won't see the primitive
//	glTranslatef(0.0f, yLocation, 0.0f); // Translate our object along the y axis
//	glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate our object around the y axis
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 0.0);
    glVertex2f(0, 0); // center of circle
    float radius = 0.35, twicePi = 2 * 3.14;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(0.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.25;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 1.0, 0.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.15;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glBegin(GL_TRIANGLE_FAN); //BEGIN CIRCLE
    glColor3f(1.0, 0.0, 1.0);
    glVertex2f(0, 0); // center of circle
    radius = 0.05f;
    for (int i = 0; i <= 50; i++)
    {
        glVertex2f(radius * cos(i * twicePi / 50), radius * sin(i * twicePi / 50));
    }
    glEnd();
    glFlush(); // Flush the OpenGL buffers to the window

    if(c!=0)
    {

        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,0.0);
        glVertex2f(x, y); // center of circle
        radius=0.05f;
        for (int i = 0; i <= 50; i++)
        {
            glVertex2f ((x + radius * cos(i * twicePi / 50)),(y + radius * sin(i * twicePi / 50)));
        }
        glEnd();

    }
    glTranslatef(x,y,0.0f);

    if(desc=='y')
    {
        glBegin(GL_LINES);

        glColor3f(0.0,0.0,1.0);
        glVertex2f(-0.05f,0.0f);
        glVertex2f(0.05f,0.0f);
        glVertex2f(0.0f,0.05f);
        glVertex2f(0.0f,-0.05f);
        glEnd();
    }
    glutSwapBuffers();
}

void key(unsigned char c, int x, int y)
{
    if(c=='q')
    {
//        cout<<score;
//        pthread_exit(NULL);
        exit(0);
    }
}

void * crender(void * abc)
{
    a *b=(a *)abc;
    glutInit(b->ac,b->av);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE |GLUT_RGBA);
    glutInitWindowSize(h,w);
    glutCreateWindow("GMD");
    int c;
    cout<<"Select levels between 1 to 20:";
    cin>>c;
    if(c==1)
    {
        glutDisplayFunc(render);
        glutIdleFunc(render);
    }
    if(c==2)
    {
        glutDisplayFunc(render1);
        glutIdleFunc(render1);
    }
    if(c==3)
    {
        glutDisplayFunc(render2);
        glutIdleFunc(render2);
    }
    if(c==4)
    {
        glutDisplayFunc(render3);
        glutIdleFunc(render3);
    }
    if(c==5)
    {
        glutDisplayFunc(render4);
        glutIdleFunc(render4);
    }
    if(c==6)
    {
        glutDisplayFunc(render5);
        glutIdleFunc(render5);
    }
    if(c==7)
    {
        glutDisplayFunc(render6);
        glutIdleFunc(render6);
    }
    if(c==8)
    {
        glutDisplayFunc(render7);
        glutIdleFunc(render7);
    }
    if(c==9)
    {
        glutDisplayFunc(render8);
        glutIdleFunc(render8);
    }
    if(c==10)
    {
        glutDisplayFunc(render9);
        glutIdleFunc(render9);
    }
    if(c==11)
    {
        glutDisplayFunc(render10);
        glutIdleFunc(render10);
    }
    if(c==12)
    {
        glutDisplayFunc(render11);
        glutIdleFunc(render11);
    }
    if(c==13)
    {
        glutDisplayFunc(render12);
        glutIdleFunc(render12);
    }
    if(c==14)
    {
        glutDisplayFunc(render13);
        glutIdleFunc(render13);
    }
    if(c==15)
    {
        glutDisplayFunc(render14);
        glutIdleFunc(render14);
    }
    if(c==16)
    {
        glutDisplayFunc(render15);
        glutIdleFunc(render15);
    }
    if(c==17)
    {
        glutDisplayFunc(render16);
        glutIdleFunc(render16);
    }
    if(c==18)
    {
        glutDisplayFunc(render17);
        glutIdleFunc(render17);
    }
    if(c==19)
    {
        glutDisplayFunc(render18);
        glutIdleFunc(render18);
    }
    if(c==20)
    {
        glutDisplayFunc(render19);
        glutIdleFunc(render19);
    }
    if(c==21)
    {
        glutDisplayFunc(render20);
        glutIdleFunc(render20);
    }
    glutReshapeFunc(reshape); // Tell GLUT to use the method "reshape" for reshaping
    glutKeyboardFunc(keyPressed); // Tell GLUT to use the method "keyPressed" for key presses
    glutKeyboardUpFunc(keyUp); // Tell GLUT to use the method "keyUp" for key up events
    //  glutKeyboardFunc(key);

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
    ofstream fout;
    fout.open("score.dat");
    fout<<no_shots<<endl;
    fout<<score_one<<endl;
    fout<<no_hits<<endl;
    fout<<no_miss<<endl;
    fout<<accuracy<<endl;
    fout.close();


    return no_shots;
}
