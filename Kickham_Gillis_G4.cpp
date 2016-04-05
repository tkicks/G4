/*

Names: Austin Gillis Tyler Kickham
Program Name: Assignment G4- Flowerly Fractal
Purpose: The purpose of this program is to utilize openGL to 
Input:
Output: An openGL window displaying

*/

using namespace std;

// #include <stdio.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <pthread.h>


using namespace std;

/************************Globals******************************/

static GLsizei width, height; // OpenGL window size.

float z = 5.0;

static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;

/*************************************************************/


void* simpleFunc(void*) { return NULL; }
void forcePThreadLink() { pthread_t t1; pthread_create(&t1, NULL, &simpleFunc, NULL); }

class Tree{

public:
    
    Tree () {};  // constructor

    void readIn(char* inFilename);

    vector<string> grammars;   
};

Tree fractal;

/************************Class Methods***************************/

void Tree::readIn(char* inFilename){

   ifstream inFile (inFilename);
   string line;
   
   if (inFile.is_open()) {
   
      cout << "open\n";
      while ( getline (inFile,line) ){
         grammars.push_back(line);
      }


   }

   else{
      cout<<"Input file could not be opened. Terminating..."<<endl;
   }


}


/*****************************************************************/

// Drawing routine.
void drawScene(void)
{

   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   
   /*-----------
   Bottom viewport....Menu goes here
   */

   float firstHeight= (height/2)-.3333*(height/2);

   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   glOrtho (-1, 1, -1, 1, -1, 1);
   glViewport(0, 0, width, firstHeight);
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity (); 

   glColor3f (0.0, 0.0, 0.0);
   glPointSize(6.0);
   glBegin(GL_POINTS);
      
      for (float i = -1; i <= 1; i+=.01){
         
         glVertex3f(i,.9,0);
      }

   glEnd(); 


   //--------------------------END MENU VIEWPORT---------------------------


   /*-----------
   Top viewport...Fractal goes here
   */

   float top=(height-firstHeight);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity ();
   glFrustum (-1, 1, -1, 1, 1.5, 20.0);
   gluLookAt (0, 0, z, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

   glViewport(0, firstHeight, width, top);
   glMatrixMode (GL_MODELVIEW);
   glLoadIdentity ();

   // Commands to turn the hemisphere.
   glRotatef(Zangle, 0.0, 0.0, 1.0);
   glRotatef(Yangle, 0.0, 1.0, 0.0);
   glRotatef(Xangle, 1.0, 0.0, 0.0);

   glColor3f (0.0, 0.0, 1.0);
   glutWireTeapot (0.7);

   //--------------------------END Fractal VIEWPORT---------------------------

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
   if (state == GLUT_DOWN) {
      float topBottomView= (height/2)+.3333*(height/2);

      // multiply x and y by width and height (750x750)
      // when clicking buttons to get x and y to check if clicked

      // top left button
      if ((x < 118 & x > 18) & (y < 574.2 & y > 508))
         cout << "pressed button 1\n";


      // say where x and y clicked are and if top or bottom viewport
      if (y < topBottomView)
         cout << "top x: " << x/(float)width << "   y: " << y/(float)height << endl;
      else
         cout << "bottom: " << x/(float)width << "   y: " << y/(float)height << endl;
   }
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
      case 'x': Xangle += 5.0;
                if (Xangle > 360.0) Xangle -= 360.0;
                glutPostRedisplay();
                break;
      case 'X': Xangle -= 5.0;
                if (Xangle < 0.0) Xangle += 360.0;
                glutPostRedisplay();
                break;
      case 'y': Yangle += 5.0;
                if (Yangle > 360.0) Yangle -= 360.0;
                glutPostRedisplay();
                break;
      case 'Y': Yangle -= 5.0;
                if (Yangle < 0.0) Yangle += 360.0;
                glutPostRedisplay();
                break;
      case 'z': Zangle += 5.0;
                if (Zangle > 360.0) Zangle -= 360.0;
                glutPostRedisplay();
                break;
      case 'Z': Zangle -= 5.0;
                if (Zangle < 0.0) Zangle += 360.0;
                glutPostRedisplay();
                break;
      case 'q': exit (1);
   }
}

// Main routine.
int main(int argc, char **argv) 
{
   char *filename = argv[1];
   fractal.readIn(filename);
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize(750, 750);
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

