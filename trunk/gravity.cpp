#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>              // Header File For The OpenGL32 Library
#include <GL/glu.h>             // Header File For The GLu32 Library
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <fstream>

#include "scene.h"
#include "window.h"

#include "pogel/pogel.h"
#include "pogel/classes/physics/physics.h"

using namespace POGEL;

#define th

#ifdef th
#include "threads.h"

THREAD *simulator_runner;
#endif

#define frameskip 1
unsigned int objnum;
#define grd objnum
#define numobjs (grd*grd*grd)

#define svfrq 1000/numobjs

#define sps 1.0f/2
#define size 1.0f/20
OBJECT *obj;
POGEL::PHYSICS::SOLID **sphs;

POGEL::PHYSICS::SIMULATION sim;

IMAGE *particle;


#define rndrnge 2.0f

float x = POGEL::FloatRand(2)-1, y = POGEL::FloatRand(2)-1, z = POGEL::FloatRand(2)-1;

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 2.0f, 2.0f, 2.0f, 1.0f };
GLfloat LightPosition[]= { 100.0f, 100.0f, 100.0f, 1.0f };

bool dimlock = !true;

bool keypres, go = true;

unsigned long int updts = 0;

bool sc = false;
#ifdef th
bool ef = false;
void* sim_runner(void* arg) {
	for(;;) {
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
		if(ef) break;
	}
	pthread_exit(NULL);
};

void exitingfunction() {
	ef = true;
	simulator_runner->joinThread();
	char *flnm = POGEL::string("log%d.txt",numobjs);
	std::ofstream outfile (flnm, std::ios_base::trunc);
	free(flnm);
	for(unsigned long a=0;a<numobjs;a++) {
		//printf("%s\n",sphs[a]->toString().c_str());
		outfile << sphs[a]->toString() << "\n";
	}
	outfile.close();
};

#endif

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)              // We call this right after our OpenGL window is created.
{
	std::cout << "number of objects to have(will be cubed): "; std::cin >> objnum;
        glEnable(GL_TEXTURE_2D);                                // Enable Texture Mapping
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // Clear The Background Color To Black 
        glClearDepth(500.0);                                    // Enables Clearing Of The Depth Buffer
        glDepthFunc(GL_LESS);                                   // The Type Of Depth Test To Do
        glEnable(GL_DEPTH_TEST);                                // Enable Depth Testing
        glShadeModel(GL_SMOOTH);                                // Enables Smooth Color Shading
        //glShadeModel(GL_FLAT);                                // Enables flat Color Shading
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glHint(GL_POLYGON_SMOOTH,GL_NICEST);
        glEnable(GL_POLYGON_SMOOTH);
        
        glColor4f(1.0f,1.0f,1.0f,0.5f);			// Full Brightness, 50% Alpha ( NEW )
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	
	//glShadeModel(GL_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);					// Set Line Antialiasing
	glEnable(GL_BLEND);							// Enable Blending
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glDisable(GL_DEPTH_TEST);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();                               // Reset The Projection Matrix
        
        gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,500.0f);       // Calculate The Aspect Ratio Of The Window
    	
        glMatrixMode(GL_MODELVIEW);
        /*glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
        glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);*/
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHTING);
        sim.boundingskips = 0;
        
        srand((unsigned)time(NULL));
        x = POGEL::FloatRand(2)-1; y = POGEL::FloatRand(2)-1; z = POGEL::FloatRand(2)-1;
        
        sphs=(POGEL::PHYSICS::SOLID**)malloc(sizeof(POGEL::PHYSICS::SOLID*)*numobjs);
        obj = new POGEL::OBJECT[numobjs];
        particle=new IMAGE("Data/particle.bmp", IMAGE_LINEAR);
        for(unsigned int i=0;i<numobjs;i++) {
                
                obj[i].setname(POGEL::string("sphere%d",i));
                if(i == 0) {
                addDisk(&obj[0], 6, 1, size/2.0f, 0.0f, particle,1, 1, 0|TRIANGLE_COLORED, false, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
                }
                else obj[i].copytriangles(&obj[0]);
                obj[i].setproperties(OBJECT_ROTATE_TOCAMERA|OBJECT_DRAW_CHILDREN/*|OBJECT_DEBUG|OBJECT_DRAW_DISPLAYLIST*/);
                obj[i].moveto(POINT(
						((float)(i%grd)*sps)-( (float(grd)*sps)/2.0f-sps/2.0f),
						((float)((i/grd)%grd)*sps)-( (float(grd)*sps)/2.0f-sps/2.0f),
						(float)(i/(grd*grd))*(sps) - sps*float(grd)/2.0f + sps/2.0f /* - (10.0f-(sps/2.0f)), \*/
                ));
                if(i==0)
                for(unsigned int a = 0; a < obj[i].getnumfaces(); a++) {
                	POGEL::TRIANGLE t = obj[i].gettriangle(a);
                	//t.addproperty(TRIANGLE_COLORED|TRIANGLE_LIT);
                	for(int b = 0; b < 3; b++) {
                		t.vertex[b].color = POGEL::COLOR(.2,.5,1,1);
                	}
                	obj[i].settriangle(a, t);
                }
                
                sphs[i] = new POGEL::PHYSICS::SOLID(&obj[i], POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1.0f, 0.75f, 5000.0f, 1.0f, 1.0f, 1.0f, false, (i%2==0?-1.0f:1.0f)), 2|4|(i%2==0 && false ? 0 : 16));
                sphs[i]->turnto(POINT(0.0f,0.0f,0.0f));
                //sphs[i]->build();
                sphs[i]->setstepstaken(0);
                
                sphs[i]->visable = true;
                
                sim.addSolid(sphs[i]);
        }
        
        char *flnm = POGEL::string("log%d.txt",numobjs);
        std::ifstream ifs ( flnm , std::ifstream::in );
		free(flnm);
		if(ifs.good()) {
			
			std::string line;
			std::string pos_str, dir_str;
			for(unsigned int i=0;i<numobjs;i++) {
				if(!ifs.good())break;
				line.clear();
				std::getline(ifs,line,'\n');
		    	sphs[i]->position = POGEL::POINT(POGEL::getStringComponentLevel('{','}',line,"0 0"));
		    	sphs[i]->direction = POGEL::VECTOR(POGEL::getStringComponentLevel('{','}',line,"0 2"));
		    	//std::cout << "pos:  " + sphs[i]->position.toString() + ", dir:  " + sphs[i]->direction.toString() + "\n";
		    	sphs[i]->build();
	    	}
  		}
        
        ifs.close();
        
        
        POGEL::InitFps();
        printf("\n");
        
       	#ifdef th
       	simulator_runner = new THREAD(sim_runner);
       	simulator_runner->startThread();
       	exfnc = exitingfunction;
       	#endif
}

//unsigned long frames=0;

POGEL::POINT camrot(0,0,0), campos;
bool p = false;
/* The main drawing function. */
void DrawGLScene()
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);             // Clear The Screen And The Depth Buffer
        glLoadIdentity();
		
		MOUSE_ROT_FUNC
		
		glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
        glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHTING);
		
        glTranslatef(0.0f+campos.x,0.0f+campos.y,-12.5f+campos.z);
        //glRotatef( 90.0f,  1.0f, 0.0f, 0.0f );
        glRotatef( camrot.x + ((float)frames)*0.0f,  1.0f, 0.0f, 0.0f );
        glRotatef( camrot.y + ((float)frames)*0.0f,  0.0f, 1.0f, 0.0f );        
        glRotatef( camrot.z + ((float)frames)*0.0f,  0.0f, 0.0f, 1.0f );
        //glRotatef( 90.0f,  0.0f, 1.0f, 0.0f );
        //message("%ld: ",frames);
        //if(POGEL::frames >= 1000) go = false;
        
        POGEL::IncrementFps();
        if(frames%frameskip == 0)
        POGEL::PrintFps();
        
        if(frames%frameskip == 0)
                sim.draw();
        #ifndef th
        if(keypres) {
        				//if(POGEL::GetTimePassed() < 60.0f)
                        sim.increment();
                        keypres = false;
                }//sc = true;
			//printf("updates = %ld\n", updts++);
                else if(go) {
                //if(POGEL::GetTimePassed() < 60.0f)
                       sim.increment();
                }
        if(keys[';'] || frames%svfrq == 0)
		{
			char *flnm = POGEL::string("log%d.txt",numobjs);
			std::ofstream outfile (flnm, std::ios_base::trunc);
			free(flnm);
			for(unsigned long a=0;a<numobjs;a++) {
				//printf("%s\n",sphs[a]->toString().c_str());
				outfile << sphs[a]->toString() << "\n";
			}
			outfile.close();
		}
        #endif
        
        // since this is double buffered, swap the buffers to display what just got drawn.
        if(frames%frameskip == 0) {
        glutSwapBuffers();
        }
        
        //printf("POGEL::getproperties() = %u\n", POGEL::getproperties());
}
