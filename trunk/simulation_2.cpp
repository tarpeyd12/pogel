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

#define numobjs 2
#define grd 1
#define sps 10.01f
#define size 2.0f
OBJECT obj[numobjs];
POGEL::PHYSICS::SOLID **sphs;
POGEL::PHYSICS::SOLID *border;

POGEL::PHYSICS::SIMULATION sim;

IMAGE *earth;
IMAGE *defaultimg;

float x = POGEL::FloatRand(10.0f)-5.0f, y = POGEL::FloatRand(10.0f)-5.0f, z = POGEL::FloatRand(10.0f)-5.0f;

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 2.0f, 2.0f, 2.0f, 1.0f };
GLfloat LightPosition[]= { 5.0f, 0.0f, 5.0f, 1.0f };

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
	
	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,500.0f);	// Calculate The Aspect Ratio Of The Window
    
	glMatrixMode(GL_MODELVIEW);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	
	//sim = new POGEL::PHYSICS::SIMULATION();
	sim.deactivation = false;
	sim.precision = 0.01f;
	
	srand((unsigned)time(NULL));
	
	sphs=(POGEL::PHYSICS::SOLID**)malloc(sizeof(POGEL::PHYSICS::SOLID*)*numobjs);
	
	//earth=new IMAGE("Data/earth.bmp");
	defaultimg=new IMAGE("Data/default_2.bmp");
	
	int i = 0;
	obj[i].setname("cube");
	addCube(&obj[i], size,size,size, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX());
	obj[i].moveto(POINT(0.0f,0.0f,0.0f));
	sphs[i] = new POGEL::PHYSICS::SOLID(&obj[i], POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(0.0f, 10.0f, 1.0f, 1.0f, 0.0f, 0.0f, false, (i%2==0?-1.0f:1.0f)), 4|1);
	sphs[i]->build();
	sphs[i]->spin=POGEL::VECTOR(0.0f,0.0f,0.0f);
	sphs[i]->resizetrail(250);
	sim.addSolid(sphs[i]);
	
	i = 1;
	obj[i].setname("bar");
	//addCylinder(&obj[i], 12, 1, size*8, size/2.0f, size/2.0f, defaultimg, 1.0f, 1.0f, 0, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,90.0f,0.0f))*MATRIX(POINT(),POINT(45.0f,0.0f,0.0f)));
	addCube(&obj[i], size,size*8,size, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX());
	obj[i].moveto(POINT(5.0f,3.0f,0.0f));
	sphs[i] = new POGEL::PHYSICS::SOLID(&obj[i], POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, false, (i%2==0?-1.0f:1.0f)), 4|2);
	sphs[i]->build();
	//sphs[i]->spin=POGEL::VECTOR(1.0f,0.0f,0.0f);
	sphs[i]->resizetrail(250);
	sim.addSolid(sphs[i]);
	
	//sim.addsingularity( POGEL::PHYSICS::SINGULARITY(POGEL::POINT(0.0f,0.0f,0.0f),100000000000.0f) );
	//sim.addfan(PHYSICS::FAN(POINT(0.0f,0.0f,0.0f), VECTOR(0.0f,1.0f,0.0f), 150.0f));
	sim.gravity = POGEL::VECTOR(0.0f,-1.0f,0.0f)*1.0f;
	//sim.air_dencity = 1.0f;
	
	POGEL::OBJECT *ring = new POGEL::OBJECT();
	ring->setname("border");
	
	addDisk(ring, 16, 1, 20.0f, 0.0f, defaultimg,1, 1, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, true, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
	
	ring->setproperties(2);
	ring->moveto(POGEL::POINT(0.0f,-10.0f,0.0f));
	ring->turnto(POGEL::POINT(0.0f,0.0f,0.0f));
	//ring->build();
	border = new POGEL::PHYSICS::SOLID(ring, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(), 1);
	//border->spin = POGEL::VECTOR(POGEL::FloatRand(1.0f),POGEL::FloatRand(1.0f),POGEL::FloatRand(1.0f))*1.0f;
	//border->spin = POGEL::VECTOR(0.0f,1.0f,0.0f);
	border->behavior.bounce = 1.0f;
	border->behavior.friction = 1.0f;
	
	border->behavior.magnetic = true;
	border->behavior.charge = -0.01f;
	border->build();
	//border->visable = false;
	sim.addSolid(border);
	
	
	POGEL::InitFps();
	printf("\n");
}
bool keypres, go = true;
POGEL::POINT camrot, campos;
/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	
	glTranslatef(0.0f,0.0f,-25.0f*2.0f);
	glRotatef( camrot.x + ((float)frames*x)*0.0f,  1.0f, 0.0f, 0.0f );
	glRotatef( camrot.y + ((float)frames*y)*0.0f,  0.0f, 1.0f, 0.0f );
	glRotatef( camrot.z + ((float)frames*z)*0.0f,  0.0f, 0.0f, 1.0f );
	
	POGEL::IncrementFps();
	POGEL::PrintFps();
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	
	sim.increment();
	sim.draw();
	
	// since this is double buffered, swap the buffers to display what just got drawn.
	glutSwapBuffers();
}

