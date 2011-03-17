#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>		// Header File For The OpenGL32 Library
#include <GL/glu.h>		// Header File For The GLu32 Library
#include <stdlib.h>     // Header file for malloc/free.
#include <unistd.h>     // needed to sleep
#include <stdio.h>

#include "window.h"
#include "pogel/pogel.h"

int window;
int screenx, screeny;

bool keys[256];

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
	
	screenx = Width;
	screeny = Height;
}

void quit() {
	keyPressed(ESCAPE,0,0);
};

void (*exfnc)(void);

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
		if(exfnc != NULL) exfnc();
		
		/* shut down our window */
		glutDestroyWindow(window); 
		
		/* exit the program...normal termination. */
		exit(0);
	}
	
	keys[key] = true;
	
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
		campos.x+=1.0f/8;
	if(key == 'S')
		campos.x-=1.0f/8;
	if(key == 'A')
		campos.y+=1.0f/8;
	if(key == 'D')
		campos.y-=1.0f/8;
	if(key == 'Q')
		campos.z+=1.0f/8;
	if(key == 'E')
		campos.z-=1.0f/8;
	
	if(key == ' ')
		keypres = true;
	//else keypres = false;
	
	if(key == '\r') go = !go;
	
	
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
	
	if(key == '8') {
		if(POGEL::hasproperty(256))
			POGEL::removeproperty(256);
		else
			POGEL::addproperty(256);
	}
	
	/*if(key == '9') {
		if(POGEL::hasproperty(POGEL_TIMEBASIS))
			POGEL::removeproperty(POGEL_TIMEBASIS);
		else
			POGEL::addproperty(POGEL_TIMEBASIS);
	}*/
}

void keyUnpressed(unsigned char key, int x, int y) 
{
	keys[key] = false;
}

int mouse_x;
int mouse_y;
int mouse_static_x;
int mouse_static_y;
int mouse_button;
int mouse_state = 1;

int prevmousex;
int prevmousey;

POGEL::POINT prot;

void mouseClicked(int button, int state, int x, int y)
{
	if(state == GLUT_UP) {
		prevmousex = x;
		prevmousey = y;
	}
	if(state == GLUT_DOWN) {
		prot = camrot;
		mouse_static_x = mouse_x = x;
		mouse_static_y = mouse_y = y;
	}
	mouse_button = button;
	mouse_state = state;
};

void mouseMoved(int x, int y)
{
	//prevmousex = mouse_x;
	//prevmousey = mouse_y;
	mouse_x = x;
	mouse_y = y;
};

