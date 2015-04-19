/** Color selection program that allows the user to select an object
 *  on the screen. The object that is selected will change color.
 *  Also, the arrow keys will cause movement in the world.
 *  3-08-05
 *  CPSC 4500 - Graphical Algorithms
 *  Assignment 4 
 *  Matthew Jallouk
 *  Written on a Linux Ubuntu Distribution in Emacs.
 *
 */

# include <iostream>
#include <math.h>
#ifdef __APPLE__
# include <GLUT/glut.h>
#else
# include <GL/glut.h>
#endif

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358797932

enum ObjType { CUBE, TEAPOT, CONE, SPHERE, DODECA };

typedef struct
{
    ObjType type;

    float x;
    float y;
    float z;

    int selected;
    int rotation;
} object;


static const int num_objects=15;

static object objects[] =
{ { CONE, 1, 2, -10, 0, 0},
  { CUBE, 0, 0, -10, 0, 0 },
  { TEAPOT, -4, 0, -10, 0 },
  { CONE, 4, 0, -10, 0, 0 },
  { CONE, 4, 0, -19, 0, 0 },
  { CONE, -2, 2, -12, 0, 180 },
  { TEAPOT, -7, 2, -12, 0 },
  { SPHERE, -4, 2, -8, 0, 0},
  { DODECA, 6, 4, -10, 0, 0},
  { SPHERE, 3, 7, -8, 0, 0},
  { TEAPOT, -1, 5, -12, 0 },
  { DODECA, -6, 4, -15, 0, 0},
  { DODECA, 5, 6, -19, 0, 0},
  { CUBE, 1, 7, -10, 0, 45 },
  { CONE, 8, 1, -10, 0, 90 }
};


static float my_x=0;
static float my_y=0;
static float my_z=0;
static float my_angle=0;
static int isDragging = 0;
static unsigned int buffer[1024]; // Hit buffer.
static int isSelecting = 1;
static unsigned int isSelected;
static int windowX = 500;
static int windowY = 500;
static int shifter = 0;

void drawFloor(){
    glColor3f(.5,.5,.5);
    glBegin(GL_POLYGON);
       glVertex3f(-100,-1,-100);
       glVertex3f(100,-1,-100);
       glVertex3f(100,-1,100);
       glVertex3f(-100,10,100);
    glEnd();
}



void drawObject(const object& o, int i){
    glPushMatrix();
    glTranslatef(o.x, o.y, o.z);
    glLineWidth(1);
    
    if (o.selected){
        glColor3f(1.0, 0.0, 0.0);
    }else{
        glColor3f(0.0, 1.0, 0.0);
    }
    
    switch (o.type){
     case CUBE:
         if (isSelecting) glLoadName(i);
         glRotatef(o.rotation, 0, 0, 1);
         glutSolidCube(1);
         glColor3f(0,0,0);
         glutWireCube(1);
         break;

     case TEAPOT:
         if (isSelecting) glLoadName(i);
         glRotatef(o.rotation, 0, 0, 1);
         glutSolidTeapot(1);
         glColor3f(0,0,0);
         glutWireTeapot(1);
         break;

     case CONE:
         if (isSelecting) glLoadName(i);
         glRotatef(o.rotation, 0, 0, 1);
         glTranslatef(0, -.5, 0);
         glRotatef(90, -1, 0, 0);
         glScalef(0.25, 0.25, 0.25);
         glutSolidCone(5, 10, 3, 3);
         glColor3f(0,0,0);
         glutWireCone(5, 10, 3, 3);
         break;

    case SPHERE:
        if (isSelecting) glLoadName(i);
        glRotatef(o.rotation, 1, 0, 0);
        glutSolidSphere(1, 36, 36);
        glColor3f(0,0,0);
        glutWireSphere(1, 36, 36);
        break;
        
    case DODECA:
        if (isSelecting) glLoadName(i);
        glRotatef(o.rotation, 1, 0, 0);
        glutSolidDodecahedron();
        glColor3f(0,0,0);
        glutWireDodecahedron();
        break;
        
    }
    glPopMatrix();
}



// Drawing routine.
void drawScene(void){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    // calculate heading
    float radiansX;
    float radiansY;
    float radiansZ;
    
    if (shifter == 0){ //Side to side looking
        radiansX = PI*my_angle/180;
        gluLookAt(my_x, my_y, my_z,
                  my_x - sin(radiansX), my_y - sin(radiansY), my_z - cos(radiansX),
                  0, 1, 0);
    }else if (shifter == 1){ //Up and down looking
        radiansY = PI*my_angle/180;
        gluLookAt(my_x, my_y, my_z,
                  my_x - sin(radiansX), my_y - sin(radiansY), my_z - cos(radiansX),
                  0, 1, 0);
    }
    
    drawFloor();
    for (int i=0; i<num_objects; i++){
        drawObject(objects[i], i);
    }

    glutSwapBuffers();
}


void pickFunction(int button, int state, int x, int y){

    int viewport[4]; // Viewport data.
        
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN) return; // Don't react unless left button is pressed.
   
    glGetIntegerv(GL_VIEWPORT, viewport); // Get viewport data.

    glSelectBuffer(1024, buffer); // Specify buffer to write hit records in selection mode
    glRenderMode(GL_SELECT); // Enter selection mode.

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
   
    // Define a viewing volume corresponding to selecting in 3 x 3 region around the cursor.
    glLoadIdentity();
    gluPickMatrix((float)x, (float)(viewport[3] - y), 3.0, 3.0, viewport);
    // glFrustum(-5.0, 5.0, -5.0, 5.0, 5.0, 100.0); // Copied from the reshape routine.
    gluPerspective(50, 1, 0.1, 250);


    float radiansX;
    float radiansY;
    float radiansZ;
    
    if (shifter == 0){ //Side to side looking
        radiansX = PI*my_angle/180;
        gluLookAt(my_x, my_y, my_z,
                  my_x - sin(radiansX), my_y - sin(radiansY), my_z - cos(radiansX),
                  0, 1, 0);
    }else if (shifter == 1){ //Up and down looking
        radiansY = PI*my_angle/180;
        gluLookAt(my_x, my_y, my_z,
                  my_x - sin(radiansX), my_y - sin(radiansY), my_z - cos(radiansX),
                  0, 1, 0);
    }



    glMatrixMode(GL_MODELVIEW); // Return to modelview mode before drawing.
    glLoadIdentity();

    glInitNames(); // Initializes the name stack to empty.
    glPushName(0); // Puts name 0 on top of stack.

    isSelecting=1;
    
    for (int i=0; i<num_objects; i++){
        drawObject(objects[i], i);
    }
    
    isSelecting = 0;
    glPopName();
   
    int hits = glRenderMode(GL_RENDER);

    //printf("clicked on %d thing[s]\n", hits);
    
    if (hits > 0){
        unsigned int* ptr = buffer;
        unsigned int closest_image;
        unsigned int closest_image_stack;

        for (int i=0; i<hits; i++){
            ptr++;
            if (i == 0){
                closest_image = *ptr;
            }
            
            unsigned int minZ = *ptr;
            ptr +=2;
            unsigned int stack_top = *ptr;
            ptr++;
            
            if ((closest_image*1.0/0xffffffff) >= (minZ*1.0/0xffffffff) ){
                closest_image = minZ;
                //printf("closest: %f: %f\n", closest_image*1.0/0xffffffff, minZ*1.0/0xffffffff);
                closest_image_stack = stack_top;
                //printf("closest: %u\n", closest_image_stack);
                
            }
            for (int i = 0; i < num_objects; i++){
                objects[i].selected = 0;
            }

            objects[closest_image_stack].selected = 1;
            //printf("item %u at %f\n", stack_top, minZ*1.0/0xffffffff);
        }
        
    }else{
        for (int i = 0; i < num_objects; i++){
            objects[i].selected = 0;
        }
        isSelected = 0;
    }
    


    // Restore viewing volume of the resize routine and return to modelview mode.
   glMatrixMode(GL_PROJECTION);
   glPopMatrix();
   glMatrixMode(GL_MODELVIEW);
   glutPostRedisplay();
}




// Initialization routine.
void setup(void){
    glClearColor(1.0, 1.0, 1.0, 0.0); 
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset( 1, 1);
}



// OpenGL window reshape routine.
void resize(int w, int h){
    float aspect = w*1.0/h;
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50, aspect, 0.1, 250);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// Keyboard input processing routine.
void keyInput(unsigned char key, int x, int y){
   switch(key){
   case 27:                  // escape
       exit(0);
       break;
   default:
       break;
   }
}

void specialKeyInput(int key, int x, int y)
{
    switch (key){
       case GLUT_KEY_LEFT: 
           my_angle+=5;
           break;
       case GLUT_KEY_RIGHT: 
           my_angle-=5;
           break;
       case GLUT_KEY_UP:
           my_x -= 1*sin((M_PI/180.0)*my_angle);
           my_z -= 1*cos((M_PI/180.0)*my_angle);
           break;
       case GLUT_KEY_DOWN:
           my_x += 1*sin((M_PI/180.0)*my_angle);
           my_z += 1*cos((M_PI/180.0)*my_angle);
           break;
    }
    
    if (my_angle > 360.0) my_angle -= 360;
    if (my_angle < 0) my_angle += 360;

//    printf("my angle: %f\n", my_angle);
    
    glutPostRedisplay();
}

void trackMouse(int x, int y){
    //Handles movement when facing up/down.
    if (y <= .3*windowY){
        shifter = 1;
        my_angle-=.5;
    }else if (y > .7*windowY){
        shifter = 1;
        my_angle+=.5;
    }
    
//Handleds movment when facing left/right.
    if (x <= .4*windowX){
        shifter = 0;
        my_angle+=0.5;
    }else if (x > .7*windowX){
        shifter = 0;
        my_angle-=.5;
    }
    
    glutPostRedisplay();
}


// Main routine.
int main(int argc, char **argv){
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA); 
   glutInitWindowSize(windowX, windowY);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("Pick an object!");
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutKeyboardFunc(keyInput);
   glutSpecialFunc(specialKeyInput);
   glutPassiveMotionFunc(trackMouse);
   glutMouseFunc(pickFunction);
   
   setup(); 
   
   glutMainLoop(); 
}

