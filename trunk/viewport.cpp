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

IMAGE *img;

VIEWPORT viewport[2];

GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightDiffuse[]= { 5.0f, 5.0f, 5.0f, 5.0f };
GLfloat LightPosition[]= { 25.0f, 0.0f, 0.0f, 1.0f };

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
	glEnable(GL_TEXTURE_2D);				// Enable Texture Mapping
	glClearColor(0.5f, 0.5f, 0.0f, 0.0f);	// Clear The Background Color To Blue 
	glClearDepth(5.0);						// Enables Clearing Of The Depth Buffer
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
	
	img=new IMAGE("Data/default_2.bmp", IMAGE_NEAREST);
	
	viewport[0].setretscreensize(&screenx, &screeny);
	viewport[0].setviewport(0,0,Width/2,Height);
	viewport[0].setbgcolor(POGEL::COLOR(0.0f,0.0f,0.0f,0.5f));
	viewport[0].setfilter(IMAGE_MIPMAP);
	viewport[0].build();
	
	viewport[1].setretscreensize(&screenx, &screeny);
	viewport[1].setviewport(Width/2,0,Width/2,Height);
	viewport[1].setbgcolor(POGEL::COLOR(0.0f,0.0f,0.0f,0.5f));
	viewport[1].setfilter(IMAGE_MIPMAP);
	viewport[1].build();
	
	obj[0].setname("ball");
	//addCube(&obj[0], 1.0f,1.0f,1.0f, img, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX());
	addSphere(&obj[0],32,32, 0.5, img,1,1, 0|TRIANGLE_LIT, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
	obj[0].setproperties(0);
	obj[0].build();
	obj[0].moveto(POINT(0.0f,0.0f,0.0f));
	obj[0].spin = VECTOR(1.0f,1.0f,1.0f);
	
	obj[1].setname("box");
	addCube(&obj[1], 1.0f,1.0f,1.0f, img, 1,1,0,POGEL::MATRIX());
	//addSphere(&obj[1],32,32, 10, view,1,1, 0|TRIANGLE_LIT, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,180.0f,0.0f)));
	//addCylinder(&obj[1], 10, 1, 1, 1/2.0f, 1/2.0f, view, 1.0f, 1.0f, 0, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
	//addDisk(&obj[1], 32, 32, 5, 0.0f, view,4, 1, 0, true, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
	obj[1].setproperties(0);
	obj[1].build();
	obj[1].moveto(POINT(0.0f,0.0f,0.0f));
	obj[1].spin = VECTOR(1.0f,1.0f,1.0f);
}
bool keypres, go = true;
POGEL::POINT camrot, campos;
float diff=-0.85;
/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	//glClearColor(0.5f, 0.5f, 0.0f, 1.0f);
	
	viewport[0].setviewport(0,0,screenx/2,screeny);
	viewport[1].setviewport(screenx/2,0,screenx,screeny);
	
	if(keys['c'])
		diff+=0.005;
	if(keys['v'])
		diff-=0.005;
		
	viewport[0].startrender();	
		glPushMatrix();
		glTranslatef(0,0.0f,-10.0f);
		glTranslatef(campos.x,campos.y,campos.z);
		glRotatef( camrot.x,  1.0f, 0.0f, 0.0f );
		glRotatef( camrot.y+diff,  0.0f, 1.0f, 0.0f );
		glRotatef( camrot.z,  0.0f, 0.0f, 1.0f );
		obj[0].draw();
		glPopMatrix();
	viewport[0].endrender();
	
	viewport[1].startrender();
		glPushMatrix();
		glTranslatef(0,0.0f,-10.0f);
		glTranslatef(campos.x,campos.y,campos.z);
		glRotatef( camrot.x,  1.0f, 0.0f, 0.0f );
		glRotatef( camrot.y-diff,  0.0f, 1.0f, 0.0f );
		glRotatef( camrot.z,  0.0f, 0.0f, 1.0f );
		obj[0].draw();
		glPopMatrix();
	viewport[1].endrender();
	
	//viewport[0].resetscreen();
	
	
	//glClearColor(0.5f, 1.0f, 0.0f, 1.0f); // the background color of the of the to be rendered scene
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		// Clear The Screen And The Depth Buffer
	//glLoadIdentity();
	MOUSE_ROT_FUNC
	
	//rings.rotate(VECTOR(0.0f,0.6f,0.0f));
	//glRotatef(23.0f,0.0f,0.0f,-1.0f);
	
	POGEL::IncrementFps();
	//POGEL::PrintFps();
	
	cout << diff << endl;
	
	
	// since this is double buffered, swap the buffers to display what just got drawn.
	glutSwapBuffers();
}

