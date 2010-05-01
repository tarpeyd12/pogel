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

extern POGEL::POINT camrot;

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
}

