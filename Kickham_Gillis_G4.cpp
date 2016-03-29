/*

Names: Austin Gillis Tyler Kickham
Program Name: Assignment G4- Flowerly Fractal
Purpose: The purpose of this program is to utilize openGL to 
Input:
Output: An openGL window displaying

*/

using namespace std;

#include <stdio.h>
#include <GL/glut.h>
#include <iostream>


using namespace std;

/************************Globals******************************/

static GLsizei width, height; // OpenGL window size.
float topBottomView;
float z = 5.0;

/*************************************************************/



class Tree{

public:
    
    Tree () {};  // constructor

   
};

Tree fractal;

/************************Class Methods***************************/



/*****************************************************************/

// Drawing routine.
void drawScene(void)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity ();

   // Define first viewport.
   float firstHeight= (height/2)-.3333*(height/2);
   glViewport(0, 0, width, firstHeight);

   glColor3f(1.0, 0.0, 0.0);
   glBegin(GL_POLYGON);
      glVertex3f(5.0, 5.0, -5.0);
      glVertex3f(2.0, 5.0, -5.0);
      glVertex3f(2.0, 2.0, -5.0);
   glEnd();
   glColor3f(0.0, 0.0, 0.0);


   
   /*-----------
   Bottom viewport....Menu goes here
   */


   glMatrixMode(GL_MODELVIEW);
   gluLookAt (0.0, 0.0, z, 2.0, 3.5, 0.0, 0.0, 1.0, 0.0);

   float top=(height-firstHeight);
   // Define second viewport.
   glViewport(0, firstHeight, width, top);
   topBottomView = (height/2)+.3333*(height/2);

   glBegin(GL_POLYGON);
      glVertex3f(5.0, 5.0, -5.0);
      glVertex3f(2.0, 5.0, -5.0);
      glVertex3f(2.0, 2.0, -5.0);
   glEnd();

   
   
   // A horizontal black line separates the viewports.
   glColor3f(0.0, 0.0, 0.0);
   glLineWidth(2.0);
   glBegin(GL_LINES);
      glVertex3f(-1.0, 0.0, 0.0);
      glVertex3f(6.5, 0.0, 0.0);
   glEnd();
   glLineWidth(1.0);


   /*-----------
   Top viewport...Fractal goes here
   */



   // End contents of second viewport.

   glFlush();
}

// Initialization routine.
void setup(void) 
{
   glClearColor(1.0, 1.0, 1.0, 0.0);
   glShadeModel (GL_FLAT);
   glEnable (GL_DEPTH_TEST);
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glFrustum (-1, 1, -1, 1, 1.5, 20.0);
   glMatrixMode(GL_MODELVIEW);

   // Pass the size of the OpenGL window to globals.
   width = w;
   height = h;
}

// Mouse input processing routine.
void mouse (int button, int state, int x, int y)
{
   if (y < topBottomView)
      cout << "top\n";
   else
      cout << "bottom\n";
}

void keyboard (unsigned char key, int x, int y)
{
   switch (key) {
      case '8': z = z + 1;
      cout << "z = " << z << endl;
                glutPostRedisplay ();
                break;
      case '2': z = z - 1;
      cout << "z = " << z << endl;
                glutPostRedisplay ();
                break;
      case 'q': exit (1);
   }
}

// Main routine.
int main(int argc, char **argv) 
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("Flowerly Fractal");
   setup();
   glutDisplayFunc(drawScene);
   glutReshapeFunc(resize);
   glutMouseFunc(mouse);
   glutKeyboardFunc (keyboard);
   glutMainLoop();

   return 0;
}

