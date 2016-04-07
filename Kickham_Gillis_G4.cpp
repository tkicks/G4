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

float z = 3.75;

static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;
static GLUquadricObj *qobj;

/*************************************************************/


void* simpleFunc(void*) { return NULL; }
void forcePThreadLink() { pthread_t t1; pthread_create(&t1, NULL, &simpleFunc, NULL); }

class Tree{

public:
	 
	 Tree () {objHeight = 0.2; objRadius = 0.05; angle = 0; n = 0;};  // constructor

	 void readIn(char* inFilename);
	 void drawButtons(float x1, float y1);

	 void drawLeaf(float centerX,float centerY,float centerZ);
	 void drawTree();
	 void drawBranch();
	 void growTree();
	 void rotateObj(float omega, float x, float y, float z);

	 vector<string> grammars;
	 vector<char> plant;
	 float objHeight;    // height of cylinders (branches)
	 float objRadius;    // radius of cylinders (branches)
	 float angle;        // angle to rotate branches by
	 int n;					// number of iterations
	 int grammarNum;		// which grammar
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

void Tree::drawLeaf(float centerX,float centerY,float centerZ){

		cout << "drawLeaf()\n";

		//Remeber what Gousie said about overlapping leaves...have a pallete to choose from
		glColor3f (0.0, 1.0, 0.0);
		
		glBegin(GL_POLYGON);
			glVertex3f(centerX-.05,centerY,centerZ);
			glVertex3f(centerX,centerY+.1,centerZ);
			glVertex3f(centerX+.05,centerY,centerZ);
			glVertex3f(centerX,centerY-.1,centerZ);
		glEnd();
}

void Tree::drawTree()
{



	for (int i = 0; i < plant.size(); i++)
		cout << plant[i];
	cout << endl;
	float centerX, centerY, centerZ;

	for (int i = 0; i < plant.size(); i++)
	{
		if (plant[i] == 'B')
			drawBranch();
		else if (plant[i] == 'L')
			drawLeaf(centerX, centerY, centerZ);
		else if (plant[i] == 'X')
			rotateObj(angle, 1.0, 0.0, 0.0);
		else if (plant[i] == 'x')
			rotateObj(-angle, 1.0, 0.0, 0.0);
		else if (plant[i] == 'Y')
			rotateObj(angle, 0.0, 1.0, 0.0);
		else if (plant[i] == 'y')
			rotateObj(-angle, 0.0, 1.0, 0.0);
		else if (plant[i] == 'Z')
			rotateObj(angle, 0.0, 0.0, 1.0);
		else if (plant[i] == 'z')
			rotateObj(-angle, 0.0, 0.0, 1.0);
	}
}

void Tree::growTree()
{
	if (n < 5)
	{
		if (plant[plant.size()-1] == 'B')
		{
			plant.pop_back();
			// start at 2 for now b/c grammar starts with "B "
			for (int i = 2; i < grammars[grammarNum].size(); i++)
				plant.push_back(grammars[grammarNum][i]);
		}
		n += 1;
	}
}

void Tree::rotateObj(float omega, float x, float y, float z)
{
	cout << "rotateObj(" << omega << ", " << x << ", " << y << ", " << z << ")\n";
}

void Tree::drawBranch()
{
	cout << "drawBranch()\n";

	glPushMatrix();
		glColor3f(.545, .271, .075);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(qobj, objRadius, objRadius, objHeight, 15.0, 5.0);
	glPopMatrix();
	glFlush();
}

void Tree::drawButtons(float x1, float y1)
// draw menu buttons
{
	glColor3f(0.2, 0.2, 0.2);
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

	fractal.drawTree();

	// fractal.drawBranch();


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
	qobj = gluNewQuadric();
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
		int viewport[4];
		float topBottomView= (height/2)+.3333*(height/2);
		float winZ;
		double posX, posY, posZ;
		GLdouble modelview[16];
		GLdouble projection[16];

		// grammar 1
		if ((x < (width*.3) & x > (width*.1)) & (y < (height*.77) & y > (height*.7)))
		{
			// cout << "Grammar 1\n";
			fractal.grammarNum = 0;
			fractal.angle = 45;
			if (fractal.n == 0)
				fractal.plant.push_back('B');
			fractal.growTree();
			fractal.drawTree();
		}
		// grammar 2
		else if ((x < (width*.6) & x > (width*.4)) & (y < (height*.77) & y > (height*.7)))
		{
			// cout << "pressed button 2\n";
			fractal.grammarNum = 1;
			if (fractal.n == 0)
				fractal.plant.push_back('B');
			fractal.drawTree();
		}
		// grammar 3
		else if ((x < (width*.9) & x > (width*.7)) & (y < (height*.77) & y > (height*.7)))
		{
			// cout << "pressed button 3\n";
			fractal.grammarNum = 2;
			if (fractal.n == 0)
				fractal.plant.push_back('B');
			fractal.drawTree();
		}

		// gluUnProject((float)x, (float) y, (float) winZ, modelview, projection, viewport, &posX, &posY, &posZ);

		// say where x and y clicked are and if top or bottom viewport
		// if (y < topBottomView)
		// 	cout << "top x: " << x << "   y: " << y << endl;
		// else
		// 	cout << "bottom: " << x << "   y: " << y << endl;
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

