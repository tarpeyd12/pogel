#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>		// Header File For The OpenGL32 Library
#include <GL/glu.h>		// Header File For The GLu32 Library
#include <stdlib.h>     // Header file for malloc/free.
#include <unistd.h>     // needed to sleep
#include <stdio.h>

#include "window.h"
#include "pogel/pogel.h"

int window;

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
void ReSizeGLScene(int Width, int Height)
{
	if(Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
		Height=1;

	glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
}

extern POGEL::POINT camrot, campos;
extern bool keypres, go;
/* The function called whenever a key is pressed. */
void keyPressed(unsigned char key, int x, int y) 
{
	/* avoid thrashing this procedure */
	usleep(100);
	
	/* If escape is pressed, kill everything. */
	if (key == ESCAPE)
	{ 
		/* shut down our window */
		glutDestroyWindow(window); 
		
		/* exit the program...normal termination. */
		exit(0);
	}
	
	if(key=='\b')
		glutFullScreen();
	
	if(key == 'w')
		camrot.x+=1.0f;
	if(key == 's')
		camrot.x-=1.0f;
	if(key == 'a')
		camrot.y+=1.0f;
	if(key == 'd')
		camrot.y-=1.0f;
	if(key == 'q')
		camrot.z+=1.0f;
	if(key == 'e')
		camrot.z-=1.0f;
	
	if(key == 'W')
		campos.x+=1.0f;
	if(key == 'S')
		campos.x-=1.0f;
	if(key == 'A')
		campos.y+=1.0f;
	if(key == 'D')
		campos.y-=1.0f;
	if(key == 'Q')
		campos.z+=1.0f;
	if(key == 'E')
		campos.z-=1.0f;
	
	if(key == ' ')
		keypres = true;
	else keypres = false;
	
	if(key == '\r')
		go = !go;
	
	
	if(key == '0') {
		if(POGEL::hasproperty(POGEL_OOPS))
			POGEL::setproperties(0);
		else
			POGEL::addproperty(POGEL_OOPS);
	}
	
	if(key == '1') {
		if(POGEL::hasproperty(2))
			POGEL::removeproperty(2);
		else
			POGEL::addproperty(2);
	}
	
	if(key == '2') {
		if(POGEL::hasproperty(4))
			POGEL::removeproperty(4);
		else
			POGEL::addproperty(4);
	}
	
	if(key == '3') {
		if(POGEL::hasproperty(8))
			POGEL::removeproperty(8);
		else
			POGEL::addproperty(8);
	}
	
	if(key == '4') {
		if(POGEL::hasproperty(16))
			POGEL::removeproperty(16);
		else
			POGEL::addproperty(16);
	}
	
	if(key == '5') {
		if(POGEL::hasproperty(32))
			POGEL::removeproperty(32);
		else
			POGEL::addproperty(32);
	}
	
	if(key == '6') {
		if(POGEL::hasproperty(64))
			POGEL::removeproperty(64);
		else
			POGEL::addproperty(64);
	}
	
	if(key == '7') {
		if(POGEL::hasproperty(128))
			POGEL::removeproperty(128);
		else
			POGEL::addproperty(128);
	}
}
