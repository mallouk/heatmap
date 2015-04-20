//////////////////////////////////////////////////////////////////////////////////////////// 
// textured_surface.cpp
// modified from Sumanta Guha

#include <cstdlib>
#include <iostream>
#include <fstream>

#ifdef __APPLE__
#  include <GL/glew.h>
#include <GLUT/glut.h>
#  include <OpenGL/glext.h>
#else
#  include <GL/glew.h>
#include <GL/glut.h>
#  include <GL/glext.h>
#pragma comment(lib, "glew32.lib") 
#endif

#include "getbmp.h"

using namespace std;

// Globals.
static unsigned int texture[1]; // Array of texture indices.

// Load external textures.
void loadExternalTextures()			
{
   // Local storage for bmp image data.
   BitMapFile *image[1];

   // Load the images.
   image[0] = getbmp("color_test.bmp");
   
   // Bind grass image to texture object texture[0]. 
   glBindTexture(GL_TEXTURE_2D, texture[0]); 
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[0]->sizeX, image[0]->sizeY, 0, 
	            GL_RGBA, GL_UNSIGNED_BYTE, image[0]->data);


   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

}

static int xres=100;
static int zres=50;


static float* surfaceVerts;
static int* surfaceTris;
static float* surfaceTexs;

// nx, nz >= 2
void createSurface(float minX, float maxX, float minZ, float maxZ, int nx, int nz)
{
    float dx = (maxX - minX) / (nx-1); // delta coord x
    float dz = (maxZ - minZ) / (nz-1); // delta coord z
    
    float dtx = 1.0 / (nx-1);     // delta texture x
    float dtz = 1.0 / (nz-1);     // delta texture z

    surfaceVerts = new float[nx*nz*3];
    surfaceTexs = new float[nx*nz*2];
    surfaceTris = new int[(nx-1)*(nz-1)*6];

    int vp=0;
    int tp=0;
 
    // vertex data
    for (int row=0; row<nz; row++)
    {
        for (int col=0; col<nx; col++)
        {
            surfaceVerts[vp++]=col*dx+minX;
            surfaceVerts[vp++]=0;
            surfaceVerts[vp++]=row*dz+minZ;
            
            surfaceTexs[tp++]=col*dtx;
            surfaceTexs[tp++]=row*dtz;
        }
    }

    // triangle data
    tp=0;
    for (int row=0; row<nz-1; row++)
    {
        for (int col=0; col<nx-1; col++)
        {
            surfaceTris[tp++]=row*nx+col;
            surfaceTris[tp++]=row*nx+col+1;
            surfaceTris[tp++]=row*nx+col+1+nx;

            surfaceTris[tp++]=row*nx+col;
            surfaceTris[tp++]=row*nx+col+1+nx;
            surfaceTris[tp++]=row*nx+col+nx;
        }
    }
}



// Initialization routine.
void setup(void)
{    
    glClearColor(1,1,1,1);


   // Create texture ids.
   glGenTextures(2, texture);
   // load textures
   loadExternalTextures();
   // Specify how texture values combine with current surface color values.
   glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE); 

   createSurface(-20, 20, 20, -20, xres, zres);
   

   // enable stuff
   glEnable(GL_DEPTH_TEST); 
   glEnable(GL_TEXTURE_2D); 


   // Cull back faces.
   // glEnable(GL_CULL_FACE);
   // glCullFace(GL_BACK);
}


// Drawing routine.
void drawScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();

    gluLookAt(0.0, 10.0, 20.0, 0.0, 0, -15, 0.0, 1.0, 0.0);

    // Map the texture
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindTexture(GL_TEXTURE_2D, texture[0]);        
    // glColor3f(0,0,0);

    // draw the surface
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, surfaceVerts);
    glTexCoordPointer(2, GL_FLOAT, 0, surfaceTexs);


    glDrawElements(GL_TRIANGLES, (zres-1)*(xres-1)*6, GL_UNSIGNED_INT, surfaceTris);

    glutSwapBuffers();	
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glViewport(0, 0, w, h);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0);
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


// Main routine.
int main(int argc, char **argv) 
{
   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("textured_surface.cpp");
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutKeyboardFunc(keyInput);

   setup(); 

   glutMainLoop(); 
}


// Local Variables:
// compile-command: "g++ -framework OpenGL -framework GLUT -Wall -Wno-deprecated textured_surface.cpp getbmp.cpp -lglew"
// End:

