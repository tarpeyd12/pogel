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
#define startsize	2.0f
#define size    	0.6f
POGEL::FRACTAL* obj;
POGEL::OBJECT* allofobj;

float *lst;

//IMAGE *earth;
IMAGE *defaultimg;

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]= { -50.0f, 0.0f, 0.0f, 1.0f };

void construct(FRACTAL_FNC_DEFS) {
	POGEL::FRACTAL * tmp;
	
	float height = startsize*((float)pow(size, (float)itter));
	//float radius = (startsize/10.0f)/((float)pow(size, (float)itter))/2.0f;
	
	//if(itter == 0)
		//addCylinder(obj, 8, 1, (startsize/((float)pow(size, (float)itter-1)))/size, (startsize/10.0f)/((float)pow(size, (float)itter-1))/2.0f, (startsize/10.0f)/((float)pow(size, (float)itter-1))/2.0f, defaultimg, 1.0f, 1.0f, 2, MATRIX(VERTEX(0.0f,startsize/((float)pow(size, (float)itter-1))/(size*2.0f),0.0f), VERTEX(0.0f,0.0f,0.0f)));
	
	tmp = obj->spawn();
		if(tmp != NULL) {
			
			tmp->setname("trunk");
			
			//addCylinder(tmp, 8, 1, height/size, radius, radius, defaultimg, 1.0f, 1.0f, 2, MATRIX(VERTEX(0.0f,height/(size*2.0f),0.0f), VERTEX(0.0f,0.0f,0.0f)));
			
			tmp->position = POGEL::POINT(0.0,height,0.0);
			tmp->rotation = POGEL::POINT(0,0,0);
			tmp->spin = POGEL::VECTOR(0,0,1);
		}
	
	
	switch(itter%1) {
	case 0:
		tmp = obj->spawn();
		if(tmp != NULL) {
			tmp->setname(POGEL::string("itter%ld_branch_0", itter));
			//addCylinder(tmp, 8, 1, height/size, radius, radius, defaultimg, 1.0f, 1.0f, 2, MATRIX(VERTEX(0.0f,height/(size*2.0f),0.0f), VERTEX(0.0f,0.0f,0.0f)));
			tmp->position = POGEL::POINT(0.0,height,0.0);
			tmp->rotation = POGEL::POINT(90,0,0);
			tmp->spin = POGEL::VECTOR(0,0,0);
		}
	break;
	
	case 2:
		tmp = obj->spawn();
		if(tmp != NULL) {
			tmp->setname(POGEL::string("itter%ld_branch_1", itter));
			//addCylinder(tmp, 8, 1, height/size, radius, radius, defaultimg, 1.0f, 1.0f, 2, MATRIX(VERTEX(0.0f,height/(size*2.0f),0.0f), VERTEX(0.0f,0.0f,0.0f)));
			tmp->position = POGEL::POINT(0.0,height,0.0);
			tmp->rotation = POGEL::POINT(0,90,0);
			tmp->spin = POGEL::VECTOR(0,0,0);
		}
	break;
	
	case 1:
		tmp = obj->spawn();
		if(tmp != NULL) {
			tmp->setname(POGEL::string("itter%ld_branch_2", itter));
			//addCylinder(tmp, 8, 1, height/size, radius, radius, defaultimg, 1.0f, 1.0f, 2, MATRIX(VERTEX(0.0f,height/(size*2.0f),0.0f), VERTEX(0.0f,0.0f,0.0f)));
			tmp->position = POGEL::POINT(0.0,height,0.0);
			tmp->rotation = POGEL::POINT(0,0,90);
			tmp->spin = POGEL::VECTOR(0,0,0);
		}
	break;
	}
};

void destruct(FRACTAL_FNC_DEFS) {
	
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
	
	lst = new float[itterations];
	for(int i = 0; i < itterations; i++)
		lst[i] = 10+(i>0 ? lst[i-1] : 0);
	
	obj = new FRACTAL(NULL, &construct, &destruct, itterations);
	obj->setname("Tree\0");
	
	//addCylinder(obj, 4, 1, (float)10, (float)10, (float)10, defaultimg, 1.0f, 1.0f, 0, MATRIX(VERTEX(0.0f,(float)10,0.0f)*0.5f, VERTEX(0.0f,0.0f,0.0f)));
	
	obj->create();
	//obj->build();
	
	obj->moveto(POGEL::POINT(0,-2,0));
	
	
	POGEL::InitFps();
	printf("\n");
}

//unsigned long frames=0;
//loat x = POGEL::FloatRand(2.0)-1.0, y = POGEL::FloatRand(2.0)-1.0, z = POGEL::FloatRand(2.0)-1.0;
float y = 0.0f;

bool keypres, go = false;
POGEL::POINT camrot = POGEL::POINT(45,45,0)*0, campos = POGEL::POINT(0,0,-5);

/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	
	MOUSE_ROT_FUNC
	glTranslatef(0.0f+campos.x,campos.y,campos.z);
	//glRotatef( 90.0f,  1.0f, 0.0f, 0.0f );
	glRotatef( camrot.x,  1.0f, 0.0f, 0.0f );
	glRotatef( camrot.y,  0.0f, 1.0f, 0.0f );
	glRotatef( camrot.z,  0.0f, 0.0f, 1.0f );
	
	//glTranslatef(0.0f,-2.0f,-25.0f*0.25f);
	//glRotatef( (float)frames * x,  1.0f, 0.0f, 0.0f );
	if(keypres||go) {
		if(keypres)
			keypres=false;
		//y += 0.1;
		obj->step();
	}
	glRotatef( y,  0.0f, 1.0f, 0.0f );
	//glRotatef( 180,  0.0f, 0.0f, 1.0f );
	//glRotatef( (float)frames * z,  0.0f, 0.0f, 1.0f );
	//glRotatef( 90.0f,  0.0f, 1.0f, 0.0f );
	
	POGEL::IncrementFps();
	POGEL::PrintFps();
	
	/*glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);*/
	
	
	obj->draw();
	//obj->step();
	
	
	// since this is double buffered, swap the buffers to display what just got drawn.
	glutSwapBuffers();
}

