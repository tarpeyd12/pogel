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

using namespace POGEL;

POGEL::POINT camrot;

#define numobjs 50
#define numcosms 5
#define grd 1
#define sps 0.10f
#define size 0.15f
OBJECT obj;
POGEL::PHYSICS::FOUNTAIN *sphs;

POGEL::PHYSICS::DYNAMICS sim;

//IMAGE *earth;
IMAGE *defaultimg;

float x = POGEL::FloatRand(10.0f)-5.0f, y = POGEL::FloatRand(10.0f)-5.0f, z = POGEL::FloatRand(10.0f)-5.0f;

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 2.0f, 2.0f, 2.0f, 1.0f };
GLfloat LightPosition[]= { 0.0f, 0.0f, 0.0f, 1.0f };

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
	//sim.deactivation = false;
	//sim.precision = 0.001f;
	
	srand((unsigned)time(NULL));
	
	//earth=new IMAGE("Data/earth.bmp");
	defaultimg=new IMAGE("Data/default_2.bmp");
	
	obj.setname("base_object");
	
	//addDisk(&obj, 4, 1, size/2.0f, 0.0f, defaultimg,1, 1, 0, true, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
	//addSphere(&obj,2,4, size/2.0f, defaultimg,1,1, 0, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
	//addCylinder(&obj, 8, 1, size, size/2.0f, size/2.0f, defaultimg, 1.0f, 1.0f, 0, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
	addCube(&obj, size,size,size, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX());
	
	//obj.setproperties(8);
	
	//obj.build();
	
	sphs = new POGEL::PHYSICS::FOUNTAIN[numcosms];
	
	POGEL::MATRIX matcosmtransform(POGEL::POINT(), POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0)));
	
	for(unsigned long cosm = 0; cosm < numcosms; cosm++) {
		matcosmtransform = matcosmtransform * POGEL::MATRIX(POGEL::POINT(), POGEL::POINT(x,y,z));
		
		POGEL::MATRIX matobjtransform(POGEL::POINT(), POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0)));
		
		for(unsigned long objs = 0; objs < numobjs; objs++) {
			
			matobjtransform = matobjtransform * POGEL::MATRIX(POGEL::POINT(), POGEL::POINT(x,y,z));
			
			POGEL::PHYSICS::SOLID* tmp = new POGEL::PHYSICS::SOLID();// = new POGEL::PHYSICS::SOLID(&obj, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, false, (objs%2==0?-10.0f:10.0f)), 2);
			
			tmp->setname(POGEL::string("object%ld_of_microcosm_%ld", objs, cosm));
			
			addCube(tmp, size,size,size, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX());
			//tmp->setproperties(8);
			//tmp->addobject(&obj);
			
			tmp->behavior = POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, false, (cosm%2==0?-1.0f:1.0f));
			tmp->setOptions(2);
			
			tmp->moveto(matobjtransform.transformPoint(POGEL::POINT(0.0f, ((float)objs+(sps*30))*sps, 0.0f)));
			tmp->direction = matobjtransform.transformVector(POGEL::VECTOR((float)sqrt((sps*20000000000000.0f*(GRAVITYCONSTANT/PARTICLE_SLOWDOWN))/(tmp->position.distance(POGEL::POINT()) )), 0.0f, 0.0f));
			tmp->spin=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)/0.05f * VECTOR(1.0f,1.0f,1.0f);
			
			tmp->resizetrail(25);
			
			tmp->build();
			
			sphs[cosm].addSolid(tmp);
		}
		sphs[cosm].addsingularity( POGEL::PHYSICS::SINGULARITY(POGEL::POINT(0.0f,0.0f,0.0f),sps*20000000000000.0f) );
		
		sphs[cosm].setname(POGEL::string("microcosm_%ld", cosm));
		
		addCube(&sphs[cosm], size,size,size, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX());
		//sphs[cosm].addobject(&obj);
		
		sphs[cosm].behavior = POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, false, (cosm%2==0?-1.0f:1.0f));
		sphs[cosm].setOptions(2);
		
		sphs[cosm].moveto(matcosmtransform.transformPoint(POGEL::POINT(0.0f, ((float)cosm+1)*sps, 0.0f)*(float)numobjs));
		sphs[cosm].direction = matcosmtransform.transformVector(POGEL::VECTOR((float)sqrt((1000000000000.0f*(GRAVITYCONSTANT/PARTICLE_SLOWDOWN))/(sphs[cosm].position.distance(POGEL::POINT()) )), 0.0f, 0.0f));
		//sphs[cosm].spin=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)/5.0f * VECTOR(1.0f,1.0f,1.0f);
		
		sphs[cosm].resizetrail(250);
		
		//sphs[cosm].setproperties(FOUNTAIN_USE_MASTER_GRAVITY);
		
		sphs[cosm].build();
		
		sim.addSolid(&sphs[cosm]);
	}
	sim.addsingularity( POGEL::PHYSICS::SINGULARITY(POGEL::POINT(0.0f,0.0f,0.0f),1000000000000.0f) );
	
	sim.addSolid(new POGEL::PHYSICS::SOLID(&obj, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false, 0.0f), 1));
	
	
	
	POGEL::InitFps();
	printf("\n");
}

//unsigned long frames=0;

bool keypres, go;
/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	
	glTranslatef(0.0f,0.0f,-25.0f*2.0f);
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
	
	//frames++;
	//border->rotate(POGEL::VECTOR(0.0f,1.0f,0.0f)/1.0f);
	//if(frames < 100)
	
	/*for(int i = 0; i < 1; i++)
		sim.increment();*/
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
}

