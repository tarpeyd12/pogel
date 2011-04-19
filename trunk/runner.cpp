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

#define frameskip 1

using namespace POGEL;

HASHLIST<PHYSICS::SOLID*> objs;

POGEL::PHYSICS::SIMULATION sim;

std::string file;

//#define th

#ifdef th
THREAD *simulator_runner;
#endif

bool sametri = false;

POGEL::POINT camrot, campos;

GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightDiffuse[]= { 5.0f, 5.0f, 5.0f, 5.0f };
GLfloat LightPosition[]= { 5.0f, 0.0f, 0.0f, 1.0f };

bool keypres, go = true;
POGEL::POINT camrotvect;

#ifdef th
unsigned int updts = 0;
void* sim_runner(void* arg) {
	for(;;) {
		POGEL::message("Step Cycle ...\n");
		if(keypres) {
			keypres = false;
			sim.increment();
			updts++;
		}
		else if(go) {
			sim.increment();
			updts++;
		}
		if(POGEL::hasproperty(POGEL_TIMEBASIS)) POGEL::removeproperty(POGEL_TIMEBASIS);
	}
	pthread_exit(NULL);
};
#endif

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)	        // We call this right after our OpenGL window is created.
{
	bool fincmd = false, rndcmd = false, dimlock = false;
	unsigned int rndnum = 100;
	unsigned int rndtrsz = 64;
	float rndrnge = .25;
	float ballsize = .01;
	for(int i = 0; i < numcmdargs; i++) {
		if(strlen(cmdargs[i]) == 2 && !strncmp(cmdargs[i], "-f", 2)) {
			file = cmdargs[++i];
			fincmd = true; continue;
		} else
		
		if(strlen(cmdargs[i]) == 2 && !strncmp(cmdargs[i], "-r", 2)) {
			if(numcmdargs > i+1)if(cmdargs[i+1][0] >= '0' && cmdargs[i+1][0] <= '9') {
				sscanf(cmdargs[++i], "%u", &rndnum);
			if(numcmdargs > i+1)if((cmdargs[i+1][0] >= '0' && cmdargs[i+1][0] <= '9') || cmdargs[i+1][0] == '.') {
				sscanf(cmdargs[++i], "%f", &rndrnge);
			if(numcmdargs > i+1)if((cmdargs[i+1][0] >= '0' && cmdargs[i+1][0] <= '9') || cmdargs[i+1][0] == '.') {
				sscanf(cmdargs[++i], "%f", &ballsize);
			if(numcmdargs > i+1)if(cmdargs[i+1][0] >= '0' && cmdargs[i+1][0] <= '9')
				sscanf(cmdargs[++i], "%u", &rndtrsz);
			}}}
			sametri = true;
			rndcmd = true; continue;
		} else
		
		if(strlen(cmdargs[i]) == 3 && !strncmp(cmdargs[i], "-2d", 3)) {
			dimlock = true;
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
	if(!fincmd && !rndcmd) {
		//cout << "enter the file to load: "; cin >> file;
		file = "./kd4.sav";
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
	
	gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.0001f,100.0f);	// Calculate The Aspect Ratio Of The Window
    
	glMatrixMode(GL_MODELVIEW);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHTING);
	
	srand((unsigned)time(NULL));
	
	if(fincmd && !rndcmd) {
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
				s->visable = !true;
			}
			if(i < 1) fst = s;
			objs += s;
			s->visable = false;
			s->build();
			printf("\t%u\r",objs.length());
			i++;
		}
		else
			exit(0);
	}
	else if(rndcmd) {
		POGEL::PHYSICS::SOLID* tmp;
		printf("\n");
		while(objs.length() < rndnum) {
			printf("%u\r",objs.length());
			char* ms;
			ms = POGEL::string("%f", 100 + POGEL::FloatRand(100)-50);
			tmp = new POGEL::PHYSICS::SOLID(POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES("{[1],[0],["+std::string(ms)+"],[1],[1],[1],[0],[0]}"), 2|4|16);
			free(ms);
			if(!objs.length())
				addSphere(tmp, 2,4, ballsize, POGEL::requestImage("{IMAGE_NULL}"), 1,1, TRIANGLE_LIT);
			else
				tmp->referencetriangles(objs[0]);
			tmp->resizetrail(rndtrsz);
			tmp->addproperty(OBJECT_ROTATE_TOCAMERA);
			tmp->build();
			tmp->visable = false;
			POGEL::MATRIX m(POGEL::POINT(),POGEL::POINT(0,0,POGEL::FloatRand(360)));
			tmp->position = m.transformPoint(POGEL::POINT(0,POGEL::FloatRand(rndrnge),POGEL::FloatRand(rndrnge)-rndrnge/2));
			//tmp->position = POGEL::POINT(POGEL::FloatRand(rndrnge)-rndrnge/2,POGEL::FloatRand(rndrnge)-rndrnge/2,POGEL::FloatRand(rndrnge)-rndrnge/2);
			float d = tmp->position.distance(POGEL::POINT(0,0,0));
			tmp->direction = m.transformVector(POGEL::VECTOR(1,0,0)).normal() * 
				(float)sqrt( ( ( (rndnum*100)*tmp->behavior.mass )*(GRAVITYCONSTANT/1000) )/d );
			if(dimlock)
				tmp->position.z = 0;
			objs += tmp;
		}
		/*objs[0]->position = POGEL::POINT(1,1,0)*2000;
		objs[0]->direction = POGEL::VECTOR(1,1,0)*0;
		objs[1]->position = POGEL::POINT(1,1,0)*-2000;
		objs[1]->direction = POGEL::VECTOR(1,1,0)*-0;*/
		//sim.addsingularity(POGEL::PHYSICS::SINGULARITY(POGEL::POINT(), rndnum*100));
	}
	
	sim.FORCEfastAccessList();
	//sim.setThreadsNum(10);
	
	for(unsigned int i = 0; i < objs.length(); i++)
		sim.addSolid(objs[i]);
	//POGEL::removeproperty(POGEL_BOUNDING);
	
	//sim.FORCEfastAccessList();
	//sim.setThreadsNum(4);
	#ifdef th
	simulator_runner = new THREAD(sim_runner);
	//simulation.setThreadsNum(1);
	simulator_runner->startThread();
	#endif
	
	POGEL::InitFps();
}

/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	POGEL::IncrementFps();
	if(frames%frameskip == 0)
	POGEL::PrintFps();
	POGEL::message("\n");
	
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
	
	glTranslatef(campos.x,campos.y,campos.z);
	glRotatef( camrot.x,  1.0f, 0.0f, 0.0f );
	glRotatef( camrot.y,  0.0f, 1.0f, 0.0f );
	glRotatef( camrot.z,  0.0f, 0.0f, 1.0f );
	//POGEL::removeproperty(POGEL_BOUNDING);
	//unsigned int p = POGEL::getproperties();
	//POGEL::removeproperty(POGEL_BOUNDING);
	POGEL::message("Draw Cycle ...\n");
	if(frames%frameskip == 0)
	sim.draw();
	//POGEL::setproperties(p);
	//POGEL::removeproperty(POGEL_BOUNDING);
	#ifndef th
	if(go || keypres) {
		POGEL::message("Step Cycle ...\n");
		sim.increment();
		if(keys['m'])
	       	sim.drawGravityGrid(1000, .075, POGEL::POINT(0,0,0), 8);
	}
	#endif
	
	if(keypres) keypres = false;
	
	//bkdtree.getClosest(POGEL::POINT())->position.draw();
	//POGEL::POINT().draw();
	
	//obj.scroll_all_tex_values(0.0004f,0.0005f);
	// since this is double buffered, swap the buffers to display what just got drawn.
	if(frames%frameskip == 0)
	glutSwapBuffers();
}

