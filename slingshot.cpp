#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>              // Header File For The OpenGL32 Library
#include <GL/glu.h>             // Header File For The GLu32 Library
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "scene.h"
#include "window.h"

#include "pogel/pogel.h"
#include "pogel/classes/physics/physics.h"

using namespace POGEL;

#define frameskip 1

#define numobjs 10
#define sps 5.0
#define size 1.0/20
OBJECT obj[numobjs];
POGEL::PHYSICS::SOLID **sphs;
POGEL::PHYSICS::SOLID *border;
POGEL::PHYSICS::SOLID *ball;

POGEL::PHYSICS::SIMULATION sim;

IMAGE *earth;
IMAGE *defaultimg;

#define rndrnge 2.0f

POGEL::POINT camrot, campos;
bool keypres, go = true;

#define th

#ifdef th
THREAD *simulator_runner;
#endif

#ifdef th
unsigned int updts = 0;
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
	}
	pthread_exit(NULL);
};
#endif

float x = POGEL::FloatRand(2)-1, y = POGEL::FloatRand(2)-1, z = POGEL::FloatRand(2)-1;

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 2.0f, 2.0f, 2.0f, 1.0f };
GLfloat LightPosition[]= { 100.0f, 100.0f, 100.0f, 1.0f };

void oob(SOLID_FNC_DEF) {
    //obj->position.z = 0.0;
    //obj->direction.z = 0.0f;
};

unsigned int cnt = 0;

void callb(SOLID_CALLBACK_FNC_DEF) {
	
	cnt++;
	obj->moveto(POGEL::POINT(0,0,0));
	
	float spacev=180.0f/64;
	float spaceh=360.0f/128;
	float a=0, b=0;
	unsigned long cur_vert = 0;
	bool bgr = false;
	for( b = 0.0f; b <= 180.0f - spacev+0.001f; b+=spacev) {
		if(cnt<=cur_vert) {bgr = true; break;}
		for( a = 0.0f; a <= 360.0f - spaceh+0.001f; a+=spaceh) {
			if(cnt<=cur_vert) {bgr = true; break;}
			else cur_vert++;
		}
	}
	obj->direction = POGEL::VECTOR( (sin((a)/180.0f*PI)*sin((b)/180.0f*PI)), (cos((b)/180.0f*PI)), (cos((a)/180.0f*PI)*sin((b)/180.0f*PI)) ).normal()*.075;
	/*camrot.x = b;
	camrot.y = a;*/
	if(!bgr) {
		cnt=0; //obj->setstepstaken(0);
		obj->direction = POGEL::VECTOR(0,1,0).normal()*.075;
		//go = false;
		POGEL::addproperty(POGEL_TRAILS);
	}
	
	//obj->setstepstaken(0);
};

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)              // We call this right after our OpenGL window is created.
{
        glEnable(GL_TEXTURE_2D);                                // Enable Texture Mapping
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // Clear The Background Color To Black 
        glClearDepth(100.0);                                    // Enables Clearing Of The Depth Buffer
        glDepthFunc(GL_LESS);                                   // The Type Of Depth Test To Do
        glEnable(GL_DEPTH_TEST);                                // Enables Depth Testing
        glCullFace(GL_BACK);
		glEnable(GL_CULL_FACE);
        glShadeModel(GL_SMOOTH);                                // Enables Smooth Color Shading
        //glShadeModel(GL_FLAT);                                // Enables flat Color Shading
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glHint(GL_POLYGON_SMOOTH,GL_NICEST);
        glEnable(GL_POLYGON_SMOOTH);
        
        glColor4f(1.0f,1.0f,1.0f,0.5f);			// Full Brightness, 50% Alpha ( NEW )
		glBlendFunc(GL_SRC_ALPHA,GL_ONE);
		glEnable(GL_BLEND);
		glDisable(GL_DEPTH_TEST);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();                               // Reset The Projection Matrix
        
        gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);       // Calculate The Aspect Ratio Of The Window
    
        glMatrixMode(GL_MODELVIEW);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHTING);
        
        //sim = new POGEL::PHYSICS::SIMULATION();
        sim.deactivation = !true;
        sim.precision = 0.0075f;
        //sim.inactive_index = 20;
        
        sim.boundingskips = 0;
        
        srand((unsigned)time(NULL));
        //srand(1234567890);
        x = POGEL::FloatRand(2)-1; y = POGEL::FloatRand(2)-1; z = POGEL::FloatRand(2)-1;
        
        sphs=(POGEL::PHYSICS::SOLID**)malloc(sizeof(POGEL::PHYSICS::SOLID*)*numobjs);
        
        //earth=new IMAGE("Data/earth.bmp");
        defaultimg=new IMAGE("Data/default_2.bmp");
        
        POGEL::addproperty(POGEL_WIREFRAME);
        
        for(int i=0;i<numobjs;i++) {
                obj[i].setname(POGEL::string("sphere%d",i));
                float sz = POGEL::FloatRand(.9)+.1;
                addSphere(&obj[i],16,16, sz, defaultimg,2,4, 0 | TRIANGLE_VERTEX_NORMALS, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
                
                obj[i].setproperties(0/*OBJECT_DEBUG|OBJECT_DRAW_DISPLAYLIST*/);
                obj[i].moveto(POINT(POGEL::FloatRand(sps)-sps/2.0,POGEL::FloatRand(sps)-sps/2.0,POGEL::FloatRand(sps)-sps/2.0)/0.5f * POINT(1.0f,1.0f,1.0f));
                sphs[i] = new POGEL::PHYSICS::SOLID(&obj[i], POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1.0f, 0.75f, 50000000.0f*sz, 1.0f, 1.0f, 1.0f, false, (i%2==0?-1.0f:1.0f)), 1|4|16);
                sphs[i]->turnto(POINT(0.0f,0.0f,0.0f));
                sphs[i]->build();
                
                sphs[i]->setstepstaken(0);
                
                sphs[i]->resizetrail(10);
                
                sphs[i]->setStepFunc(oob);
                
                sphs[i]->visable = true;
                
                sim.addSolid(sphs[i]);
        }
        
        POGEL::OBJECT* ring = new POGEL::OBJECT();
        ring->setname("border");
        addSphere(ring,32,32, 30.0f, defaultimg,1,1, 0 | TRIANGLE_VERTEX_NORMALS|TRIANGLE_INVERT_NORMALS, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
        ring->setproperties(0);
        ring->moveto(POGEL::POINT(0.0f,0.0f,0.0f));
        ring->build();
        border = new POGEL::PHYSICS::SOLID(ring, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(), 1|PHYSICS_SOLID_CONCAVE|16);
        border->behavior.bounce = 1.0f;
        border->behavior.friction = 0.0f;
        border->behavior.mass = 1;
        
        border->behavior.magnetic = false;
        border->behavior.charge = -0.01f;
        border->resizetrail(10);
        border->build();
        sim.addSolid(border);
        
        border->visable = true;
        
        
        POGEL::OBJECT* sp = new POGEL::OBJECT();
        sp->setname("ball");
        addSphere(sp,2,4, size/2, defaultimg,1,1, 0 | TRIANGLE_LIT, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
        //addCube(sp, 10,6,4, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX());
        sp->setproperties(0|OBJECT_ROTATE_TOCAMERA);
        sp->moveto(POGEL::POINT(0.0f,0.0f,0.0f));
        sp->build();
        ball = new POGEL::PHYSICS::SOLID(sp, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1, 1, 1, 1, 1, 1, false, 0), 2|PHYSICS_SOLID_CONVEX|16);
        ball->behavior.bounce = 1.0f;
        ball->behavior.friction = 1.0f;
        ball->behavior.mass = 5000.0;
        ball->resizetrail(5000);
        
        ball->moveto(POGEL::POINT(0,0,0));
		ball->direction = POGEL::VECTOR(0,1,0).normal()*.075;
        
        ball->setCallback(callb);
        
        sim.addSolid(ball);
        
        ball->visable = true;
        
        sim.FORCEfastAccessList();
		#ifdef th
		simulator_runner = new THREAD(sim_runner);
		//simulation.setThreadsNum(1);
		simulator_runner->startThread();
		#endif
		
        POGEL::InitFps();
}

//unsigned long frames=0;

//camrot = POGEL::POINT(0,0,0);//, campos;
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
		
        glTranslatef(0.0f+campos.x,0.0f+campos.y,-20.0f+campos.z);
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
                
        if(frames%frameskip == 0) {
                sim.draw();
                //unsigned int op = POGEL::getproperties();
                //POGEL::addproperty(POGEL_BOUNDING);
                //border->bounding.draw(POGEL::POINT());
                //POGEL::setproperties(op);
        }
        #ifndef th
        if(keypres) {
        				//if(POGEL::GetTimePassed() < 60.0f)
                        sim.increment();
                        keypres = false;
                }
                else if(go) {
                //if(POGEL::GetTimePassed() < 60.0f)
                        sim.increment();
                }
		#endif
        border->bounding.draw(POGEL::POINT());
        // since this is double buffered, swap the buffers to display what just got drawn.
        if(frames%frameskip == 0) {
        glutSwapBuffers();
        }
        
        //printf("POGEL::getproperties() = %u\n", POGEL::getproperties());
}
