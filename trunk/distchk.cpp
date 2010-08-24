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

OBJECT obj[2];

PHYSICS::SOLID *box1, *box2;

IMAGE *img;

GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightDiffuse[]= { 5.0f, 5.0f, 5.0f, 5.0f };
GLfloat LightPosition[]= { 5.0f, 0.0f, 0.0f, 1.0f };

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
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				// Reset The Projection Matrix
	
	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window
    
	glMatrixMode(GL_MODELVIEW);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	
	img=new IMAGE("Data/earth.bmp");
	
	obj[0].setname("box");
	addCube(&obj[0], 1.0f,1.0f,1.0f, img, 1,1,0,POGEL::MATRIX());
	//addSphere(&obj[0],5,5, 0.5, img,1,1, 0, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
	obj[0].setproperties(0 | OBJECT_DRAW_DISPLAYLIST);
	obj[0].build();
	obj[0].moveto(POINT(0.0f,2.0f,0.0f));
	//obj[0].spin = VECTOR(1.0f,1.0f,1.0f)*50;
	box1 = new PHYSICS::SOLID(&obj[0], PHYSICS::SOLIDPHYSICALPROPERTIES(), 2|4);
	box1->build();
	box1->resizetrail(25);
	box1->spin = VECTOR(1.0f,1.0f,1.0f);
	
	obj[1].setname("box");
	//addCube(&obj[1], 10.0f,10.0f,10.0f, img, 1,1,0,POGEL::MATRIX());
	//addSphere(&obj[1],3,3, 10, img,1,1, 0, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
	//addCylinder(&obj[1], 10, 1, 1, 1/2.0f, 1/2.0f, img, 1.0f, 1.0f, 0, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
	addDisk(&obj[1], 10, 1, 1/2.0f, 0.0f, img,1, 1, 0, true, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
	obj[1].setproperties(0 | OBJECT_DRAW_DISPLAYLIST);
	obj[1].build();
	obj[1].moveto(POINT(0.0f,-2.0f,0.0f));
	//obj[1].spin = VECTOR(1.0f,1.0f,1.0f)*50;
	box2 = new PHYSICS::SOLID(&obj[1], PHYSICS::SOLIDPHYSICALPROPERTIES(), 2|4);
	box2->build();
	box2->resizetrail(25);
	box2->spin = VECTOR(0.0f,0.0f,1.0f);
}
bool keypres, go = true;
POGEL::POINT camrot, campos;
/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	
	glTranslatef(0.0f,0.0f,-15.0f);
	glRotatef( camrot.x,  1.0f, 0.0f, 0.0f );
	glRotatef( camrot.y,  0.0f, 1.0f, 0.0f );
	glRotatef( camrot.z,  0.0f, 0.0f, 1.0f );
	//rings.rotate(VECTOR(0.0f,0.6f,0.0f));
	//glRotatef(23.0f,0.0f,0.0f,-1.0f);
	
	POGEL::IncrementFps();
	POGEL::PrintFps();
	
	box1->draw();
	box2->draw();
	
	POGEL::POINT tmp_1, tmp_2;
	POGEL::TRIANGLE tmptri;
	
	box1->closest(box2, &tmp_1, &tmp_2, &tmptri, &tmptri);
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glLineWidth(1);
	glColor3f(0.0f,0.75f,0.75f);
	glBegin(GL_LINES);
		glVertex3f(tmp_1.x,tmp_1.y,tmp_1.z);
		glVertex3f(tmp_2.x,tmp_2.y,tmp_2.z);
	glEnd();
	glLineWidth(1);
	glColor3f(1.0f,1.0f,1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
		
	box2->closest(box1, &tmp_1, &tmp_2, &tmptri, &tmptri);
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glLineWidth(1);
	glColor3f(0.0f,0.75f,0.75f);
	glBegin(GL_LINES);
		glVertex3f(tmp_1.x,tmp_1.y,tmp_1.z);
		glVertex3f(tmp_2.x,tmp_2.y,tmp_2.z);
	glEnd();
	glLineWidth(1);
	glColor3f(1.0f,1.0f,1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	
	
	/*tmp_1 = box2->position;
	box1->closest(tmp_1, &tmp_2, &tmptri);
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glLineWidth(1);
	glColor3f(0.0f,1.75f,0.75f);
	glBegin(GL_LINES);
		glVertex3f(tmp_1.x,tmp_1.y,tmp_1.z);
		glVertex3f(tmp_2.x,tmp_2.y,tmp_2.z);
	glEnd();
	glLineWidth(1);
	glColor3f(1.0f,1.0f,1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	
	
	tmp_1 = box1->position;
	box2->closest(tmp_1, &tmp_2, &tmptri);
	
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glLineWidth(1);
	glColor3f(0.0f,1.75f,0.75f);
	glBegin(GL_LINES);
		glVertex3f(tmp_1.x,tmp_1.y,tmp_1.z);
		glVertex3f(tmp_2.x,tmp_2.y,tmp_2.z);
	glEnd();
	glLineWidth(1);
	glColor3f(1.0f,1.0f,1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);*/
	
	box1->increment();
	box2->increment();
	//obj.scroll_all_tex_values(0.0004f,0.0005f);
	// since this is double buffered, swap the buffers to display what just got drawn.
	glutSwapBuffers();
}
