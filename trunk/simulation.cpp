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

//#define th

#ifdef th
//#include "threads.h"

THREAD *simulator_runner;
#endif

#define sheerradius 10
#define emitfrq 10

#define frameskip 1
unsigned int objnum;
#define grd 5//objnum
#define numobjs (grd*grd)

#define svfrq 1000/numobjs

#define sps 1.0f/10
#define size 1.0f/10

POGEL::POINT sheerrot;
POGEL::VECTOR sheerspin;

OBJECT *obj;
OBJECT patern;
POGEL::PHYSICS::SOLID **sphs;
POGEL::PHYSICS::SOLID *border;
POGEL::PHYSICS::SOLID *ball;
POGEL::PHYSICS::SOLID *box;

POGEL::PHYSICS::DYNAMICS sim;
POGEL::PHYSICS::DYNAMICS sim2;

IMAGE *particle;
IMAGE *defaultimg;

POGEL::VIEW v;

#define rndrnge 2.0f

float x = 0, y = 0, z = 0;

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 2.0f, 2.0f, 2.0f, 1.0f };
GLfloat LightPosition[]= { 100.0f, 100.0f, 100.0f, 1.0f };

bool dimlock = true;

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
			sim2.increment();
			updts++;
			sheerrot += sheerspin;
		}
		else if(go) {
			sim.increment();
			sim2.increment();
			updts++;
			sheerrot += sheerspin;
		}
		if(POGEL::hasproperty(POGEL_TIMEBASIS)) POGEL::removeproperty(POGEL_TIMEBASIS);
		if(ef) break;
		/*if(keys['c']) {
			keys['c'] = false;
			sim.deactivation = !sim.deactivation;
		}
		if(sim.deactivation)
			std::cout << " deactivation enabled" << std::endl;
		else
			std::cout << " deactivation disabled" << std::endl;*/
	}
	pthread_exit(NULL);
};

#endif

void oob(SOLID_FNC_DEF) {
        
        if(keys['l'])
        	obj->spin=POGEL::VECTOR(1,1,1)/0.010f * VECTOR(0,0,1);
        if(keys['L']) {
        	obj->spin = POGEL::VECTOR();
        	obj->rotation = POGEL::POINT();
        }
        if(obj->getstepstaken() == (numobjs)*emitfrq) {
        	obj->position = POGEL::POINT(sheerradius*3,0,0);
        	obj->direction = POGEL::VECTOR(-1,0,0).normal()*0.2;
        	obj->setstepstaken(0);
        }
       	if(dimlock) {
        obj->position.z = 0.0;
        obj->direction.z = 0.0f;
        }
        
        /*for(unsigned int a = 0; a < obj->getnumfaces(); a++) {
           	POGEL::TRIANGLE t = obj->gettriangle(a);
           	for(int b = 0; b < 3; b++) t.vertex[b].color = obj->getLabelColor();
           	obj->settriangle(a, t);
		}*/
		
		if(obj->getcontainer() == &sim)
		if(obj->position.distance(POGEL::POINT(x,y,z)) >= sheerradius) {
			//printf("removing object: %p\n", obj);
			sim.removeSolidKeepGravity(obj);
			POGEL::MATRIX m(POGEL::POINT(),sheerrot*-1); m.invert(); m.transformPoint(&obj->position); m.transformVector(&obj->direction);
			sim2.addSolidHoldGravity(obj);
			obj->setstepstaken(0);
		}
		
		if(obj->getcontainer() == &sim2)
		if(obj->position.distance(POGEL::POINT(x,y,z)) < sheerradius) {
			//printf("removing object: %p\n", obj);
			sim2.removeSolidKeepGravity(obj);
			//POGEL::MATRIX m(POGEL::POINT(),POGEL::POINT(1,1,1)*360-sheerrot); m.transformPoint(&obj->position); m.transformVector(&obj->direction);
			sim.addSolidHoldGravity(obj);
			obj->setstepstaken(0);
		}
		
		/*if(obj->getstepstaken() == (numobjs)*emitfrq) {
        	obj->position = POGEL::POINT(sheerradius*3,0,0);
        	obj->direction = POGEL::VECTOR(-1,0,0).normal()*0.2;
        	obj->setstepstaken(0);
        }*/
};

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)              // We call this right after our OpenGL window is created.
{
	//std::cout << "number of objects to have(will be cubed): "; std::cin >> objnum;
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
        
        gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.01f,500.0f);       // Calculate The Aspect Ratio Of The Window
    	
        glMatrixMode(GL_MODELVIEW);
        /*glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
        glLightfv(GL_LIGHT1, GL_POSITIO(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0))N,LightPosition);*/
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHTING);
        
        //POGEL::addproperty(POGEL_TIMEBASIS);
        //sim = new POGEL::PHYSICS::SIMULATION();
        /*sim.deactivation = false;
        sim.precision = 0.00075f;
        sim.inactive_index = 250;*/
        
        sim.boundingskips = 0;
        
        srand((unsigned)time(NULL));
        //x = POGEL::FloatRand(2)-1; y = POGEL::FloatRand(2)-1; z = POGEL::FloatRand(2)-1;
        //sheerspin = POGEL::VECTOR(1,1,1)*POGEL::POINT(1,1,1)*1;
        sheerspin = POGEL::VECTOR(POGEL::FloatRand(2)-1,POGEL::FloatRand(2)-1,POGEL::FloatRand(2)-1)*POGEL::POINT(0,0,1)*1;
        sheerrot = POGEL::POINT(0,0,0);
        sphs = new POGEL::PHYSICS::SOLID*[numobjs];
        obj = new POGEL::OBJECT[numobjs];
        //earth=new IMAGE("Data/earth.bmp");
        particle=new IMAGE("Data/particle.bmp", IMAGE_LINEAR);
        defaultimg=new IMAGE("Data/default_2.bmp", IMAGE_LINEAR);
        //POGEL::removeproperty(POGEL_WIREFRAME);
        
        //POGEL::MATRIX m(POGEL::POINT(), POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0)));
   		addDisk(&patern, 6, 1, size/2.0f, 0.0f, particle,1, 1, 0|TRIANGLE_COLORED, false, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
   		for(unsigned int a = 0; a < patern.getnumfaces(); a++) {
           	POGEL::TRIANGLE t = patern.gettriangle(a);
           	//t.addproperty(TRIANGLE_COLORED|TRIANGLE_LIT);
           	for(int b = 0; b < 3; b++)
           		t.vertex[b].color = POGEL::COLOR(.2,.5,1,1);
          	patern.settriangle(a, t);
        }
        //printf("obj = %p\n",&obj);
        for(unsigned int i=0;i<numobjs;i++) {
        		if(i%100==0)printf("%u of %u\r",i,numobjs);
        		POGEL::POINT pos(
						((float)(i%grd)*sps)-( (float(grd)*sps)/2.0f-sps/2.0f),
						((float)((i/grd)%grd)*sps)-( (float(grd)*sps)/2.0f-sps/2.0f),
						(float)(i/(grd*grd))*(sps) - sps*float(grd)/2.0f + sps/2.0f /* - (10.0f-(sps/2.0f)), \*/
                );
                //pos.z=0;
                //pos+=POGEL::POINT(0,1,0);
                //m = m * POGEL::MATRIX(POGEL::POINT(), POGEL::POINT(x,y,z));
                //POGEL::MATRIX m(POGEL::POINT(), POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0)));
                
                //obj[i].setname(POGEL::string("sphere%d",i));
                //if(i%2!=0)
                
                //if(pos.distance(POGEL::POINT(x,y,z)) >= 1.0f || pos.distance(POGEL::POINT(x,y,z)) <= 0.9f)
                
                obj[i].copytriangles(&patern);
                
                //addSphere(&obj[i],4,8, size/2.0f, defaultimg,2,4, 0 | TRIANGLE_VERTEX_NORMALS, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
                //addCylinder(&obj[i], 10, 1, size, size/2.0f, size/2.0f, defaultimg, 1.0f, 1.0f, 0, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
                //else if(i%2==0)
                //addCube(&obj[i], size,size,size, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX());
                
                obj[i].setproperties(OBJECT_ROTATE_TOCAMERA|OBJECT_DRAW_CHILDREN/*|OBJECT_DEBUG|OBJECT_DRAW_DISPLAYLIST*/);
                obj[i].moveto(pos);
                //obj[i].moveto(POINT(POGEL::FloatRand(sps)-sps/2.0,POGEL::FloatRand(sps)-sps/2.0,POGEL::FloatRand(sps)-sps/2.0)/0.5f * POINT(1.0f,1.0f,0.0f));
                
                //obj[i].scroll_all_tex_values(.5,.5);
                //float g = POGEL::FloatRand(1.0)+0.0;
                
                if(dimlock) {
		        obj[i].position.z = 0.0;
		        obj[i].direction.z = 0.0f;
		        }
                
                //obj[i].translate(POGEL::VECTOR(1.0f,0.0f,0.0f)*(POGEL::FloatRand(2.0f)-1.0f));
                //obj[i].moveto(m.transformPoint(POINT(0.0f, ((float)i+1)*sps + (sqrt(10000000000000.0)/1000000.0), 0.0f)));
                //obj[i].moveto(POINT(0.0f,(float)(i)*2.75f,0.0f));
                //obj[i].turnto(POINT(POGEL::FloatRand(360.0), POGEL::FloatRand(360.0), POGEL::FloatRand(360.0)) * POINT(1.0f,1.0f,1.0f));
                //obj[i].turnto(POINT());
                sphs[i] = new POGEL::PHYSICS::SOLID(&obj[i], POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1.0f, 0.75f, 1.0f, 1.0f, 1.0f, 1.0f, false, (i%2==0?-1.0f:1.0f)), 2|4|(i%2==0 && false ? 0 : 16));
//                if(i%2==0) sphs[i]->behavior.mass = 5000.0f;
                //sphs[i]->moveto(POINT(POGEL::FloatRand(5.0)-2.5,POGEL::FloatRand(5.0)-2.5,POGEL::FloatRand(5.0)-2.5));
                //sphs[i]->position.print();
                //sphs[i]->turnto(POINT(POGEL::FloatRand(20.0)-10,POGEL::FloatRand(20.0)-10,POGEL::FloatRand(20.0)-10));
                sphs[i]->turnto(POINT(0.0f,0.0f,0.0f));
                sphs[i]->build();
                //sphs[i]->direction=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5).normal()*0.2;
                //sphs[i]->direction = m.transformVector(POGEL::VECTOR((float)sqrt((10000000000000.0f*(GRAVITYCONSTANT/PARTICLE_SLOWDOWN))/(obj[i].position.distance(POGEL::POINT()) )), 0.0f, 0.0f));
                //sphs[i]->spin=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)/0.10f * VECTOR(0.0f,0.0f,1.0f);
                //sphs[i]->rotation=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)/0.010f * VECTOR(1.0f,1.0f,1.0f);
                //sphs[i]->direction = POGEL::VECTOR(0.05f,0.25f,0.0f);
                //sphs[i]->spin=POGEL::VECTOR(0.0f,1.0f,0.0f);
                //sphs[i]->visable = true;
                
                /*if(i == numobjs-1) {
                        //sphs[i]->setOptions(1|4);
                        sphs[i]->position=(POGEL::POINT(-5.0f,0.0f,0.0f));
                        sphs[i]->direction = POGEL::VECTOR(1,0,0)*.5*-1;
                        //sphs[i]->spin=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)/0.10f * VECTOR(1,1,1);
                        //sphs[i]->behavior.mass = 1.0f;
                        //sphs[i]->visable = true;
                        //sphs[i]->bounding.maxdistance=5.0f;
                }*/
                //if(i%50==0)
                	//sphs[i]->zombify();
                
                //sphs[i]->direction = POGEL::VECTOR(1,0,0)*.05;
                
                sphs[i]->setstepstaken(i*emitfrq);
                
                //sphs[i]->resizetrail(64);
                
                sphs[i]->setStepFunc(oob);
                
                sphs[i]->visable = true;
				//usleep(100);
	            sim.addSolid(sphs[i]);
	            sim2.addSolidsGravity(sphs[i]);
	            oob(sphs[i]);
        }
        for(unsigned int i=0;i<numobjs;i++) {
        	//sim .addsingularity( POGEL::PHYSICS::SINGULARITY(&sphs[i]->position,&sphs[i]->behavior.mass) );
	        //sim2.addsingularity( POGEL::PHYSICS::SINGULARITY(&sphs[i]->position,&sphs[i]->behavior.mass) );
	        //sim2.removeSolidKeepGravity(sphs[i]);
        }
        
        //sim .addsingularity( POGEL::PHYSICS::SINGULARITY(POGEL::POINT(0.0f,0.0f,0.0f),-250000000.0f) );
        //sim2.addsingularity( POGEL::PHYSICS::SINGULARITY(POGEL::POINT(0.0f,0.0f,0.0f), 250000000.0f) );
        //sim.addsingularity( POGEL::PHYSICS::SINGULARITY(POGEL::POINT(0.0f,0.0f,0.0f),-10000000000000.0f) );
        //sim.addfan(POGEL::PHYSICS::FAN(POINT(0.0f,0.0f,0.0f), VECTOR(0.0f,1.0f,0.0f), 50000.0f));
        //sim.gravity = POGEL::VECTOR(0.0f,1.0f,0.0f).normal()*0.8f;
        //sim2.gravity = POGEL::VECTOR(0.0f,-1.0f,0.0f).normal()*0.8f;
        //sim.air_dencity = 1000;
        
        //POGEL::addproperty(POGEL_LABEL);
        
        POGEL::OBJECT* ring = new POGEL::OBJECT();
        ring->setname("border");
        
        //addCylinder(ring, 16, 1, 20.0f, 50.0f, 20.0f, defaultimg, 4.0f, 4.0f, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,10.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
        //addDisk(ring, 16, 1, 50.0f, 0.0f, defaultimg,1, 1, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, true, MATRIX(VERTEX(0.0f,-0.01f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
        //addDisk(ring, 3, 1, 200.0f, 0.0f, defaultimg,1, 1, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, true, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
        //addDisk(ring, 16, 1, 20.0f, 2.0f, defaultimg,1, 1, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, true, MATRIX(VERTEX(0.0f,2.01f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
        //addCylinder(ring, 16, 1, 2.01f, 20.0f, 20.0f, defaultimg, 1.0f, 1.0f, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,1.005,0.0f), VERTEX(0.0f,0.0f,0.0f)));
        //addCylinder(ring, 4, 1, 5.0f, 20.0f, 5.0f, defaultimg, 1.0f, 1.0f, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,-2.5f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
        
        //addCube(ring, 20.0f,200.0f,200.0f, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX(POGEL::POINT(0.0f,0.0f,0.0f),POGEL::POINT(0.0f,0.0f,0.0f)));
        
        //addCube(ring, 5.0f,5.0f,5.0f, defaultimg, 1,1,0,POGEL::MATRIX(POGEL::POINT(0.0f,0.0f,0.0f),POGEL::POINT(0.0f,0.0f,0.0f)));
        
        //
        /*addCube(ring, 20.0f,20.0f,20.0f, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX(POGEL::POINT(20.0f,10.0f,0.0f),POGEL::POINT(0.0f,0.0f,0.0f)));
        addCube(ring, 20.0f,20.0f,20.0f, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX(POGEL::POINT(-20.0f,10.0f,0.0f),POGEL::POINT(0.0f,0.0f,0.0f)));*/
        
        //addSphere(ring,32,32, 5.0f, defaultimg,2,4, 0 , MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
        addSphere(ring,32,32, sheerradius, defaultimg,2,4, 0|TRIANGLE_COLORED, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
        //addSphere(ring,8,8, 5.0f, defaultimg,2,4, 0 , MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
        //addSphere(ring,4,4, 5.0f, defaultimg,2,4, 0 , MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
        
        /*addCylinder(ring, 4, 1, 10.0f,10.0f, 10.0f, defaultimg, 4.0f, 4.0f, 0, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
        addDisk    (ring, 4, 1, 10.0f, 0.0f, defaultimg, 2, 2, 0, !true, MATRIX(VERTEX(0.0f,0.0f,5.0f), VERTEX(0.0f,0.0f,180.0f)));
        addDisk    (ring, 4, 1, 10.0f, 0.0f, defaultimg, 2, 2, 0, !true, MATRIX(VERTEX(0.0f,0.0f,-5.0f), VERTEX(0.0f,0.0f,180.0f)));*/
        
        ring->setproperties(0);//|OBJECT_DRAW_DISPLAYLIST);
        ring->moveto(POGEL::POINT(0.0f,0.0f,0.0f));
        //ring->turnto(POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0))*1);
        ring->build();
        border = new POGEL::PHYSICS::SOLID(ring, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(), 1|PHYSICS_SOLID_CONCAVE|0);
        //border->spin = POGEL::VECTOR(POGEL::FloatRand(1.0f),POGEL::FloatRand(1.0f),POGEL::FloatRand(1.0f))*1.0f;
        //border->spin = POGEL::VECTOR(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0))*0.001;
        //border->spin = POGEL::VECTOR(0,0,-0.1);
        //border->direction = POGEL::VECTOR(0,1,0)/10;
        border->behavior.bounce = 1.0f;
        border->behavior.friction = 0.0f;
        border->behavior.mass = 1.0f;
        
        border->behavior.magnetic = false;
        border->behavior.charge = -0.01f;
        //border->resizetrail(1000);
        //border->visable = false;
        //border->spin = POGEL::VECTOR(0.0f,1.0f,0.0f);
        //sim.addSolid(border);
        
        for(unsigned int a = 0; a < border->getnumfaces(); a++) {
           	POGEL::TRIANGLE t = border->gettriangle(a);
           	t.addproperty(TRIANGLE_COLORED);
           	for(int b = 0; b < 3; b++)
           		t.vertex[b].color = POGEL::COLOR(1,1,1,0.25);
           	border->settriangle(a, t);
        }
        
        //POGEL::addproperty(POGEL_WIREFRAME);
        border->build();
        //POGEL::removeproperty(POGEL_WIREFRAME);
        
        border->visable = true;
        
        //for(int i = 0; i < numobjs; i++) sim.addSolid(sphs[i]);
        
        
        POGEL::OBJECT* sp = new POGEL::OBJECT();
        sp->setname("ball");
        addSphere(sp,32,32, sheerradius*2, defaultimg,1,1, 0 | TRIANGLE_COLORED, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
        //addCube(sp, 10,6,4, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX());
        sp->setproperties(0);
        sp->moveto(POGEL::POINT(0.0f,0.0f,0.0f));
        //sp->turnto(POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0)));
        //sp->turnto(POGEL::POINT(0,0,0));
        sp->build();
        ball = new POGEL::PHYSICS::SOLID(sp, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1, 0, 1, 1, 1, 1, false, 0), 1|PHYSICS_SOLID_CONCAVE|16);
        //ball->spin = POGEL::VECTOR(POGEL::FloatRand(2)-1,POGEL::FloatRand(2)-1,POGEL::FloatRand(2)-1)*2;
        ball->behavior.bounce = 1.0f;
        ball->behavior.friction = 1.0f;
        ball->behavior.mass = 1.0f;
        ball->behavior.magnetic = false;
        ball->behavior.charge = -150.0f;
        ball->resizetrail(10);
        ball->build();
        //sim.addSolid(ball);
        sim2.addSolid(ball);
        
        for(unsigned int a = 0; a < ball->getnumfaces(); a++) {
           	POGEL::TRIANGLE t = ball->gettriangle(a);
           	t.addproperty(TRIANGLE_COLORED);
           	for(int b = 0; b < 3; b++)
           		t.vertex[b].color = POGEL::COLOR(1,1,1,0.25);
           	ball->settriangle(a, t);
        }
        
        ball->visable = true;
        
        POGEL::OBJECT* sq = new POGEL::OBJECT();
        sq->setname("box");
        //addSphere(sq,10,10, 2.75f, defaultimg,1,1, 0 | TRIANGLE_VERTEX_NORMALS, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
        addCube(sq, size,size,size, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX());
        sq->setproperties(0);
        sq->moveto(POGEL::POINT(5.0f,-5.0f,0.0f));
        sq->turnto(POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0))*0);
        //sq->turnto(POGEL::POINT(0,0,0));
        sq->build();
        box = new POGEL::PHYSICS::SOLID(sq, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1, 0, 1, 1, 1, 1, false, 0), 2|PHYSICS_SOLID_CONVEX|0);
        //box->spin = POGEL::VECTOR(POGEL::FloatRand(2)-1,POGEL::FloatRand(2)-1,POGEL::FloatRand(2)-1)*2;
        box->behavior.bounce = 1.0f;
        box->behavior.friction = 1.0f;
        box->behavior.mass = 5000.0f;
        box->behavior.magnetic = false;
        box->behavior.charge = 150.0f;
        box->resizetrail(1000);
        box->build();
        //sim.addSolid(box);
        
        box->visable = true;
        
        POGEL::InitFps();
        printf("\n");
        //camrot.x = 90.0f;
        
        //for(int i = 0; i < 10; i++)
        
        /*keys['/']=true;
       	sim.increment();
       	keys['/']=false;*/
       	
       	v.setretscreensize(&screenx, &screeny);
   	    v.settexsize(800, 600);
   	    //sim.setThreadsNum(4); sim2.setThreadsNum(4);
       	//sim.FORCEfastAccessList(); sim2.FORCEfastAccessList();
       	#ifdef th
       	simulator_runner = new THREAD(sim_runner);
       	simulator_runner->startThread();
       	//exfnc = exitingfunction;
       	#endif
}

//unsigned long frames=0;

POGEL::POINT camrot(0,0,0), campos;
bool p = false;
/* The main drawing function. */
void DrawGLScene()
{
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);             // Clear The Screen And The Depth Buffer
        glLoadIdentity();                               // Reset The View
        
        /*POGEL::POINT p = POGEL::POINT();
        for(unsigned int i = 0; i < numobjs; i++)
                p += sphs[i]->position;
        p /= float(numobjs);*/
        
        /*float d = 0.0f;
        for(unsigned int i = 0; i < numobjs; i++)
                d += p.distance(sphs[i]->position);
        d /= float(numobjs);
        if(d >= 99.0f)
                d=99.0f;
        
        glTranslatef(p.x*-1.0f, p.y*-1.0f, p.z*-1.0f);
        glTranslatef(0.0f,0.0f,-(d*2.0f));*/
        
        /*if(mouse_state == GLUT_DOWN && mouse_button == GLUT_LEFT_BUTTON) {
			float xrt = float(mouse_x-mouse_static_x)/float(screenx);
			float yrt = float(mouse_y-mouse_static_y)/float(screeny);
			camrot.x = (yrt)*180 + prot.x;
			camrot.y = (xrt)*360 + prot.y;
			
			//camrot.x += (syrt)*180;
			//camrot.y += (sxrt)*360;
		}
		POGEL::message("mouse: (%d,%d), (%d,%d)\n",mouse_x,mouse_y,mouse_static_x,mouse_static_y);*/
		
		MOUSE_ROT_FUNC
		//if(mouse_state == GLUT_DOWN && mouse_button == GLUT_LEFT_BUTTON)  camrot = POGEL::POINT( ((float(-1*(mouse_static_y-mouse_y))/float(screeny))*180 + prot.x), ((float(-1*(mouse_static_x-mouse_x))/float(screenx))*360 + prot.y), camrot.z);
		
		glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
        glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHTING);
		
        glTranslatef(0.0f+campos.x,0.0f+campos.y,-100+campos.z);
        //glRotatef( 90.0f,  1.0f, 0.0f, 0.0f );
        glRotatef( camrot.x + ((float)frames)*0.0f*x,  1.0f, 0.0f, 0.0f );
        glRotatef( camrot.y + ((float)frames)*0.0f*y,  0.0f, 1.0f, 0.0f );        
        glRotatef( camrot.z + ((float)frames)*0.0f*z,  0.0f, 0.0f, 1.0f );
        //glRotatef( 90.0f,  0.0f, 1.0f, 0.0f );
        //message("%ld: ",frames);
        //if(POGEL::frames >= 1000) go = false;
        
        POGEL::IncrementFps();
        if(frames%frameskip == 0)
        POGEL::PrintFps();
        
        if(keys['['])
        	ball->direction.y=1;
        else if(keys[']'])
        	ball->direction.y=-1;
        else if(ball->hasOption(1)) ball->direction.y=0;
        if(keys['{'])
        	ball->direction.x=1;
        else if(keys['}'])
        	ball->direction.x=-1;
        else if(ball->hasOption(1)) ball->direction.x=0;
        
        ball->position.z=0;
        ball->direction.z=0;
        
        /*if(keys[','])
        	ball->addOption(1);
        if(keys['.'])
        	ball->removeOption(1);*/
        
        if(keys['v'])
        	dimlock = !dimlock;
        //if(dimlock) printf("hel\n");
        
        if(keys['-']) {
        	sphs[numobjs-1]->moveto(POGEL::POINT(0,20,0));
        	sphs[numobjs-1]->direction=POGEL::VECTOR();
        }
        
        //POGEL::ThrotleFps(60);
        
        //frames++;
        //border->rotate(POGEL::VECTOR(0.0f,1.0f,0.0f)/1.0f);
        //if(frames < 100)
        //for(int i = 0; i < 1; i++)
                //getchar();
        
        //if(keys['\r']) go = !go;
        if(keys['/']) sheerrot*=0;
        if(frames%frameskip == 0) {
        		sim2.draw();
        		glRotatef( sheerrot.x ,  1.0f, 0.0f, 0.0f );
		        glRotatef( sheerrot.y ,  0.0f, 1.0f, 0.0f );        
		        glRotatef( sheerrot.z ,  0.0f, 0.0f, 1.0f );
                sim.draw();
                unsigned int op = POGEL::getproperties();
                POGEL::addproperty(POGEL_TRAILS);
                border->draw();
                POGEL::setproperties(op);
                border->step();
                if(keys['m'])
	                sim.drawGravityGrid(10000, .075*10, POGEL::POINT(0,0,0), 8);
                glRotatef( sheerrot.z ,  0.0f, 0.0f,-1.0f );
		        glRotatef( sheerrot.y ,  0.0f,-1.0f, 0.0f );        
		        glRotatef( sheerrot.x , -1.0f, 0.0f, 0.0f );
                //unsigned int op = POGEL::getproperties();
                //POGEL::addproperty(POGEL_BOUNDING);
                //border->bounding.draw(POGEL::POINT());
                //POGEL::setproperties(op);
        }
        
        #ifndef th
        
        if(keypres) {
        				//if(POGEL::GetTimePassed() < 60.0f)
                        sim.increment(); sim2.increment();
                        keypres = false;
                        sheerrot += sheerspin;
                }//sc = true;
			//printf("updates = %ld\n", updts++);
                else if(go) {
                //if(POGEL::GetTimePassed() < 60.0f)
                       sim.increment(); sim2.increment();
                       sheerrot += sheerspin;
                }
        #endif
        if(keys['t']) {
        	keys['t'] = false;
        	sim.gravity *= -1.0f;
        }
        if(keys['g']) {
        	keys['g'] = false;
        	sim.gravity *= 0.0f;
        }
        if(keys['r']) {
        	keys['r'] = false;
        	sim.gravity /= 2.0f;
        }
        if(keys['f']) {
        	keys['f'] = false;
        	sim.gravity *= 2.0f;
        }
        
        if(keys['x'])
        	sim.gravity = POGEL::VECTOR(1.0f,0.0f,0.0f).normal()*9.8f;
        if(keys['X'])
        	sim.gravity = POGEL::VECTOR(-1.0f,0.0f,0.0f).normal()*9.8f;
        if(keys['y'])
        	sim.gravity = POGEL::VECTOR(0.0f,1.0f,0.0f).normal()*9.8f;
        if(keys['Y'])
        	sim.gravity = POGEL::VECTOR(0.0f,-1.0f,0.0f).normal()*9.8f;
        if(keys['z'])
        	sim.gravity = POGEL::VECTOR(0.0f,0.0f,1.0f).normal()*9.8f;
        if(keys['Z'])
        	sim.gravity = POGEL::VECTOR(0.0f,0.0f,-1.0f).normal()*9.8f;
        
        if(keys['p']) {
        	keys['p'] = false;
        	p = !p;
        }
        if(p)
        	switch(POGEL::frames/150%4) {
        		case 0:
        			sim.gravity = POGEL::VECTOR(0.0f,-1.0f,0.0f).normal()*9.8f;
        		break;
        		case 1:
        			sim.gravity = POGEL::VECTOR(1.0f,0.0f,0.0f).normal()*9.8f;
        		break;
        		case 2:
        			sim.gravity = POGEL::VECTOR(0.0f,1.0f,0.0f).normal()*9.8f;
        		break;
        		case 3:
        			sim.gravity = POGEL::VECTOR(-1.0f,0.0f,0.0f).normal()*9.8f;
        		break;
        	}
        	//sim2.gravity = sim.gravity*-1;
        if(keys[',']) sheerrot.z += 0.2;
        if(keys['.']) sheerrot.z -= 0.2;
        #ifndef th
       
		/*if(keys['c']) {
			keys['c'] = false;
			sim.deactivation = !sim.deactivation;
		}
		if(sim.deactivation)
			std::cout << " deactivation enabled" << std::endl;
		else
			std::cout << " deactivation disabled" << std::endl;*/
        #endif	
        //sim.gravity.print();
        
        //sphs[0]->position.print();
        //POGEL::message("duration = %f\n", POGEL::duration);
        
        /*if(sc) {
        	sc = false;
			v.save(1, POGEL::string("screens/sc_%04u.bmp", frames));
		}*/
        
        //message("\n");
        //border->bounding.draw(POGEL::POINT());
        // since this is double buffered, swap the buffers to display what just got drawn.
        if(frames%frameskip == 0) {
        glutSwapBuffers();
        }
        
        //printf("POGEL::getproperties() = %u\n", POGEL::getproperties());
}
