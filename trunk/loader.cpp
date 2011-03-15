#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>		// Header File For The OpenGL32 Library
#include <GL/glu.h>		// Header File For The GLu32 Library
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include <iostream>
#include <fstream>

#include "scene.h"
#include "window.h"

#include "pogel/pogel.h"

using namespace POGEL;

PHYSICS::SOLID* obj;
PHYSICS::SIMULATION sim;

IMAGE *img, *glass, *particle, *defaultimg;

GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightDiffuse[]= { 5.0f, 5.0f, 5.0f, 5.0f };
GLfloat LightPosition[]= { 5.0f, 0.0f, 0.0f, 1.0f };

void exitingfunction() {
	std::ofstream outfile ("Data/object_file.txt", std::ios_base::trunc);
	outfile << obj->toString() << "\n";
	outfile.close();
};

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
	
	
    std::ifstream ifs ( "Data/object_file.txt" , std::ifstream::in );
	if(ifs.good()) {
		std::string line;
		std::getline(ifs,line,'\n');
		obj = new POGEL::PHYSICS::SOLID(line);
        obj->build();
	}
	else {
		img = new IMAGE("Data/default_2.bmp", IMAGE_LINEAR);
		glass = new IMAGE("Data/Glass.bmp", IMAGE_LINEAR);
		particle = new IMAGE("Data/particle.bmp", IMAGE_LINEAR);
		defaultimg = new IMAGE("Data/default.bmp", IMAGE_LINEAR);
		obj = new POGEL::PHYSICS::SOLID();
		obj->setname("object_thing");
		obj->spin = POGEL::VECTOR(FloatRand(1),FloatRand(1),FloatRand(1));
		addDisk(obj, 32, 1, 3.5, 2.5, particle,10, 1, 0|TRIANGLE_LIT, true, MATRIX(VERTEX(0.0f,0.0f, 0.001f), VERTEX(0.0f,0.0f,0.0f)));
		addDisk(obj, 32, 1, 3.5, 2.5, glass,10, 1, 0|TRIANGLE_LIT, true, MATRIX(VERTEX(0.0f,0.0f,-0.001f), VERTEX(0.0f,180.0f,0.0f)));
		addSphere(obj,16,16, 2, img,2,4, 0|TRIANGLE_VERTEX_NORMALS, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
		addCylinder(obj, 32, 1, 1, 3, 3, defaultimg, 10, 1, 0|TRIANGLE_LIT, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
		addCylinder(obj, 32, 1, 1, 2.99, 2.99, defaultimg, 10, 1, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
		obj->build();
		exitingfunction();
	}
	ifs.close();
	obj->resizetrail(100000);
	
	//sim.addSolid(obj);
	exfnc=exitingfunction;
	//POGEL::addproperty(POGEL_DEBUG);
};

float x = POGEL::FloatRand(2.0)-1.0, y = POGEL::FloatRand(2.0)-1.0, z = POGEL::FloatRand(2.0)-1.0;
bool keypres, go = true;
POGEL::POINT camrot(0,0,0), campos(0,0,-10);

/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	MOUSE_ROT_FUNC
	glTranslatef(campos.x,campos.y,campos.z);
	glRotatef( camrot.x,  1.0f, 0.0f, 0.0f );
	glRotatef( camrot.y,  0.0f, 1.0f, 0.0f );
	glRotatef( camrot.z,  0.0f, 0.0f, 1.0f );
	
	POGEL::IncrementFps();
	POGEL::PrintFps();
	
	//camrot.print();
	//POGEL::message("\n");
	
	//rings.rotate(VECTOR(0.0f,0.6f,0.0f));
	//glRotatef(23.0f,0.0f,0.0f,-1.0f);
	obj->draw();
	obj->step();
	//std::cout << obj->toString() << std::endl;
	//sim.increment();
	//obj.scroll_all_tex_values(0.0004f,0.0005f);
	// since this is double buffered, swap the buffers to display what just got drawn.
	glutSwapBuffers();
}

