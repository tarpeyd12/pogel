#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>		// Header File For The OpenGL32 Library
#include <GL/glu.h>		// Header File For The GLu32 Library
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "scene.h"
#include "window.h"

#include "pogel/pogel.h"

using namespace POGEL;

POGEL::PHYSICS::SIMULATION simulation;

POGEL::PHYSICS::SOLID cube("cube"), sphere("sphere");

GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightDiffuse[]= { 5.0f, 5.0f, 5.0f, 5.0f };
GLfloat LightPosition[]= { 5.0f, 0.0f, 0.0f, 1.0f };

bool dimlock = !true;

void oob(SOLID_FNC_DEF) {
	if(dimlock) {
		obj->position.z = 0.0;
		obj->direction.z = 0.0f;
	}
}

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
	glEnable(GL_TEXTURE_2D);				// Enable Texture Mapping
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	// Clear The Background Color To Blue 
	glClearDepth(1.0);						// Enables Clearing Of The Depth Buffer
	glDepthFunc(GL_LESS);					// The Type Of Depth Test To Do
	glEnable(GL_DEPTH_TEST);				// Enables Depth Testing
	glShadeModel(GL_SMOOTH);				// Enables Smooth Color Shading
	//glShadeModel(GL_FLAT);				// Enables flat Color Shading
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH,GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	
	/*glColor4f(1,1,1,.5);			// Full Brightness, 50% Alpha ( NEW )
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);*/
	
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
	
	addCube(&cube, 1,1,1, POGEL::requestImage("{[Data/default_2.bmp],[32],[32],[2]}"),1,1, TRIANGLE_LIT, POGEL::MATRIX());
	cube.addproperty(OBJECT_DRAW_DISPLAYLIST); cube.build();
	addSphere(&sphere, 8,8, .5, POGEL::requestImage("{[Data/default_2.bmp],[32],[32],[2]}"),2,4, TRIANGLE_VERTEX_NORMALS, POGEL::MATRIX());
	sphere.addproperty(OBJECT_DRAW_DISPLAYLIST); sphere.build();
	
	POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES defprp(0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, false, 0.0);
	POGEL::PHYSICS::SOLID *tmp;
	for(unsigned int i = 0; i < 50; i++) {
		printf("%u\r", i);
		tmp = new POGEL::PHYSICS::SOLID(defprp, PHYSICS_SOLID_VOLITAL|PHYSICS_SOLID_CONVEX);
		switch(i) {
			/*case 0:
				tmp->referencetriangles(&cube);
				tmp->base = cube.base;
				tmp->setname("cube");
			break;*/
			default:
			case 1:
				tmp->referencetriangles(&sphere);
				tmp->base = sphere.base;
				tmp->setname("sphere");
				tmp->addOption(PHYSICS_SOLID_SPHERE);
			break;
		};
		tmp->position = POGEL::POINT(POGEL::FloatRand(2)-1, POGEL::FloatRand(2)-1, POGEL::FloatRand(2)-1)*10;
		//tmp->direction = POGEL::VECTOR(POGEL::FloatRand(2)-1, POGEL::FloatRand(2)-1, POGEL::FloatRand(2)-1).normal()/2;
		if(dimlock)
			tmp->position.z = tmp->direction.z = 0.0;
		tmp->build();
		tmp->addproperty(OBJECT_DRAW_DISPLAYLIST);
		tmp->setStepFunc(oob);
		simulation.addSolid(tmp);
		//tmp->visable=false;
	}
	tmp = new POGEL::PHYSICS::SOLID(defprp, PHYSICS_SOLID_CONCAVE|PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_STATIONARY);
	addSphere(tmp, 24,24, 10, POGEL::requestImage("{[Data/default_2.bmp],[32],[32],[2]}"),2,4, TRIANGLE_VERTEX_NORMALS, POGEL::MATRIX());
	tmp->visable=false;
	tmp->setname("border");
	tmp->addproperty(OBJECT_DRAW_DISPLAYLIST);
	POGEL::addproperty(POGEL_WIREFRAME);
	tmp->build();
	POGEL::removeproperty(POGEL_WIREFRAME);
	simulation.addSolid(tmp);
	
	tmp = new POGEL::PHYSICS::SOLID(defprp, PHYSICS_SOLID_CONVEX|PHYSICS_SOLID_STATIONARY);
	addCube(tmp, 6,6,6, POGEL::requestImage("{[Data/default_2.bmp],[32],[32],[2]}"),1,1, TRIANGLE_LIT, POGEL::MATRIX());
	tmp->moveto(POGEL::POINT(0,-6,0));
	tmp->spin.y = 1;
	tmp->setname("box");
	tmp->addproperty(OBJECT_DRAW_DISPLAYLIST);
	tmp->build();
	simulation.addSolid(tmp);
	
	simulation.gravity = POGEL::VECTOR(0,-9.8,0);
	//simulation.addsingularity(POGEL::PHYSICS::SINGULARITY(0,0,0, 2000000000));
	
	//POGEL::addproperty(POGEL_WIREFRAME);
	
	simulation.setThreadsNum(1);
	
	//simulation.deactivation = true;
	//simulation.precision = 0.05;
	
	simulation.boundingskips = 0;
	
	POGEL::InitFps();
}
bool keypres, go = true;
POGEL::POINT camrot(0,0,0), campos(0,0,-30);
/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	
	MOUSE_ROT_FUNC
	glTranslatef(campos.x,campos.y,campos.z);
	glRotatef( camrot.x + ((float)frames)*0.0f,  1.0f, 0.0f, 0.0f );
	glRotatef( camrot.y + ((float)frames)*0.0f,  0.0f, 1.0f, 0.0f );
	glRotatef( camrot.z + ((float)frames)*0.0f,  0.0f, 0.0f, 1.0f );
	
	POGEL::IncrementFps();
	POGEL::PrintFps();
	
	LightPosition[0] = cos((float)frames/200)*20;
	LightPosition[2] = sin((float)frames/200)*20;
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	if(keys['\r']) go = !go;
	
	if(go || keypres)
		simulation.increment();
	simulation.draw();
	
	if(keypres) keypres = false;
	// since this is double buffered, swap the buffers to display what just got drawn.
	glutSwapBuffers();
}

