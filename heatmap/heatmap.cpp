/** Heatmap Program
 *
 *  4-14-15
 *  CPSC 4500 - Graphical Algorithms
 *  Semester Project - Heatmap
 *  Matthew Jallouk
 *  Written on Linux Ubuntu Distribution in Emacs
 */

#include <cstdlib>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GL/glew.h>
#  include <GLUT/freeglut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#  include <GL/freeglut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

#include "getbmp.h"

using namespace std;

// Globals.
static float d = 0.0; // Distance parameter in gluLookAt().
static float angle = 0;
static unsigned int pixelData[1];

static int numPixels = 1;


// Load external textures.
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

// Initialization routine.
void setup(void)
{    
   glClearColor(0.0, 0.0, 0.0, 0.0);
   glEnable(GL_DEPTH_TEST); 

   // Load external textures.
   generatePixels();
}

// Drawing routine.
void drawScene(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   glLineWidth(5);
   
   glBegin(GL_LINE_STRIP);
       for (int i = 0; i < numPixels; i+=3){
//           printf("%u, %u, %u\n", pixelData[i], pixelData[i+1], pixelData[i+2]);
//           glColor3ub(pixelData[i], pixelData[i+1], pixelData[i+2]);
           glColor3ub(pixelData[i], 0, 0);
           glVertex3f(i*2, (pixelData[i]), 0);
//       cout << pixelData[i] << endl;   
       }
   glEnd();
   

   glBegin(GL_LINE_STRIP);
       for (int i = 0; i < numPixels; i+=3){
//           printf("%u, %u, %u\n", pixelData[i], pixelData[i+1], pixelData[i+2]);
//           glColor3ub(pixelData[i], pixelData[i+1], pixelData[i+2]);
           glColor3ub(0, pixelData[i+1], 0);
           glVertex3f(i*2, (pixelData[i+1]), 0);
//       cout << pixelData[i] << endl;   
       }
   glEnd();


   glBegin(GL_LINE_STRIP);
       for (int i = 0; i < numPixels; i+=3){
//           printf("%u, %u, %u\n", pixelData[i], pixelData[i+1], pixelData[i+2]);
//           glColor3ub(pixelData[i], pixelData[i+1], pixelData[i+2]);
           glColor3ub(0, 0, pixelData[i+2]);
           glVertex3f(i*2, (pixelData[i+2]), 0);
//       cout << pixelData[i] << endl;   
       }
   glEnd();




   glLoadIdentity();
   // gluLookAt(0.0, 10.0, 15.0 + d, 0.0, 10.0, d, 0.0, 1.0, 0.0);

   glutSwapBuffers();	
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
//   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);

    glOrtho(-5.0, 150.0, -5.0, 260, -1.0, 1.0);   


   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y)
{
   switch(key) 
   {
      case 27:
          exit(0);
         break;
      default:
         break;
   }
}

// Callback routine for non-ASCII key entry.
void specialKeyInput(int key, int x, int y)
{
   if (key == GLUT_KEY_UP) 
   {
      if (d > -50.0) d -= 0.1;
   }
   if (key == GLUT_KEY_DOWN) 
   {
      if (d < 15.0) d += 0.1;
   }
   glutPostRedisplay();
}

// Routine to output interaction instructions to the C++ window.
void printInteraction(void)
{
   cout << "Interaction:" << endl;
   cout << "Press the up and down arrow keys to move the viewpoint over the field." << endl;
}

void update()
{
    angle+=1;
    if (angle == 360){ angle = 0;}
    glutPostRedisplay();
}


// Main routine.
int main(int argc, char **argv) 
{
   printInteraction();
   glutInit(&argc, argv);

   glutInitContextVersion(2, 1);
   glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Heatmap!");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutIdleFunc(update);
   

   glewExperimental = GL_TRUE;
   glewInit();

   setup(); 

   glutMainLoop(); 
}
