#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>		// Header File For The OpenGL32 Library
#include <GL/glu.h>		// Header File For The GLu32 Library
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "scene.h"
#include "window.h"

#include "pogel/pogel.h"
#include "pogel/classes/physics/physics.h"

using namespace POGEL;

#define itterations 1
POGEL::OBJECT* obj;

int itt=1;

//IMAGE *earth;
IMAGE *defaultimg;

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]= { 10.0f, 10.0f, 10.0f, 1.0f };


float square(const float x)
{
// Period
static const float period = PI;
static const float amplitude = 1.0;

// Force positive argument.
const float xx = x < 0.0 ? -x : x;

// Scale the argument.
const float nT = xx / period;
const float x_scaled = xx - floor(nT) * period;
const float period_half = period / 2.0;

// Return square wave.
return x_scaled < period_half ? amplitude : -amplitude;
}

float triangle(float x) {
	// Period
	static const float period = PI;
	static const float amplitude = 1.0;
	
	// Force positive argument.
	const float xx = x < 0.0 ? -x : x;
	
	// Scale the argument.
	//const float nT = xx / period;
	const float x_scaled = fmod(xx,period);// - floor(nT) * period;
	const float period_half = period / 2.0;
	
	// Return triangle wave.
	return (x_scaled <= period_half ? (-amplitude)*(x_scaled) + (amplitude)*(period_half-x_scaled) : (amplitude)*(x_scaled) + (-amplitude)*(period-x_scaled) - period/2)/2;
};

float sqrop(float x, float i, float itermax) {
	if(i >= itermax)
		return 0.0f;
	float g = float(pow(2.0, (float)i));
	float ret = (float)square(x*g)/g;
	return ret + sqrop(x, i+1, itermax);
};

float triop(float x, float i, float itermax) {
	if(i >= itermax)
		return 0.0f;
	float g = float(pow(2.0, (float)i));
	float ret = (float)triangle(x*g)/g;
	return ret + triop(x, i+1, itermax);
};

float cosop(float x, float i, float itermax) {
	if(i >= itermax)
		return 0.0f;
	float g = float(pow(2.0, (float)i));
	float ret = (float)cos(x*g)/g;
	return ret + cosop(x, i+1, itermax);
};

float sinop(float x, float i, float itermax) {
	if(i >= itermax)
		return 0.0f;
	float g = float(pow(2.0, (float)i));
	float ret = (float)sin(x*g)/g;
	return ret + sinop(x, i+1, itermax);
};

float tanop(float x, float i, float itermax) {
	if(i >= itermax)
		return 0.0f;
	float g = float(pow(2.0, (float)i));
	float ret = (float)tan(x*g)/g;
	return ret + sinop(x, i+1, itermax);
};

SHAPE_FUNCTION_RESULT func(SHAPE_FUNCTION_ARGS) {
	// b is the number of itterations to complete
	// c is the cuttent itteration
	
	// x is theta
	
	//POGEL::message("\nhello\n");
	
	POGEL::POINT ret;
	
	/*ret.x = sinop(x,0,c+1);
	ret.y = cosop(x,0,c+1);
	ret.z = tanop(x,0,c+1);*/
	
	/*ret.x = sinop(x,0,c+1);
	ret.y = cosop(x,0,c+1);
	ret.z = z;*/
	
	/*ret.x = x;
	ret.y = cosop(x,0,itt) + cosop(z,0,itt);
	ret.z = z;*/
	
	ret.x = x;
	ret.y = cosop(x,0,c+1);
	ret.z = z;
	
	/*ret.x = x;
	ret.y = (sqrop(x,0,itt) + sqrop(z,0,itt))/1;
	ret.z = z;*/
	
	/*ret.x = x;
	ret.y = sqrop(x,0,c+1);
	ret.z = z;*/
	
	/*ret.x = x;
	ret.y = triop(x,0,itt) + triop(z,0,itt);
	ret.z = z;*/
	
	/*ret.x = x;
	ret.y = triop(x,0,c+1);
	ret.z = z;*/
	
	/*ret.y = x*z/2;
	ret.z=z;
	ret.x=x;*/
	
	return ret;
};

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
	glEnable(GL_TEXTURE_2D);				// Enable Texture Mapping
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// Clear The Background Color To Black 
	glClearDepth(500.0);					// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);					// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);				// Enables Depth Testing
	glShadeModel(GL_SMOOTH);				// Enables Smooth Color Shading
	//glShadeModel(GL_FLAT);				// Enables flat Color Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH,GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				// Reset The Projection Matrix
	
	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window
    
	glMatrixMode(GL_MODELVIEW);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	
	srand((unsigned)time(NULL));
	
	//earth=new IMAGE("Data/earth.bmp");
	defaultimg=new IMAGE("Data/default_2.bmp");
	
	//getchar();
	
	
	obj = new OBJECT[itterations];
	
	for(int i = 0; i<itterations; i++) {
		obj[i].setname(POGEL::string("graph%d\0",i));
		itt = i + 1;
		addFunctionShape(&obj[i], func, "xz", defaultimg, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, PI*2, PI*2, PI*2, 1024+0*pow(2,itt)*2, 0, 10+0*pow(2,itt)*2);
		obj[i].build();
	}
	
	POGEL::InitFps();
	printf("\n");
}

//unsigned long frames=0;
float x = 1, y = 1, z =1;

bool keypres, go = true;
POGEL::POINT camrot(45,0,0), campos(0,0,-10);

/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	
	glTranslatef(campos.x,campos.y,campos.z);
	//glRotatef( 90.0f,  1.0f, 0.0f, 0.0f );
	glRotatef( camrot.x + ((float)frames*x)*0.0f,  1.0f, 0.0f, 0.0f );
	glRotatef( camrot.y + ((float)frames*y)*0.0f,  0.0f, 1.0f, 0.0f );
	glRotatef( camrot.z + ((float)frames*z)*0.0f,  0.0f, 0.0f, 1.0f );
	//glRotatef( 90.0f,  0.0f, 1.0f, 0.0f );
	
	POGEL::IncrementFps();
	POGEL::PrintFps();
	
	/*campos.print();
	camrot.print();*/
	
	/*glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);*/
	
	if(POGEL::frames%12 == 0)
	//if(keypres)
	{
		//keypres = false;
		itt++;
	}
	if(itt >= itterations || keypres) {
		keypres = false;
		itt = 0;
	}
	
	obj[itt].draw();
	
	// since this is double buffered, swap the buffers to display what just got drawn.
	glutSwapBuffers();
}

