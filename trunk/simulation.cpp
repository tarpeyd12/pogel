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

POGEL::POINT camrot;

#define numobjs 150
#define grd 5
#define sps 2.01f
#define size 0.5f
OBJECT obj[numobjs];
POGEL::PHYSICS::SOLID **sphs;
POGEL::PHYSICS::SOLID *border;

POGEL::PHYSICS::SIMULATION sim;

IMAGE *earth;
IMAGE *defaultimg;

float x = POGEL::FloatRand(10.0f)-5.0f, y = POGEL::FloatRand(10.0f)-5.0f, z = POGEL::FloatRand(10.0f)-5.0f;

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 2.0f, 2.0f, 2.0f, 1.0f };
GLfloat LightPosition[]= { 50.0f, 50.0f, 50.0f, 1.0f };

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
	
	sim.boundingskips = 2;
	
	srand((unsigned)time(NULL));
	
	sphs=(POGEL::PHYSICS::SOLID**)malloc(sizeof(POGEL::PHYSICS::SOLID*)*numobjs);
	
	//earth=new IMAGE("Data/earth.bmp");
	defaultimg=new IMAGE("Data/default_2.bmp");
	
	POGEL::MATRIX m(POGEL::POINT(), POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0)));
	
	//printf("obj = %p\n",&obj);
	for(int i=0;i<numobjs;i++) {
		
		m = m * POGEL::MATRIX(POGEL::POINT(), POGEL::POINT(x,y,z));
		//POGEL::MATRIX m(POGEL::POINT(), POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0)));
		
		obj[i].setname(POGEL::string("sphere%d",i));
		//addDisk(&obj[i], 4, 1, size/2.0f, 0.0f, defaultimg,1, 1, 0, true, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
		addSphere(&obj[i],2,3, size/2.0f, defaultimg,1,1, 0 | TRIANGLE_LIT, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
		//addCylinder(&obj[i], 10, 1, size, size/2.0f, size/2.0f, defaultimg, 1.0f, 1.0f, 0, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
		//addCube(&obj[i], size,size,size, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX());
		
		obj[i].setproperties(0);
		//obj[i].build();
		obj[i].moveto(POINT(POGEL::FloatRand(sps)-sps/2.0,POGEL::FloatRand(sps)-sps/2.0,POGEL::FloatRand(sps)-sps/2.0)/0.5f * POINT(1.0f,1.0f,1.0f));
		//obj[i].moveto(POINT( \
			((float)(i%grd)*sps)-( (float(grd)*sps)/2.0f-sps/2.0f), \
			(float)(i/(grd*grd))*(sps) - (10.0f-(sps/2.0f)), \
			((float)((i/grd)%grd)*sps)-( (float(grd)*sps)/2.0f-sps/2.0f) \
		));
		obj[i].moveto(m.transformPoint(POINT(0.0f, ((float)i+1)*sps, 0.0f)));
		//obj[i].moveto(POINT(0.0f,(float)(i)*2.75f,0.0f));
		//obj[i].turnto(POINT(POGEL::FloatRand(360.0), POGEL::FloatRand(360.0), POGEL::FloatRand(360.0)) * POINT(1.0f,1.0f,1.0f));
		//obj[i].turnto(POINT());
		sphs[i] = new POGEL::PHYSICS::SOLID(&obj[i], POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, false, (i%2==0?-1.0f:1.0f)), 2|4);
		//sphs[i]->moveto(POINT(POGEL::FloatRand(5.0)-2.5,POGEL::FloatRand(5.0)-2.5,POGEL::FloatRand(5.0)-2.5));
		//sphs[i]->position.print();
		//sphs[i]->turnto(POINT(POGEL::FloatRand(20.0)-10,POGEL::FloatRand(20.0)-10,POGEL::FloatRand(20.0)-10));
		//sphs[i]->turnto(POINT(0.0f,0.0f,0.0f));
		sphs[i]->build();
		//sphs[i]->direction=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)/2.0f * VECTOR(1.0f,1.0f,1.0f);
		sphs[i]->direction = m.transformVector(POGEL::VECTOR((float)sqrt((10000000000000.0f*(GRAVITYCONSTANT/PARTICLE_SLOWDOWN))/(obj[i].position.distance(POGEL::POINT()) )), 0.0f, 0.0f));
		sphs[i]->spin=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)/0.05f * VECTOR(1.0f,1.0f,1.0f);
		//sphs[i]->direction = POGEL::VECTOR(0.05f,0.25f,0.0f);
		//sphs[i]->spin=POGEL::VECTOR(0.0f,1.0f,0.0f);
		//sphs[i]->visable = false;
		
		//if(i == numobjs-1) { \
			sphs[i]->setOptions(1|4); \
			sphs[i]->moveto(POGEL::POINT(0.0f,-10.0f+sps/2.0f,0.0f)); \
		}
		
		sphs[i]->resizetrail(100);
		
		sim.addSolid(sphs[i]);
	}
	sim.addsingularity( POGEL::PHYSICS::SINGULARITY(POGEL::POINT(0.0f,0.0f,0.0f),10000000000000.0f) );
	//sim.addfan(PHYSICS::FAN(POINT(0.0f,0.0f,0.0f), VECTOR(0.0f,1.0f,0.0f), 20.0f));
	//sim.gravity = POGEL::VECTOR(0.0f,-1.0f,0.0f)*9.8f;
	//sim.air_dencity = 1.0f;
	
	POGEL::OBJECT *ring = new POGEL::OBJECT();
	ring->setname("border");
	
	//addCylinder(ring, 16, 1, 20.0f, 50.0f, 20.0f, defaultimg, 4.0f, 4.0f, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,10.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
	//addDisk(ring, 16, 1, 20.0f, 0.0f, defaultimg,1, 1, 0|TRIANGLE_LIT, false, MATRIX(VERTEX(0.0f,-0.01f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
	//addDisk(ring, 3, 1, 200.0f, 0.0f, defaultimg,1, 1, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, true, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
	//addDisk(ring, 16, 1, 20.0f, 2.0f, defaultimg,1, 1, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, true, MATRIX(VERTEX(0.0f,2.01f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
	//addCylinder(ring, 16, 1, 2.01f, 20.0f, 20.0f, defaultimg, 1.0f, 1.0f, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,1.005,0.0f), VERTEX(0.0f,0.0f,0.0f)));
	//addCylinder(ring, 4, 1, 5.0f, 20.0f, 5.0f, defaultimg, 1.0f, 1.0f, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,-2.5f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
	
	//addCube(ring, 20.0f,20.0f,20.0f, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX(POGEL::POINT(0.0f,0.0f,0.0f),POGEL::POINT(0.0f,0.0f,0.0f)));
	
	/*addCube(ring, 20.0f,20.0f,20.0f, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX(POGEL::POINT(20.0f,10.0f,0.0f),POGEL::POINT(0.0f,0.0f,0.0f)));
	addCube(ring, 20.0f,20.0f,20.0f, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX(POGEL::POINT(-20.0f,10.0f,0.0f),POGEL::POINT(0.0f,0.0f,0.0f)));*/
	
	addSphere(ring,8,16, 5.0f, defaultimg,1,1, 0 | TRIANGLE_VERTEX_NORMALS, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
	
	//addCylinder(ring, 16, 1, 20.0f, 20.0f, 20.0f, defaultimg, 4.0f, 4.0f, 0 | TRIANGLE_LIT | TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
	//addDisk(ring, 16, 1, 20.0f, 17.5f, defaultimg,1, 1, 0 | TRIANGLE_LIT, true, MATRIX(VERTEX(0.0f,0.0f,10.0f), VERTEX(0.0f,0.0f,180.0f)));
	//addDisk(ring, 16, 1, 20.0f, 0.0f, defaultimg,1, 1, 0 | TRIANGLE_LIT, true, MATRIX(VERTEX(0.0f,0.0f,-10.0f), VERTEX(0.0f,0.0f,180.0f)));
	
	ring->setproperties(0);
	ring->moveto(POGEL::POINT(0.0f,0.0f,0.0f));
	ring->turnto(POGEL::POINT(0.0f,0.0f,0.0f));
	//ring->build();
	border = new POGEL::PHYSICS::SOLID(ring, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(), PHYSICS_SOLID_STATIONARY|4);
	//border->spin = POGEL::VECTOR(POGEL::FloatRand(1.0f),POGEL::FloatRand(1.0f),POGEL::FloatRand(1.0f))*1.0f;
	//border->spin = POGEL::VECTOR(0.0f,1.0f,0.0f);
	border->behavior.bounce = 1.0f;
	border->behavior.friction = 1.0f;
	
	border->behavior.magnetic = false;
	border->behavior.charge = -0.01f;
	border->build();
	//border->visable = false;
	//border->spin = POGEL::VECTOR(0.0f,1.0f,0.0f);
	//sim.addSolid(border);
	
	
	POGEL::InitFps();
	printf("\n");
	//camrot.x = 90.0f;
}

//unsigned long frames=0;

bool keypres, go;
/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	
	/*POGEL::POINT p = POGEL::POINT();
	for(unsigned int i = 0; i < numobjs; i++)
		p += sphs[i]->position;
	p /= float(numobjs);*/
	
	/*float d = 0.0f;
	for(unsigned int i = 0; i < numobjs; i++)
		d += p.distance(sphs[i]->position);
	d /= float(numobjs);
	if(d >= 99.0f)
		d=99.0f;
	
	glTranslatef(p.x*-1.0f, p.y*-1.0f, p.z*-1.0f);
	glTranslatef(0.0f,0.0f,-(d*2.0f));*/
	
	glTranslatef(0.0f,0.0f,-30.0f);
	//glRotatef( 90.0f,  1.0f, 0.0f, 0.0f );
	glRotatef( camrot.x + ((float)frames*x)*0.0f,  1.0f, 0.0f, 0.0f );
	glRotatef( camrot.y + ((float)frames*y)*0.0f,  0.0f, 1.0f, 0.0f );
	glRotatef( camrot.z + ((float)frames*z)*0.0f,  0.0f, 0.0f, 1.0f );
	//glRotatef( 90.0f,  0.0f, 1.0f, 0.0f );
	//message("%ld: ",frames);
	
	POGEL::IncrementFps();
	POGEL::PrintFps();
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	
	//POGEL::ThrotleFps(1);
	
	//frames++;
	//border->rotate(POGEL::VECTOR(0.0f,1.0f,0.0f)/1.0f);
	//if(frames < 100)
	//for(int i = 0; i < 1; i++)
		//getchar();
		if(keypres) {
			sim.increment();
			keypres = false;
		}
		else if(go)
			sim.increment();
	if(frames%1 == 0) {
		sim.draw();
	}
	
	//message("\n");
	
	// since this is double buffered, swap the buffers to display what just got drawn.
	if(frames%1 == 0) {
	glutSwapBuffers();
	}
	
	//printf("POGEL::getproperties() = %u\n", POGEL::getproperties());
}

