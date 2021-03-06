/** Heatmap program
 *
 *  4-13-15
 *  CPSC 4500 - Graphical Algorithms
 *  Semester Project
 *  Matthew Jallouk
 *  Written on Linux Ubuntu Distribution in EMACS
 */

#include <iostream>
#include <math.h>
#include <stdio.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.14159265358979324626433
#include "getbmp.h"



using namespace std;

//Struct to store a pixel
struct Pixel
{
    float red;
    float green;
    float blue;
};

//Define globals
static float rotateAngle = 0;
static int xVar = 0;
static int yVar = 5;
static int zVar = -15;
static unsigned int pixelData[1];
static int numPixels = 1;
static Pixel pixels[1];


//Method that scans the pixels in the image and returns our array of pixels;
void generatePixels()
{
   // Local storage for bmp image data.
   BitMapFile *image;

   // Load the images.
   image = getbmp("color_test.bmp");

   pixelData[image->sizeY * image->sizeX * 3];
   numPixels = image->sizeY * image->sizeX;
   pixels[numPixels];


   //Extract pixel data from returned image
   int i = 0;
   for(int j = 0; j < 4*image->sizeY * image->sizeX; j+=4)
   {
       pixelData[i] = image->data[j];
       pixelData[i+1] = image->data[j+1];
       pixelData[i+2] = image->data[j+2];
       i+=3;
   }

   //Generate pixel object array
   int x = 0;
   for (int j = 0; j < image->sizeY * image->sizeX * 4; j+=4){
              pixels[x].red = image->data[j];
              pixels[x].green = image->data[j+1];
              pixels[x].blue= image->data[j+2];
              x++;
   }
}


//Method that takes the generated pixels and smooths out the points to a smooth sine curve like
//form.
void smoothPixels()
{
    int smoothDepth = 5;
    for (int x = 0; x < smoothDepth; x++){
        for (int i = 0; i < numPixels; i++){
            if (i == 0){
                pixels[i].red = (pixels[i].red + pixels[i+1].red)/2;
                pixels[i].green = (pixels[i].green + pixels[i+1].green)/2;
                pixels[i].blue = (pixels[i].blue+ pixels[i+1].blue)/2;
            }else if (i == (numPixels - 1)){
                pixels[i].red = (pixels[i].red + pixels[i-1].red)/2;
                pixels[i].green = (pixels[i].green + pixels[i-1].green)/2;
                pixels[i].blue = (pixels[i].blue+ pixels[i-1].blue)/2;
            }else{
                pixels[i].red = (pixels[i-1].red + pixels[i].red + pixels[i+1].red )/3;
                pixels[i].green = (pixels[i-1].green + pixels[i].green + pixels[i+1].green )/3;
                pixels[i].blue = (pixels[i-1].blue + pixels[i].blue + pixels[i+1].blue )/3;
            }
        } 
    }   
}

//Method that draws the red curve on the OpenGL screen
void drawRedCurve()
{
    int x = 0, z = -4;
    int caseSys = 1;
    glBegin(GL_TRIANGLES);
       for (int i = 0; i < 2*numPixels; i++){
           glColor3f(1, 0, 0);

           if (caseSys == 1){
               glVertex3f(x - numPixels/2, (pixels[x].red) - 125, z);
               z+=4;
               caseSys=2;
           }else if (caseSys == 2){
               glVertex3f(x - numPixels/2, (pixels[x].red) - 125, z);
               z-=4;
               x++;
               caseSys=3;
           }else if (caseSys == 3){
               glVertex3f(x - numPixels/2, (pixels[x].red) - 125, z);
               caseSys=1;
           }
       }
    glEnd();
    
    x = 0, z = 0;
    caseSys = 1;
    glBegin(GL_TRIANGLES);
       for (int i = 0; i < 2*numPixels; i++){
           glColor3f(1, 0, 0);

           if (caseSys == 1){
               glVertex3f(x - numPixels/2, (pixels[x].red) - 125, z);
               z-=4;
               x++;
               caseSys=2;
           }else if (caseSys == 2){
               glVertex3f(x - numPixels/2, (pixels[x].red) - 125, z);
               z+=4;
               caseSys=3;
           }else if (caseSys == 3){
               glVertex3f(x - numPixels/2, (pixels[x].red) - 125, z);
               caseSys=1;
           }
       }
    glEnd();
}


//Method that draws the green curve on the OpenGL screen.
void drawGreenCurve()
{
    int x = 0, z = -4;
    int caseSys = 1;
    glBegin(GL_TRIANGLES);
       for (int i = 0; i < 2*numPixels; i++){
           glColor3f(0, 1, 0);

           if (caseSys == 1){
               glVertex3f(x - numPixels/2, (pixels[x].green) - 125, z);
               z+=4;
               caseSys=2;
           }else if (caseSys == 2){
               glVertex3f(x - numPixels/2, (pixels[x].green) - 125, z);
               z-=4;
               x++;
               caseSys=3;
           }else if (caseSys == 3){
               glVertex3f(x - numPixels/2, (pixels[x].green) - 125, z);
               caseSys=1;
           }
       }
    glEnd();
    
    x = 0, z = 0;
    caseSys = 1;
    glBegin(GL_TRIANGLES);
       for (int i = 0; i < 2*numPixels; i++){
           glColor3f(0, 1, 0);

           if (caseSys == 1){
               glVertex3f(x - numPixels/2, (pixels[x].green) - 125, z);
               z-=4;
               x++;
               caseSys=2;
           }else if (caseSys == 2){
               glVertex3f(x - numPixels/2, (pixels[x].green) - 125, z);
               z+=4;
               caseSys=3;
           }else if (caseSys == 3){
               glVertex3f(x - numPixels/2, (pixels[x].green) - 125, z);
               caseSys=1;
           }
       }
    glEnd();
}

//Method that draws the blue curve on the OpenGL screen.
void drawBlueCurve()
{
    int x = 0, z = -4;
    int caseSys = 1;
    glBegin(GL_TRIANGLES);
       for (int i = 0; i < 2*numPixels; i++){
           glColor3f(0, 0, 1);

           if (caseSys == 1){
               glVertex3f(x - numPixels/2, (pixels[x].blue) - 125, z);
               z+=4;
               caseSys=2;
           }else if (caseSys == 2){
               glVertex3f(x - numPixels/2, (pixels[x].blue) - 125, z);
               z-=4;
               x++;
               caseSys=3;
           }else if (caseSys == 3){
               glVertex3f(x - numPixels/2, (pixels[x].blue) - 125, z);
               caseSys=1;
           }
       }
    glEnd();
    
    x = 0, z = 0;
    caseSys = 1;
    glBegin(GL_TRIANGLES);
       for (int i = 0; i < 2*numPixels; i++){
           glColor3f(0, 0, 1);

           if (caseSys == 1){
               glVertex3f(x - numPixels/2, (pixels[x].blue) - 125, z);
               z-=4;
               x++;
               caseSys=2;
           }else if (caseSys == 2){
               glVertex3f(x - numPixels/2, (pixels[x].blue) - 125, z);
               z+=4;
               caseSys=3;
           }else if (caseSys == 3){
               glVertex3f(x - numPixels/2, (pixels[x].blue) - 125, z);
               caseSys=1;
           }
       }
    glEnd();
}





// Drawing (display) routine.
void drawScene(void){
    // Clear screen to background color.
    glClear(GL_COLOR_BUFFER_BIT);

    //Draw floor
    glColor3f(.5,.5,.5);
    glBegin(GL_POLYGON);
       glVertex3f(-200, -100, -700);
       glVertex3f(200, -100, -700);
       glVertex3f(200, -100, 700);
       glVertex3f(-200, 10, 700);
    glEnd();


    // Set drawing color.
    glColor3f(0, 0 ,0);

    glPushMatrix();

    //Set camera view.
    gluLookAt(xVar, yVar, zVar, 
              xVar, yVar, 0,
              0, 1, 0);
    
    glRotatef(rotateAngle, 0, 1, 0);
    
    //Draw curves
    glLineWidth(3);
    drawRedCurve();
    drawGreenCurve();
    drawBlueCurve();
    
 
    glPopMatrix();
    
    // Flush created objects to the screen, i.e., force rendering.
    glFlush(); 
}

// Initialization routine.
void setup(void){
    // Set background (or clearing) color.
    glClearColor(1.0, 1.0, 1.0, 0.0); 
    generatePixels();
    smoothPixels();
    

}

// OpenGL window reshape routine.
void resize(int w, int h){
    // Set viewport size to be entire OpenGL window.
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    
    // Set matrix mode to projection.
    glMatrixMode(GL_PROJECTION);
    
    // Clear current projection matrix to identity.
    glLoadIdentity();
    
    // Specify the orthographic (or perpendicular) projection, 
    // i.e., define the viewing box.
    gluPerspective(170, .2, 1, 25);

    // Set matrix mode to modelview.
    glMatrixMode(GL_MODELVIEW);
    
    // Clear current modelview matrix to identity.
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y){
   switch(key){
   case 27:  // Press escape to exit.
       exit(0);
       break;
   case 'r':
       rotateAngle+=1;
       glutPostRedisplay();
       break;
   case 'e':
       rotateAngle-=1;
       glutPostRedisplay();
       break;
   case 's':
       zVar--;
       glutPostRedisplay();
       break;
   case 'w':
       zVar++;
       glutPostRedisplay();
       break;
   case 'a':
       xVar++;
       glutPostRedisplay();
       break;
   case 'd':
       xVar--;
       glutPostRedisplay();
       break;
   case 'j':
       yVar++;
       glutPostRedisplay();
       break;
   case 'k':
       yVar--;
       glutPostRedisplay();
       break;       
   default:
       break;
   }
}

// Main routine: defines window properties, creates window,
// registers callback routines and begins processing.
int main(int argc, char **argv){  


    cout << "R key - Rotate in + direction across the Y axis." << endl;
    cout << "E key - Rotate in - direction across the Y axis." << endl;
    cout << "S key - move back one in the Z direction." << endl;
    cout << "W key - move forward one in the Z direction." << endl;
    cout << "A key - move to the left, X axis." << endl;
    cout << "D key - move to the right, X axis." << endl;
    cout << "J key - move up in the y direction." << endl;
    cout << "K key - move down in the y direction." << endl;
    cout << "" << endl;
    cout << "" << endl;

   // Initialize GLUT.
   glutInit(&argc, argv);
 
   // Set display mode as single-buffered and RGB color.
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
   
   // Set OpenGL window size.
   glutInitWindowSize(500, 500);

   // Set position of OpenGL window upper-left corner.
   glutInitWindowPosition(100, 100); 
   
   // Create OpenGL window with title.
   glutCreateWindow("Heatmap!");
   // Initialize.
   setup();
   // Register display routine.
   glutDisplayFunc(drawScene);
   // Register reshape routine.
   glutReshapeFunc(resize);
   // Register keyboard routine.
   glutKeyboardFunc(keyInput);
   // Begin processing.
   glutMainLoop();
   return 0;  
}
