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
   glClear(GL_COLOR_BUFFER_BIT);

   // Define first viewport.
   float firstHeight= (height/2)-.3333*(height/2);
   glViewport(0, 0, width, firstHeight); 

   
   /*-----------
   Bottom viewport....Menu goes here
   */



   float top=(height-firstHeight);
   // Define second viewport.
   glViewport(0, firstHeight, width, top);
   topBottomView = (height/2)+.3333*(height/2);

   
   
   // A horizontal black line separates the viewports.
   glColor3f(0.0, 0.0, 0.0);
   glLineWidth(2.0);
   glBegin(GL_LINES);
      glVertex3f(0, 0.1, 0.0);
     glVertex3f(100.0,0.1, 0.0);
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
}

// OpenGL window reshape routine.
void resize(int w, int h)
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(0.0, 100.0, 0.0, 100.0, -1.0, 1.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();

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

// Main routine.
int main(int argc, char **argv) 
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB); 
   glutInitWindowSize(500, 500);
   glutInitWindowPosition(100, 100); 
   glutCreateWindow("viewports.cpp");
   setup(); 
   glutDisplayFunc(drawScene); 
   glutReshapeFunc(resize);  
   glutMouseFunc(mouse);
   glutMainLoop(); 

   return 0;  
}

