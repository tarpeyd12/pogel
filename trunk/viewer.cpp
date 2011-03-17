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

HASHLIST<PHYSICS::SOLID*> objs;

std::string file;

bool sterio = false;
bool rendandquit = false;
bool sametri = false;

std::string rendfile = "viewer_out.bmp";

POGEL::POINT camrot, campos;

VIEWPORT viewport[2];

VIEW *view;

GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightDiffuse[]= { 5.0f, 5.0f, 5.0f, 5.0f };
GLfloat LightPosition[]= { 5.0f, 0.0f, 0.0f, 1.0f };

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
	bool fincmd = false;
	for(int i = 0; i < numcmdargs; i++) {
		if(strlen(cmdargs[i]) == 2 && !strncmp(cmdargs[i], "-f", 2)) {
			file = cmdargs[++i];
			fincmd = true; continue;
		} else
		
		if(strlen(cmdargs[i]) == 7 && !strncmp(cmdargs[i], "-stereo", 7)) {
			sterio = true; continue;
		} else
		
		if(strlen(cmdargs[i]) == 2 && !strncmp(cmdargs[i], "-o", 2)) {
			rendfile = cmdargs[++i];
			rendandquit = true; continue;
		} else
		
		if(strlen(cmdargs[i]) == 7 && !strncmp(cmdargs[i], "-campos", 7)) {
			campos = POGEL::POINT(cmdargs[++i]);
		} else
		
		if(strlen(cmdargs[i]) == 7 && !strncmp(cmdargs[i], "-camrot", 7)) {
			camrot = POGEL::POINT(cmdargs[++i]);
		} else
		
		if(strlen(cmdargs[i]) == 8 && !strncmp(cmdargs[i], "-sametri", 8)) {
			sametri = true;
		} else
		
		{ continue; }
	}
	if(!fincmd) {
		cout << "enter the file to load: "; cin >> file;
	}
	
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
	
	glColor4f(1.0f,1.0f,1.0f,0.5f);			// Full Brightness, 50% Alpha ( NEW )
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();				// Reset The Projection Matrix
	
	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);	// Calculate The Aspect Ratio Of The Window
    
	glMatrixMode(GL_MODELVIEW);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	
	std::ifstream ifs ( file.c_str(), std::ifstream::in );
	std::string line;
	POGEL::PHYSICS::SOLID* fst = NULL;
	unsigned int i = 0;
	if(ifs.good())
	while(ifs.good()) {
		line.clear();
		std::getline(ifs,line,'\n');
		if(!ifs.good() || !line.length()) break;
		PHYSICS::SOLID *s = new POGEL::PHYSICS::SOLID(line);
		if(!sametri && POGEL::hasproperty(POGEL_LABEL)) {
			s->cleartriangles();
			s->visable = false;
		}
		else if(sametri && fst != NULL) {
			s->cleartriangles();
			s->referencetriangles(fst);
			s->visable = true;
		}
		if(i < 1) fst = s;
		objs += s;
		s->build();
		printf("\t%u\r",objs.length());
		i++;
	}
	else
		exit(0);
	
	viewport[0].setretscreensize(&screenx, &screeny);
	viewport[0].setviewport(0,0,Width/2,Height);
	viewport[0].setbgcolor(POGEL::COLOR(0.0f,0.0f,0.0f,0.5f));
	viewport[0].setfilter(IMAGE_MIPMAP);
	viewport[0].build();
	
	viewport[1].setretscreensize(&screenx, &screeny);
	viewport[1].setviewport(Width/2,0,Width,Height);
	viewport[1].setbgcolor(POGEL::COLOR(0.0f,0.0f,0.0f,0.5f));
	viewport[1].setfilter(IMAGE_MIPMAP);
	viewport[1].build();
}
bool keypres, go = true;
POGEL::POINT camrotvect;
float diff = -0.85f;
/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	
	POGEL::IncrementFps();
	POGEL::PrintFps();
	
	MOUSE_ROT_FUNC
	
	if(keys[',']) {
		if(keys['x']) camrotvect.x += 0.05;
		if(keys['y']) camrotvect.y += 0.05;
		if(keys['z']) camrotvect.z += 0.05;
	}
	if(keys['.']) {
		if(keys['x']) camrotvect.x -= 0.05;
		if(keys['y']) camrotvect.y -= 0.05;
		if(keys['z']) camrotvect.z -= 0.05;
	}
	
	camrot += camrotvect;
	
	if(keys['p']) sterio = !sterio;
	
	if(sterio) {
		viewport[0].setviewport(0,0,screenx/2,screeny);
		viewport[1].setviewport(screenx/2,0,screenx,screeny);
		if(keys['c']) diff+=0.005;
		if(keys['v']) diff-=0.005;
		for(int v = 0; v < 2; v++) {
			viewport[v].startrender();	
				glPushMatrix();
				glTranslatef(campos.x,campos.y,campos.z);
				glRotatef( camrot.x,  1.0f, 0.0f, 0.0f );
				glRotatef( camrot.y + (v==0 ? diff : -diff),  0.0f, 1.0f, 0.0f );
				glRotatef( camrot.z,  0.0f, 0.0f, 1.0f );
				for(unsigned int i = 0; i < objs.length(); i++)
					objs[i]->draw();
				glPopMatrix();
			viewport[v].endrender();
		}
	}
	else {
		viewport[0].resetscreen();
		glTranslatef(campos.x,campos.y,campos.z);
		glRotatef( camrot.x,  1.0f, 0.0f, 0.0f );
		glRotatef( camrot.y,  0.0f, 1.0f, 0.0f );
		glRotatef( camrot.z,  0.0f, 0.0f, 1.0f );
		for(unsigned int i = 0; i < objs.length(); i++)
			objs[i]->draw();
	}
	
	if(keys['r'] || (rendandquit && POGEL::frames > 1)) {
		view = new VIEW();
		view->setretscreensize(&screenx, &screeny);
		std::string fmt = rendfile.substr(rendfile.find_last_of('.'));
		if(fmt.compare(".bmp") || fmt.compare("bmp")) view->save(VIEW_SAVE_BMP,rendfile.c_str());
		else if(fmt.compare(".tga") || fmt.compare("tga")) view->save(VIEW_SAVE_TGA,rendfile.c_str());
		else view->save(VIEW_SAVE_BMP,(rendfile + ".bmp").c_str());
		if(rendandquit && POGEL::frames > 1) quit();
	}
	
	//obj.scroll_all_tex_values(0.0004f,0.0005f);
	// since this is double buffered, swap the buffers to display what just got drawn.
	glutSwapBuffers();
}

