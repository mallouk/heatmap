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

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define PI 3.14159265358979324626433
#include "getbmp.h"



using namespace std;

static unsigned int pixelData[1];
static int numPixels = 1;



void generatePixels()
{
   // Local storage for bmp image data.
   BitMapFile *image;

   // Load the images.
   image = getbmp("colors.bmp");

   pixelData[image->sizeY * image->sizeX * 3];
   numPixels = image->sizeY * image->sizeX * 3;
   int i = 0;
   for(int j = 0; j < 4*image->sizeY * image->sizeX; j+=4)
   {
       pixelData[i] = image->data[j];
       pixelData[i+1] = image->data[j+1];
       pixelData[i+2] = image->data[j+2];
       i+=3;
   }
}







// Drawing (display) routine.
void drawScene(void){
    // Clear screen to background color.
    glClear(GL_COLOR_BUFFER_BIT);

    // Set drawing color.
    glColor3f(0, 0 ,0);


   glLineWidth(5);
   
   glBegin(GL_LINE_STRIP);
       for (int i = 0; i < numPixels; i+=3){
//           printf("%u, %u, %u\n", pixelData[i], pixelData[i+1], pixelData[i+2]);
//           glColor3ub(pixelData[i], pixelData[i+1], pixelData[i+2]);
           glColor3ub(pixelData[i], 0, 0);
//           glVertex3f(i*2, (pixelData[i] - pixelData[i+1]), 0);
           glVertex3f(i*2, (pixelData[i]), 0);
//       cout << pixelData[i] << endl;   
       }
   glEnd();



   glBegin(GL_LINE_STRIP);
       for (int i = 0; i < numPixels; i+=3){
//           printf("%u, %u, %u\n", pixelData[i], pixelData[i+1], pixelData[i+2]);
//           glColor3ub(pixelData[i], pixelData[i+1], pixelData[i+2]);
           glColor3ub(0, pixelData[i+1], 0);
//           glVertex3f(i*2, (pixelData[i+1]), 0);
//       cout << pixelData[i] << endl;   
       }
   glEnd();


   glBegin(GL_LINE_STRIP);
       for (int i = 0; i < numPixels; i+=3){
//           printf("%u, %u, %u\n", pixelData[i], pixelData[i+1], pixelData[i+2]);
//           glColor3ub(pixelData[i], pixelData[i+1], pixelData[i+2]);
           glColor3ub(0, 0, pixelData[i+2]);
//           glVertex3f(i*2, (pixelData[i+2]), 0);
//       cout << pixelData[i] << endl;   
       }
   glEnd();



    // Flush created objects to the screen, i.e., force rendering.
    glFlush(); 
}

// Initialization routine.
void setup(void){
    // Set background (or clearing) color.
    glClearColor(1.0, 1.0, 1.0, 0.0); 
    generatePixels();
    
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
//    glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   
    glOrtho(-5.0, 150.0, -260.0, 260, -1.0, 1.0);
 
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
      default:
         break;
   }
}

// Main routine: defines window properties, creates window,
// registers callback routines and begins processing.
int main(int argc, char **argv){  
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
