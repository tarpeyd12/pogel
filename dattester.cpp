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

FILE *data_file;

POGEL::POINT* data_points;
unsigned long num_data_points = 0;
unsigned long max = 0;
float minh = 0, maxh = 0;
float minz = 0, maxz = 0;

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
	data_file = fopen("data.log", "r");
	while(!feof(data_file)) {
		POGEL::message("line count: %d\n",num_data_points++);
		unsigned long d;
		float f;
		fscanf(data_file, "%lu, %f, \n", &d, &f);
		if(d > max)
			max = d;
		if(f < minh)
			minh = f;
		if(f > maxh)
			maxh = f;
	}
	rewind(data_file);
	data_points = new POGEL::POINT[num_data_points];
	unsigned int i = 0;
	while(i++ < num_data_points-1) {
		POGEL::message("%d of %d\n", i, num_data_points);
		unsigned long d;
		float f;
		fscanf(data_file, "%lu, %f, \n", &d, &f);
		data_points[i] = POGEL::POINT((float)d, f, 0);
	}
	fclose(data_file);
	i = 0;
	while(i++ < num_data_points-2) {
		data_points[i].z = (data_points[i+0].y - data_points[i+1].y)*50;
		if(data_points[i].z > maxz)
			maxz = data_points[i].z;
		if(data_points[i].z < minz)
			minz = data_points[i].z;
	}
}
bool keypres, go = true;
POGEL::POINT camrot, campos;
POGEL::POINT size_scalar(5,4,1);
/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	MOUSE_ROT_FUNC
	glTranslatef(0.0f+campos.x,0.0f+campos.y,-5.0f+campos.z);
	glRotatef( camrot.x,  1.0f, 0.0f, 0.0f );
	glRotatef( camrot.y,  0.0f, 1.0f, 0.0f );
	glRotatef( camrot.z,  0.0f, 0.0f, 1.0f );
	//rings.rotate(VECTOR(0.0f,0.6f,0.0f));
	//glRotatef(23.0f,0.0f,0.0f,-1.0f);
	
	POGEL::IncrementFps();
	POGEL::PrintFps();
	
	for(unsigned int i = 0; i< num_data_points-1; i++) {
		POGEL::POINT a, b;
		a = POGEL::POINT(data_points[i+0].x/num_data_points,(data_points[i+0].y)/(fabs(maxh)+fabs(minh))/2,data_points[i+0].z/maxz/2) + POGEL::POINT(-(float)max/2/num_data_points,0,0);
		b = POGEL::POINT(data_points[i+1].x/num_data_points,(data_points[i+1].y)/(fabs(maxh)+fabs(minh))/2,data_points[i+1].z/maxz/2) + POGEL::POINT(-(float)max/2/num_data_points,0,0);
		POGEL::COLOR c(float(i)/float(num_data_points)*0.0 + 0.5,(data_points[i].y-minh)/(maxh-minh),fabs(data_points[i].z)/(maxz)*1.0,1);
		POGEL::LINE(a*size_scalar, b*size_scalar, 2, c).draw();
		//POGEL::LINE(a*size_scalar, POGEL::POINT(), 2, c).draw();
		//a.print(); b.print();
	}
	
	if(keys['[']) size_scalar.x-=0.25;
	if(keys[']']) size_scalar.x+=0.25;
	if(keys['{']) size_scalar.x-=0.5;
	if(keys['}']) size_scalar.x+=0.5;
	if(keys['{'] && keys['}']) size_scalar.x = 0;
	
	if(keys[';']) size_scalar.y-=0.25;
	if(keys['\'']) size_scalar.y+=0.2;
	if(keys[':']) size_scalar.y-=0.5;
	if(keys['\"']) size_scalar.y+=0.5;
	if(keys[':'] && keys['\"']) size_scalar.y = 0;
	
	if(keys[',']) size_scalar.z-=0.1;
	if(keys['.']) size_scalar.z+=0.1;
	if(keys['<']) size_scalar.z-=0.5;
	if(keys['>']) size_scalar.z+=0.5;
	if(keys['<'] && keys['>']) size_scalar.z = 0;
	
	if(keys['/']) size_scalar = POGEL::POINT(5,4,1);
	
	glutSwapBuffers();
}

