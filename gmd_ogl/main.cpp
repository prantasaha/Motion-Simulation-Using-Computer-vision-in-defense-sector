#include<windows.h>
#include<GL/gl.h>
#include<glut.h>
#include<stdio.h>
#include<math.h>
#define pi 3.14

GLfloat angle = 0.0f;
GLfloat refmills=20;
GLfloat bx=0.0,by=0.0,br=0.5,xs=0.02,ys=0.007,bxMax,byMax,bxMin,byMin;
GLdouble clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop;

void InitGL(){
    glClearColor(0.0f,0.0f,0.0f,0.1f);
}
void render(){
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    /*
    glPushMatrix();
    glTranslatef(-0.5,0.4,0.0);
    glBegin(GL_QUADS);
      glColor3f(1.0f, 0.0f, 0.0f);
      glVertex2f(0.0f, 0.5f);
      glVertex2f(0.0f, 0.0f);
      glVertex2f(0.5f, 0.0f);
      glVertex2f(0.5f, 0.5f);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.5,-0.4,0.0);
    glRotatef(angle, 0.0, 1.0, 0.0);
    glBegin(GL_QUADS);
      glColor3f(0.0f, 1.0f, 0.0f);
      glVertex2f(0.0f, 0.5f);
      glVertex2f(0.0f, 0.0f);
      glVertex2f(0.5f, 0.0f);
      glVertex2f(0.5f, 0.5f);
    glPopMatrix();
    glEnd();glTranslatef(x,0.0f,0.0f);
    glutSwapBuffers();
    angle += 1.0f;
    */
    glTranslatef(bx,by,0.0);
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0,0.0,1.0);
        glVertex2f(0.0,0.0);
        int numSegments=100;
            for(int i=0;i<=numSegments;i++){
                angle=i*2.0*pi/numSegments;
                glVertex2f(cos(angle)*br,sin(angle)*br);
            }
    glEnd();
    glutSwapBuffers();
    bx += xs;
    by += ys;

   if (bx > bxMax) {
      bx = bxMax;
      xs = -xs;
   } else if (bx < bxMin) {
      bx = bxMin;
      xs = -xs;
   }
   if (by > byMax) {
      by = byMax;
      ys = -ys;
   } else if (by < byMin) {
      by = byMin;
      ys = -ys;
   }
}
void Timer(int value){
    //glRotatef(0.10,0.0,1.0,0.0);
    glutPostRedisplay();
    glutTimerFunc(refmills,Timer,0);
}
void reshape(GLsizei width, GLsizei height){
    if(height==0){
       height=1;
    }
    GLfloat aspect= (GLfloat)width/(GLfloat)height;
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (width >= height) {
      clipAreaXLeft   = -1.0 * aspect;
      clipAreaXRight  = 1.0 * aspect;
      clipAreaYBottom = -1.0;
      clipAreaYTop    = 1.0;
   } else {
      clipAreaXLeft   = -1.0;
      clipAreaXRight  = 1.0;
      clipAreaYBottom = -1.0 / aspect;
      clipAreaYTop    = 1.0 / aspect;
   }
   gluOrtho2D(clipAreaXLeft, clipAreaXRight, clipAreaYBottom, clipAreaYTop);
   bxMin = clipAreaXLeft + br;
   bxMax = clipAreaXRight - br;
   byMin = clipAreaYBottom + br;
   byMax = clipAreaYTop - br;
}
void keybrd(unsigned char c,int x,int y){
    if(c==27){
        exit(0);
    }
}
main(int argc, char **argv){
    glutInit(&argc,argv);

    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(500,500);
    glutCreateWindow("GMD");
    glutDisplayFunc(render);
    glutReshapeFunc(reshape);
    InitGL();
    glutTimerFunc(0,Timer,0);
    glutKeyboardFunc(keybrd);
    glutMainLoop();
    return 0;
}
