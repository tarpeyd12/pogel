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

#define itterations 10
#define startsize	1.0f
#define size    	1.3f
POGEL::OBJECT* obj;

//IMAGE *earth;
IMAGE *defaultimg;

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]= { 10.0f, 10.0f, 10.0f, 1.0f };


float operation(float x, unsigned long i, unsigned long itermax) {
	if(i >= itermax)
		return 0.0f;
	float g = float(pow(2.0, (float)i));
	float ret;
	//if(i%2 == 0)
		ret = (float)cos(x*g)/g;
	//else
		//ret = (float)sin(x*g)/g;
	
	return ret + operation(x, i+1, itermax);
};

SHAPE_FUNCTION_RESULT func(SHAPE_FUNCTION_ARGS) {
	// b is the number of itterations to complete
	// c is the cuttent itteration
	
	// x is theta
	
	//POGEL::message("\nhello\n");
	
	POGEL::POINT ret;
	
	ret.x = x;
	ret.y = operation(x,0,c+1);
	ret.z = z;
	
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
	
	obj = new OBJECT();
	obj->setname("graph\0");
	
	addFunctionShape(obj, func, "xz", defaultimg, TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, 3.14*2, 3.14*2, 1, 1000, 1, 10);
	
	obj->build();
	
	POGEL::InitFps();
	printf("\n");
}

//unsigned long frames=0;
float x = POGEL::FloatRand(2.0)-1.0, y = POGEL::FloatRand(2.0)-1.0, z = POGEL::FloatRand(2.0)-1.0;

bool keypres, go = true;
POGEL::POINT camrot(0,0,0), campos(0,0,-7);

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
	
	/*glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);*/
	
	obj->draw();
	
	// since this is double buffered, swap the buffers to display what just got drawn.
	glutSwapBuffers();
}

