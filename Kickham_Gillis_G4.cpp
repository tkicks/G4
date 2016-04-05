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

float z = 1.75;

static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;

/*************************************************************/


void* simpleFunc(void*) { return NULL; }
void forcePThreadLink() { pthread_t t1; pthread_create(&t1, NULL, &simpleFunc, NULL); }

class Tree{

public:
    
    Tree () {};  // constructor

    void readIn(char* inFilename);
    void drawButtons(float x1, float y1);

    void drawLeaf(float centerX,float centerY,float centerZ);

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
      // exit(1);
   }
}

void drawLeaf(float centerX,float centerY,float centerZ){

      //Remeber what Gousie said about overlapping leaves...have a pallete to choose from
      glColor3f (0.0, 1.0, 0.0);
      
      glBegin(GL_POLYGON);
         glVertex3f(centerX-.05,centerY,centerZ);
         glVertex3f(centerX,centerY+.1,centerZ);
         glVertex3f(centerX+.05,centerY,centerZ);
         glVertex3f(centerX,centerY-.1,centerZ);
      glEnd();
}

void Tree::drawButtons(float x1, float y1)
// draw menu buttons
{
   glColor3f(0.2, 0.2, 0.2);
   cout << x1 << " " << y1 << endl;
   glBegin(GL_POLYGON);
      glVertex3f(x1, y1, 0);
      glVertex3f((x1+(width/750*.4)), y1, 0);
      glVertex3f((x1+(width/750*.4)), (y1-(width/750*.4)), 0);
      glVertex3f(x1, (y1-(width/750*.4)), 0);
   glEnd();
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
      
      for (float i = -1; i <= 1; i+=.001){
         glVertex3f(i,.9,0);
      }
      
   glEnd();

   fractal.drawButtons((-width/750*.8), .8);
   fractal.drawButtons((-width/750*.2), .8);
   fractal.drawButtons((width/750*.4), .8);

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


   glBegin(GL_POINTS);

      glVertex3f(-1,-1,0);
      glVertex3f(1,1,0);
      glVertex3f(-1,1,0);
      glVertex3f(1,-1,0);

   glEnd();


   // glutWireTeapot (0.7);

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

      // grammar 1
      if ((x < (width*.3) & x > (width*.1)) & (y < (height*.77) & y > (height*.7)))
         cout << "pressed button 1\n";


      // say where x and y clicked are and if top or bottom viewport
      if (y < topBottomView)
         cout << "top x: " << x << "   y: " << y << endl;
      else
         cout << "bottom: " << x << "   y: " << y << endl;
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
   glutInitWindowSize(750, 1125);
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

