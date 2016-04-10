/*

Names: Austin Gillis Tyler Kickham
Program Name: Assignment G4- Flowerly Fractal
Purpose: The purpose of this program is to utilize openGL to 
Input:
Output: An openGL window displaying

*/

#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <math.h>
#include <pthread.h>
GLenum rgb;					// for tkmap.c
#include "tkmap.c"

using namespace std;

#define PI 3.14159265

/************************Globals******************************/

static GLsizei width, height; // OpenGL window size.

float z = 3.75;

int strPos;

static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;
static GLUquadricObj *qobj;

const int maxZoom = 2;
const int minZoom = -16;

const string sequence="b-b[-b+b+b]+b-b-b";
/*************************************************************/


void* simpleFunc(void*) { return NULL; }
void forcePThreadLink() { pthread_t t1; pthread_create(&t1, NULL, &simpleFunc, NULL); }
int decision(char letter);

class Tree{

public:
	 
	 Tree () {objHeight = 0.2; objRadius = 0.05; angle = 45*PI/180; n = 0; startY=-0.79; currP.push_back(0.0);currP.push_back(0);currP.push_back(0.0);};  // constructor

	 void readIn(char* inFilename);
	 void drawButtons(float x1, float y1, float buttonWidth, float buttonHeight);

	 void drawLeaf();
	 void drawBranch();

	 void zRotation(int direction);
	 
	 void drawVertical();
	 void drawAngled(int direction);

	 void pushKnot();
	 void popKnot();

	 int makeTree(int operation);

	 void growTree();
	
	 void createLabels();
	 void writeLabels(float x, float y, const char label[]);
	 float leafColor();			// randomly determine leaf color

	 vector<string> grammars;
	 vector<char> plant;
	 vector<float>currP;
	 float objHeight;    // height of cylinders (branches)
	 float objRadius;    // radius of cylinders (branches)
	 float angle;        // angle to rotate branches by
	 int n;					// number of iterations
	 int grammarNum;		// which grammar
	 int zoom;

	 float startY;   //Coords for the top of the first cylinder drawn (0,startY,0) so only need var for startY 

	 stack<vector<float> > knots; //Coords for the places to pop back to
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

void Tree::drawLeaf(){
		
		float colorLeaf = leafColor();
		//Remeber what Gousie said about overlapping leaves...have a pallete to choose from
		if (colorLeaf == 0){
			glColor3f (0.0, 1.0, 0.0);
		}
		else if (colorLeaf == 1){
			glColor3f(1.0, 1.0, 0.0);
		}
		else if (colorLeaf == 2){
			glColor3f(1.0, 0.6, 0.2);
		}

		glBegin(GL_POLYGON);
		glVertex3f(-.05,0,0);
		glVertex3f(0,0.1,0);
		glVertex3f(0.05,0,0);
		glVertex3f(0,-0.1,0);
		glEnd();
}


void Tree::zRotation(int direction){

	float xCoord,yCoord,xP,yP,fX,fY;

	xCoord=currP[0];
	yCoord=startY+currP[1]+objHeight;

	fX= currP[0];
	fY= currP[1]+startY;

	xP= xCoord*cos(direction*angle) - yCoord*sin(direction*angle) +(-1*fX*cos(direction*angle)+fY*sin(direction*angle));
	yP= xCoord*sin(direction*angle) + yCoord*cos(direction*angle) +(-1*fX*sin(direction*angle)-fY*cos(direction*angle));


	currP[0]+=xP;
	currP[1]+=yP;

}

void Tree::drawBranch()
{
	
	glPushMatrix();
		glColor3f(.545, .271, .075);
		glRotatef(90.0, 1.0, 0.0, 0.0);
		gluCylinder(qobj, objRadius, objRadius, objHeight, 15.0, 5.0);
	glPopMatrix();
	glFlush();
}

void Tree::drawVertical(){
	
	glPushMatrix();
	glTranslatef(0.0,objHeight,0.0);
	glTranslatef(currP[0],currP[1],currP[2]);
	drawBranch();
	//If drawing a leaf next call drawLeaf here
	glPopMatrix();

	currP[1]+=objHeight;
}

void Tree::drawAngled(int direction){
	
	glPushMatrix();
	glTranslatef(currP[0],currP[1],currP[2]);
	glRotatef(direction*45.0,0,0,1);
	glTranslatef(0.0,objHeight,0.0);
	drawBranch();
	zRotation(direction);
	//If drawing a leaf next call drawLeaf here
	glPopMatrix();
}

void Tree::pushKnot(){

	float knot[] = {currP[0],currP[1],currP[2]};

	vector<float> temp (knot, knot + sizeof(knot) / sizeof(float) );

	knots.push(temp);
}

void Tree::popKnot(){
	
	if(!knots.empty()){
		vector<float> temp= knots.top(); 
		knots.pop(); 

		currP[0]=temp[0];
		currP[1]=temp[1];
		currP[2]=temp[2];
	}

}

int Tree::makeTree(int operation){

	int strInc;
	if(operation==0){
		// //Assumes first char in str is always a vert branch
		//cout<<strPos<<endl;
		if(strPos==0){
			drawBranch();
			currP[0]=currP[1]=currP[2]=0;	
					
		}
		else{
			//4 is ] which means pop knot
			//cout<<decision(strPos-1)<<endl;
			if(decision(sequence[strPos-1])==4){
				//the char before the vert branch was a pop
				popKnot();
				drawVertical();

			}	
			else{
				drawVertical();
			}
		
		}
		strInc=1;	
	}	
	else if(operation==1){		
		//4 is ] which means pop knot
		
		if(decision(sequence[strPos-1])==4){
			//For some reason unnkwon reason this is exeuting
			popKnot();
			drawAngled(1);
		}
		else{
			drawAngled(1);
		}
		strInc=2;
	}
	else if(operation==2){
		//4 is ] which means pop knot
		if(decision(sequence[strPos-1])==4){

			//the char before the vert branch was a pop
			popKnot();
			drawAngled(-1);
		}
		else{
			drawAngled(-1);
		}
		strInc=2;	
	}
	else if(operation==3){
		pushKnot();
		strInc=1;
	}
	else if(operation==4){
		strInc=1;
	}

	return strInc;

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

void Tree::drawButtons(float x1, float y1, float buttonWidth, float buttonHeight)
// draw menu buttons
{
	glColor3f(0.2, 0.2, 0.2);
	glBegin(GL_POLYGON);
		glVertex3f(x1, y1, 0);
		glVertex3f((x1+(width/750*buttonWidth)), y1, 0);
		glVertex3f((x1+(width/750*buttonWidth)), (y1-(width/750*buttonHeight)), 0);
		glVertex3f(x1, (y1-(width/750*buttonHeight)), 0);
	glEnd();
}

void Tree::createLabels()
{
	glColor3f(1.0, 1.0, 1.0);
	writeLabels(-0.72, 0.6, "Grammar 1");
	writeLabels(-0.12, 0.6, "Grammar 2");
	writeLabels(0.48, 0.6, "Grammar 3");
	writeLabels(-0.72, 0.0, "+");
	writeLabels(-0.72, -0.6, "-");
	writeLabels(-0.45, 0.0, "+X");
	writeLabels(-0.45, -0.6, "-X");
	writeLabels(-0.14, 0.0, "+Y");
	writeLabels(-0.14, -0.6, "-Y");
	writeLabels(0.16, 0.0, "+Z");
	writeLabels(0.16, -0.6, "-Z");
	writeLabels(0.53, 0.0, "Grow");
	writeLabels(0.44, -0.6, "Clear");
	writeLabels(0.7, -0.6, "Quit");
}

void Tree::writeLabels(float x, float y, const char label[])
{
	int i = 0;						// iterator
	glRasterPos2f((x*(width/750)), y*(height/750));	// position w/ x,y% coordinates
	// while more chars, write to map
	while (label[i] != '\0')
	{
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, label[i]);
		i++;
	}

	glFlush();	// draw
}

float Tree::leafColor()
{
	return (rand()%3);
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

	// drawButtons(x, y, width, height)
	// grammar buttons
	fractal.drawButtons((-width/750*.8), .8, .4, .4);
	fractal.drawButtons((-width/750*.2), .8, .4, .4);
	fractal.drawButtons((width/750*.4), .8, .4, .4);
	// zoom buttons
	fractal.drawButtons((-width/750*.8), .2, .2, .4);
	fractal.drawButtons((-width/750*.8), -.4, .2, .4);
	// x rotation buttons
	fractal.drawButtons((-width/750*.5), .2, .2, .4);
	fractal.drawButtons((-width/750*.5), -.4, .2, .4);
	// y rotation buttons
	fractal.drawButtons((-width/750*.2), .2, .2, .4);
	fractal.drawButtons((-width/750*.2), -.4, .2, .4);
	// z rotation buttons
	fractal.drawButtons((width/750*.1), .2, .2, .4);
	fractal.drawButtons((width/750*.1), -.4, .2, .4);
	// grow button
	fractal.drawButtons((width/750*.4), .2, .4, .4);
	// clear button
	fractal.drawButtons((width/750*.4), -.4, .2, .4);
	// clear button
	fractal.drawButtons((width/750*.65), -.4, .2, .4);

	// label buttons
	fractal.createLabels();

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

	glTranslatef(0,-.99+fractal.objHeight,0);
	strPos=0;
	int a;
	//cout<<"Hello?!??!!?"<<endl;
	while(strPos<sequence.size()){
		a=decision(sequence[strPos]);
		strPos+=fractal.makeTree(a);
	}

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
		float topBottomView= (height/2)+.3333*(height/2);

		// grammar 1
		if (x > (width*.1) & (x < (width*.3)) & (y > (height*.7) & y < (height*.77)))
		{
			fractal.grammarNum = 0;
			fractal.angle = 45;
		}
		// grammar 2
		else if (x > (width*.4) & (x < (width*.6)) & (y > (height*.7) & y < (height*.77)))
		{
			fractal.grammarNum = 1;
		}
		// grammar 3
		else if (x > (width*.7) & (x < (width*.9)) & (y > (height*.7) & y < (height*.77)))
		{
			fractal.grammarNum = 2;
		}
		// zoom in
		else if (x > (width*.1) & (x < (width*.2)) & (y > (height*.8) & y < (height*.87)))
		{
			if (fractal.zoom < maxZoom)
			{
				z -= 1;
				fractal.zoom += 1;
				glutPostRedisplay ();
			}
		}
		// zoom out
		else if (x > (width*.1) & (x < (width*.2)) & (y > (height*.9) & y < (height*.97)))
		{
			if (fractal.zoom > minZoom)
			{
				z += 1;
				fractal.zoom -= 1;
				glutPostRedisplay ();
			}
		}
		// rotate positive x
		else if (x > (width*.25) & (x < (width*.35)) & (y > (height*.8) & y < (height*.87)))
		{
			Xangle += 5.0;
			if (Xangle > 360.0) Xangle -= 360.0;
			glutPostRedisplay();
		}
		// rotate negative x
		else if (x > (width*.25) & (x < (width*.35)) & (y > (height*.9) & y < (height*.97)))
		{
			Xangle -= 5.0;
			if (Xangle < 0.0) Xangle += 360.0;
			glutPostRedisplay();
		}
		// rotate positive y
		else if (x > (width*.4) & (x < (width*.5)) & (y > (height*.8) & y < (height*.87)))
		{
			Yangle += 5.0;
			if (Yangle > 360.0) Yangle -= 360.0;
			glutPostRedisplay();
		}
		// rotate negative y
		else if (x > (width*.4) & (x < (width*.5)) & (y > (height*.9) & y < (height*.97)))
		{
			Yangle -= 5.0;
			if (Yangle < 0.0) Yangle += 360.0;
			glutPostRedisplay();
		}
		// rotate positive z
		else if (x > (width*.55) & (x < (width*.65)) & (y > (height*.8) & y < (height*.87)))
		{
			Zangle += 5.0;
			if (Zangle > 360.0) Zangle -= 360.0;
			glutPostRedisplay();
		}
		// rotate negative z
		else if (x > (width*.55) & (x < (width*.65)) & (y > (height*.9) & y < (height*.97)))
		{
			Zangle -= 5.0;
			if (Zangle < 0.0) Zangle += 360.0;
			glutPostRedisplay();
		}
		// grow tree
		else if (x > (width*.7) & (x < (width*.9)) & (y > (height*.8) & y < (height*.87)))
		{
			if (fractal.n == 0)
				fractal.plant.push_back('B');
			// fractal.growTree();
			// fractal.drawTree();
		}
		// clear
		else if (x > (width*.7) & (x < (width*.8)) & (y > (height*.9) & y < (height*.97)))
		{
			cout << fractal.plant.size() << endl;
			cout << "Clearing plant\n";
			fractal.plant.clear();
			cout << fractal.plant.size() << endl;
			fractal.n = 0;
			z = 3.75;
			fractal.zoom = 0;
			glutPostRedisplay();
		}
		// quit
		else if (x > (width*.83) & (x < (width*.925)) & (y > (height*.9) & y < (height*.97)))
		{
			cout << "Quiting Flowerly Fractal\n";
			exit (1);
		}
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
	srand(time(NULL));
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

int decision(char letter){
	
	if(letter=='b'){
		return 0;	
	}
	else if(letter=='+'){
		return 1;		
	}
	else if(letter=='-'){
		return 2;
	}
	else if(letter=='['){
		return 3;
	}
	else if(letter==']'){
		// cout<<"FUUUUUUUU!!!!"<<endl;
		return 4;
	}
}