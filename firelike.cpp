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

#define numobjs 11
#define numcosms 10
#define grd 1
#define sps 0.10f
#define size 0.15f
OBJECT obj;
POGEL::PHYSICS::FOUNTAIN *sphs;

POGEL::PHYSICS::FOUNTAIN sim;

//IMAGE *earth;
IMAGE *defaultimg;

float x = POGEL::FloatRand(10.0f)-5.0f, y = POGEL::FloatRand(10.0f)-5.0f, z = POGEL::FloatRand(10.0f)-5.0f;

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 2.0f, 2.0f, 2.0f, 1.0f };
GLfloat LightPosition[]= { 0.0f, 0.0f, 0.0f, 1.0f };


void fireyness(SOLID_FNC_DEF) {
	if(obj->stepstaken >= (unsigned long)(25-(rand()%25)) ) {
		obj->moveto(POGEL::POINT(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)*POGEL::FloatRand(0.25));
		obj->direction = POGEL::VECTOR(0.0f,-20.0f,0.0f)/PARTICLE_SLOWDOWN * POGEL::POINT(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5);
		obj->stepstaken = 0;
	}
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
	glColor4f(1.0f,1.0f,1.0f,0.5f);			// Full Brightness, 50% Alpha ( NEW )
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	
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
	defaultimg=new IMAGE("Data/lava8.bmp");
	
	obj.setname("base_object");
	
	//addDisk(&obj, 4, 1, size/2.0f, 0.0f, defaultimg,1, 1, 0, true, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
	//addSphere(&obj,2,4, size/2.0f, defaultimg,1,1, 0, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
	//addCylinder(&obj, 8, 1, size, size/2.0f, size/2.0f, defaultimg, 1.0f, 1.0f, 0, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
	addCube(&obj, size,size,size, defaultimg, 1,1,0,POGEL::MATRIX());
	
	//obj.setproperties(8);
	
	//obj.build();
	
	sphs = new POGEL::PHYSICS::FOUNTAIN[numcosms];
	
	//POGEL::MATRIX matcosmtransform(POGEL::POINT(), POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0)));
	POGEL::MATRIX matcosmtransform(POGEL::POINT(), POGEL::POINT(0.0f,0.0f,0.0f));
	
	for(unsigned long cosm = 0; cosm < numcosms; cosm++) {
		//matcosmtransform = matcosmtransform * POGEL::MATRIX(POGEL::POINT(), POGEL::POINT(x,y,z));
		
		POGEL::MATRIX matobjtransform;//(POGEL::POINT(), POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0)));
		
		for(unsigned long objs = 0; objs < numobjs; objs++) {
			
			//matobjtransform = matobjtransform * POGEL::MATRIX(POGEL::POINT(), POGEL::POINT(x,y,z));
			
			POGEL::PHYSICS::SOLID* tmp = new POGEL::PHYSICS::SOLID();// = new POGEL::PHYSICS::SOLID(&obj, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, false, (objs%2==0?-10.0f:10.0f)), 2);
			
			tmp->setname(POGEL::string("object%ld_of_microcosm_%ld", objs, cosm));
			
			//addCube(tmp, size,size,size, defaultimg, 1,1,0,POGEL::MATRIX());
			addSphere(tmp,2,4, size/2.0f, defaultimg,1,1, 0 /*| TRIANGLE_VERTEX_NORMALS*/, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
			//tmp->setproperties(8);
			//tmp->addobject(&obj);
			
			tmp->behavior = POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1.0f, 1.0f, 2.0f, 0.0f, 0.0f, 0.0f, false, (cosm%2==0?-1.0f:1.0f));
			tmp->setOptions(2);
			
			tmp->setStepFunc(fireyness);
			
			//tmp->moveto(matobjtransform.transformPoint(POGEL::POINT(0.0f, ((float)objs+(sps*40))*sps+sps*5, 0.0f)));
			//tmp->moveto(POGEL::POINT(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)*POGEL::FloatRand(0.5));
			tmp->direction = POGEL::VECTOR(0.0f,-5.0f,0.0f)/PARTICLE_SLOWDOWN;
			tmp->spin=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)/0.05f * VECTOR(1.0f,1.0f,1.0f);
			
			tmp->resizetrail(10);
			
			tmp->build();
			
			sphs[cosm].addSolid(tmp);
		}
		//sphs[cosm].addsingularity( POGEL::PHYSICS::SINGULARITY(POGEL::POINT(0.0f,0.0f,0.0f),sps*20000000000000.0f) );
		sphs[cosm].gravity = POGEL::VECTOR(0.0f, 1.0f, 0.0f) * 9.8f;
		
		sphs[cosm].setname(POGEL::string("microcosm_%ld", cosm));
		
		//addCube(&sphs[cosm], size,size,size, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX());
		//sphs[cosm].addobject(&obj);
		
		sphs[cosm].behavior = POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, false, (cosm%2==0?-1.0f:1.0f));
		sphs[cosm].setOptions(2);
		
		sphs[cosm].moveto(matcosmtransform.transformPoint(POGEL::POINT(0.0f, ((float)cosm+1)*sps, 0.0f)*10));
		sphs[cosm].direction = matcosmtransform.transformVector(POGEL::VECTOR((float)sqrt((100000000000.0f*(GRAVITYCONSTANT/PARTICLE_SLOWDOWN))/(sphs[cosm].position.distance(POGEL::POINT()) )), 0.0f, 0.0f));
		//sphs[cosm].spin=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)/5.0f * VECTOR(1.0f,1.0f,1.0f);
		
		sphs[cosm].resizetrail(100);
		
		sphs[cosm].setproperties(/*1|*/2);
		
		sphs[cosm].build();
		
		sphs[cosm].boundingskips = 0;
		
		sim.addSolid(&sphs[cosm]);
	}
	sim.addsingularity( POGEL::PHYSICS::SINGULARITY(POGEL::POINT(0.0f,0.0f,0.0f),100000000000.0f) );
	
	sim.addSolid(new POGEL::PHYSICS::SOLID(&obj, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, false, 0.0f), 1));
	
	sim.boundingskips = 0;
	
	POGEL::InitFps();
	printf("\n");
}

//unsigned long frames=0;

bool keypres, go;
POGEL::POINT camrot, campos;
/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	MOUSE_ROT_FUNC
	//glTranslatef(0.0f,0.0f,-25.0f*2.0f);
	glTranslatef(0.0f+campos.x,0.0f+campos.y,-10.0f+campos.z);
	//glRotatef( 90.0f,  1.0f, 0.0f, 0.0f );
	glRotatef( camrot.x + ((float)frames*x)*0.0f,  1.0f, 0.0f, 0.0f );
	glRotatef( camrot.y + ((float)frames*y)*0.0f,  0.0f, 1.0f, 0.0f );
	glRotatef( camrot.z + ((float)frames*z)*0.0f,  0.0f, 0.0f, 1.0f );
	//glRotatef( 90.0f,  0.0f, 1.0f, 0.0f );
	//message("%ld: ",frames);
	
	POGEL::IncrementFps();
	if(frames%1 == 0)
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

