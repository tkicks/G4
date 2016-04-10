/*

Names: Austin Gillis Tyler Kickham
Program Name: Assignment G4- Flowerly Fractal
Purpose: The purpose of this program is to utilize openGL to 
Input:
Output: An openGL window displaying

*/


// #include <stdio.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include <pthread.h>
#include <math.h>
GLenum rgb;					// for tkmap.c
#include "tkmap.c"


using namespace std;


#define PI 3.14159265

/************************Globals******************************/

static GLsizei width, height; // OpenGL window size.

float z = 3.75;

static float Xangle = 0.0, Yangle = 0.0, Zangle = 0.0;
static GLUquadricObj *qobj;

int strPos;

const string sequence="bZb[Zbb]zbb";
/*************************************************************/


void* simpleFunc(void*) { return NULL; }
void forcePThreadLink() { pthread_t t1; pthread_create(&t1, NULL, &simpleFunc, NULL); }
int decision(char letter);

class Tree{

public:
	 
	 Tree () {objHeight = 0.2; objRadius = 0.05; angle = 45*PI/180; n = 0; startY=-0.79; currP.push_back(0.0);currP.push_back(0);currP.push_back(0.0);};  // constructor

	 void drawButtons(float x1, float y1, float buttonWidth, float buttonHeight);
	 void drawLeaf(); 
	 void drawBranch();

	 void zRotation();

	 void drawVertical();
	 void drawAngled(int direction);

	 void pushKnot();
	 void popKnot();

	 int makeTree(int operation);

	 vector<string> grammars;
	 vector<char> plant;
	 vector<float>currP;
	 float objHeight;    // height of cylinders (branches)
	 float objRadius;    // radius of cylinders (branches)
	 float angle;        // angle to rotate branches by
	 int n;					// number of iterations
	 int grammarNum;		// which grammar

	 float startY;   //Coords for the top of the first cylinder drawn (0,startY,0) so only need var for startY 

	 stack<vector<float> > knots; //Coords for the places to pop back to

};

Tree fractal;

/************************Class Methods***************************/

void Tree::drawLeaf(){
		//Remeber what Gousie said about overlapping leaves...have a pallete to choose from
		glColor3f (0.0, 1.0, 0.0);
		
		glBegin(GL_POLYGON);
			glVertex3f(-.05,0,0);
			glVertex3f(0,0.1,0);
			glVertex3f(0.05,0,0);
			glVertex3f(0,-0.1,0);
		glEnd();
}

void Tree::drawBranch()
{
	// cout << "drawBranch()\n";

	glPushMatrix();
		glColor3f(.545, .271, .075);
		glRotatef(90.0,1.0,0.0,0.0);
		gluCylinder(qobj, objRadius, objRadius, objHeight, 15.0, 5.0);
	glPopMatrix();
	//glFlush();
}

void Tree::zRotation(){

	float xCoord,yCoord,xP,yP,fX,fY;

	xCoord=currP[0];
	yCoord=startY+currP[1]+objHeight;

	fX= currP[0];
	fY= currP[1]+startY;

	xP= xCoord*cos(angle) - yCoord*sin(angle) +(-1*fX*cos(angle)+fY*sin(angle));
	yP= xCoord*sin(angle) + yCoord*cos(angle) +(-1*fX*sin(angle)-fY*cos(angle));


	currP[0]+=xP;
	currP[1]+=yP;

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
	zRotation();
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

				if(decision(sequence[strPos-1])==4){
				
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
		cout<<strPos-1<<endl;
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

void Tree::drawButtons(float x1, float y1, float buttonWidth, float buttonHeight){
	glColor3f(0.2, 0.2, 0.2);
	glBegin(GL_POLYGON);
		glVertex3f(x1, y1, 0);
		glVertex3f((x1+(width/750*buttonWidth)), y1, 0);
		glVertex3f((x1+(width/750*buttonWidth)), (y1-(width/750*buttonHeight)), 0);
		glVertex3f(x1, (y1-(width/750*buttonHeight)), 0);
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
	fractal.drawButtons((width/750*.4), -.1, .4, .4);

	// label buttons
	//fractal.createLabels();

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

	// Allow plant to be rotated.
	glRotatef(Zangle, 0.0, 0.0, 1.0);
	glRotatef(Yangle, 0.0, 1.0, 0.0);
	glRotatef(Xangle, 1.0, 0.0, 0.0);

	glColor3f (0.0, 0.0, 1.0);


	glBegin(GL_POINTS);

		glVertex3f(-1,-1,0);
		glVertex3f(1,1,0);
		glVertex3f(-1,1,0);
		glVertex3f(1,-1,0);
		glVertex3f(0,0,0);
		//glVertex3f(0, -0.59 + 0.141421,0);
	glEnd();

	//"Marker" always starts at (0,-.99,0) drawing up
	
	//Caluclate x'  y' z'  without adding Fx Fx or Fz.  Add x' y' z' to current posititon
	//Values for first translate in a rotation are current posiotn x,y,z
	//Values for translate when straight up position (+.2 in the direction the top is pointing)
	
	//gotta work on popping.  Right now it pops and draws 1 click too soon   
	
	//string sequence="bZb[Zbb]zbb";

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
		
		if(y<=480){
			
			glutPostRedisplay ();
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
	else if(letter=='Z'){
		return 1;		
	}
	else if(letter=='z'){
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