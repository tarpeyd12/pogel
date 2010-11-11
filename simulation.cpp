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

#define numobjs 121
#define grd 11
#define sps 1.05f/1
#define size 1.0f/2
OBJECT obj[numobjs];
POGEL::PHYSICS::SOLID **sphs;
POGEL::PHYSICS::SOLID *border;
POGEL::PHYSICS::SOLID *ball;
POGEL::PHYSICS::SOLID *box;

POGEL::PHYSICS::SIMULATION sim;

IMAGE *earth;
IMAGE *defaultimg;

#define rndrnge 10.0f

float x = POGEL::FloatRand(rndrnge)-(rndrnge/2.0f), y = POGEL::FloatRand(rndrnge)-(rndrnge/2.0f), z = POGEL::FloatRand(rndrnge)-(rndrnge/2.0f);

GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]= { 2.0f, 2.0f, 2.0f, 1.0f };
GLfloat LightPosition[]= { 100.0f, 100.0f, 100.0f, 1.0f };

bool dimlock = true;

void oob(SOLID_FNC_DEF) {
        /*if(obj->position.distance(POGEL::POINT()) > border->bounding.maxdistance + 1) {
                obj->moveto(POGEL::POINT(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)*POGEL::FloatRand(20.0f));
                //obj->moveto(POGEL::POINT());
                obj->direction  = POGEL::VECTOR(0.0f,0.0f,0.0f);
                obj->stepstaken = 0;
                //obj->direction=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)/100;
        }*/
                
        /*if(obj->position.distance(POGEL::POINT()) >= 28.0f ) {//  || obj->position.y <= -15.0f ) {
                obj->moveto(POGEL::POINT(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)*POGEL::FloatRand(20.0f));
                //obj->moveto(POGEL::POINT());
                //obj->direction  = POGEL::VECTOR(0.0f,0.0f,0.0f);
                obj->stepstaken = 0;
                obj->direction=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)/100;
        }
        if(obj->position.distance(POGEL::POINT()) >= 25.0f ) {
                //obj->moveto(POGEL::POINT(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)*POGEL::FloatRand(5.0f));
                obj->direction += POGEL::VECTOR(obj->position, POGEL::POINT()).normal()*obj->direction.getdistance()*1.0f;
                //obj->direction *= -1.0f;
                //obj->stepstaken = 0;
        }*/
        /*if(obj->position.y <= -13.0f ) {
                //obj->moveto(POGEL::POINT(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)*POGEL::FloatRand(5.0f));
                //obj->direction = POGEL::VECTOR(obj->position, POGEL::POINT()).normal()*obj->direction.getdistance()*0.8;
                //obj->direction *= -1.0f;
                obj->direction += POGEL::VECTOR(0.0f,1.0f,0.0f).normal()*obj->direction.getdistance()*1.0;
                //obj->stepstaken = 0;
        }*/
        if(keys['/'])
        	obj->stepstaken = rand()%numobjs;
        
        if((obj->stepstaken*1) >= numobjs*1 || keys['o']  /*|| obj->stepstaken == (int)(POGEL::duration*20-fmod(POGEL::duration*20, 1.0))%numobjs  */) {
        	obj->stepstaken = 0;
        	obj->moveto(POGEL::POINT(POGEL::FloatRand(4.0)-2.0,POGEL::FloatRand(4.0)-2.0,POGEL::FloatRand(4.0)-2.0)/5*POGEL::POINT(1,0,1)*10);
        	//obj->direction = POGEL::VECTOR(cos((float)POGEL::frames/100)*20,20,sin((float)POGEL::frames/100)*20).normal()/2*POGEL::VECTOR(1,1,0);
        	//obj->direction = POGEL::VECTOR(.3,1,0).normal()/3;
        	obj->direction = POGEL::VECTOR();
        	
        }
        if(keys['l'])
        	obj->spin=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.0)/0.010f * VECTOR(0,0,1);
        if(keys['L']) {
        	obj->spin = POGEL::VECTOR();
        	obj->rotation = POGEL::POINT();
        }
        //obj->stepstaken -= 1;
        if(dimlock) {
        obj->position.z = 0.0;
        obj->direction.z = 0.0f;
        }
        
};

/* A general OpenGL initialization function.  Sets all of the initial parameters. */
void InitGL(int Width, int Height)              // We call this right after our OpenGL window is created.
{
        glEnable(GL_TEXTURE_2D);                                // Enable Texture Mapping
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // Clear The Background Color To Black 
        glClearDepth(500.0);                                    // Enables Clearing Of The Depth Buffer
        glDepthFunc(GL_LESS);                                   // The Type Of Depth Test To Do
        glEnable(GL_DEPTH_TEST);                                // Enables Depth Testing
        glShadeModel(GL_SMOOTH);                                // Enables Smooth Color Shading
        //glShadeModel(GL_FLAT);                                // Enables flat Color Shading
        glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
        glHint(GL_POLYGON_SMOOTH,GL_NICEST);
        glEnable(GL_POLYGON_SMOOTH);
        
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();                               // Reset The Projection Matrix
        
        gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,500.0f);       // Calculate The Aspect Ratio Of The Window
    
        glMatrixMode(GL_MODELVIEW);
        
        glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
        glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
        glEnable(GL_LIGHT1);
        glEnable(GL_LIGHTING);
        
        //POGEL::addproperty(POGEL_TIMEBASIS);
        
        //sim = new POGEL::PHYSICS::SIMULATION();
        sim.deactivation = false;
        //sim.precision = 0.01f;
        
        sim.boundingskips = 1;
        
        srand((unsigned)time(NULL));
        
        sphs=(POGEL::PHYSICS::SOLID**)malloc(sizeof(POGEL::PHYSICS::SOLID*)*numobjs);
        
        //earth=new IMAGE("Data/earth.bmp");
        defaultimg=new IMAGE("Data/default_2.bmp");
        
        //POGEL::MATRIX m(POGEL::POINT(), POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0)));
        
        //printf("obj = %p\n",&obj);
        for(int i=0;i<numobjs;i++) {
                
                //m = m * POGEL::MATRIX(POGEL::POINT(), POGEL::POINT(x,y,z));
                //POGEL::MATRIX m(POGEL::POINT(), POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0)));
                
                obj[i].setname(POGEL::string("sphere%d",i));
                //if(i%2!=0)
                //addDisk(&obj[i], 3, 1, size/2.0f, 0.0f, defaultimg,1, 1, 0, true, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
                addSphere(&obj[i],4,6, size/2.0f, defaultimg,1,1, 0 | TRIANGLE_VERTEX_NORMALS, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
                //addCylinder(&obj[i], 10, 1, size, size/2.0f, size/2.0f, defaultimg, 1.0f, 1.0f, 0, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
                //else if(i%2==0)
                //addCube(&obj[i], size,size,size, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX());
                
                obj[i].setproperties(0/*|OBJECT_DRAW_DISPLAYLIST*/);
                obj[i].moveto(POINT(
						((float)(i%grd)*sps)-( (float(grd)*sps)/2.0f-sps/2.0f),
						((float)((i/grd)%grd)*sps)-( (float(grd)*sps)/2.0f-sps/2.0f),
						(float)(i/(grd*grd))*(sps) - sps*float(grd)/2.0f + sps/2.0f+.1 /* - (10.0f-(sps/2.0f)), \*/
                ));
                //obj[i].moveto(POINT(POGEL::FloatRand(sps)-sps/2.0,POGEL::FloatRand(sps)-sps/2.0,POGEL::FloatRand(sps)-sps/2.0)/0.5f * POINT(1.0f,1.0f,0.0f));
                //obj[i].translate(POGEL::VECTOR(1.0f,0.0f,0.0f)*(POGEL::FloatRand(2.0f)-1.0f));
                //obj[i].moveto(m.transformPoint(POINT(0.0f, ((float)i+1)*sps + (sqrt(10000000000000.0)/1000000.0), 0.0f)));
                //obj[i].moveto(POINT(0.0f,(float)(i)*2.75f,0.0f));
                //obj[i].turnto(POINT(POGEL::FloatRand(360.0), POGEL::FloatRand(360.0), POGEL::FloatRand(360.0)) * POINT(1.0f,1.0f,1.0f));
                //obj[i].turnto(POINT());
                sphs[i] = new POGEL::PHYSICS::SOLID(&obj[i], POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1.0f, 0.3f, 1.0f, 1.0f, 1.0f, 1.0f, false, (i%2==0?-1.0f:1.0f)), 2|4|(i%2==0 && false ? 0 : 16));
                //sphs[i]->moveto(POINT(POGEL::FloatRand(5.0)-2.5,POGEL::FloatRand(5.0)-2.5,POGEL::FloatRand(5.0)-2.5));
                //sphs[i]->position.print();
                //sphs[i]->turnto(POINT(POGEL::FloatRand(20.0)-10,POGEL::FloatRand(20.0)-10,POGEL::FloatRand(20.0)-10));
                //sphs[i]->turnto(POINT(0.0f,0.0f,0.0f));
                sphs[i]->build();
                //sphs[i]->direction=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)/2;
                //sphs[i]->direction = m.transformVector(POGEL::VECTOR((float)sqrt((10000000000000.0f*(GRAVITYCONSTANT/PARTICLE_SLOWDOWN))/(obj[i].position.distance(POGEL::POINT()) )), 0.0f, 0.0f));
                //sphs[i]->spin=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)/0.10f * VECTOR(0.0f,0.0f,1.0f);
                //sphs[i]->rotation=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)/0.010f * VECTOR(1.0f,1.0f,1.0f);
                //sphs[i]->direction = POGEL::VECTOR(0.05f,0.25f,0.0f);
                //sphs[i]->spin=POGEL::VECTOR(0.0f,1.0f,0.0f);
                sphs[i]->visable = true;
                
                /*if(i == 0) {
                        //sphs[i]->setOptions(1|4);
                        sphs[i]->moveto(POGEL::POINT(-10.5f,0.0f,0.0f)*0);
                        sphs[i]->direction = POGEL::VECTOR(-1,0,0)*-5.0f;
                        //sphs[i]->spin=POGEL::VECTOR(POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5,POGEL::FloatRand(1.0)-0.5)/0.10f * VECTOR(1,1,1);
                        //sphs[i]->behavior.mass = 10.1f;
                        //sphs[i]->visable = true;
                        //sphs[i]->bounding.maxdistance=5.0f;
                }*/
                
                sphs[i]->stepstaken = i*1;
                
                if(i != numobjs-1)
                sphs[i]->resizetrail(5);
                
                if(i == numobjs-1)
                sphs[i]->resizetrail(5);
                
                sphs[i]->setStepFunc(oob);
                
                sphs[i]->visable = false;
                
                sim.addSolid(sphs[i]);
        }
        //sim.addsingularity( POGEL::PHYSICS::SINGULARITY(POGEL::POINT(0.0f,0.0f,0.0f),1000000000.0f*.25) );
        //sim.addsingularity( POGEL::PHYSICS::SINGULARITY(POGEL::POINT(0.0f,0.0f,0.0f),-10000000000000.0f) );
        //sim.addfan(POGEL::PHYSICS::FAN(POINT(0.0f,0.0f,0.0f), VECTOR(0.0f,1.0f,0.0f), 50.0f));
        sim.gravity = POGEL::VECTOR(0.0f,-1.0f,0.0f).normal()*9.8f*PARTICLE_SLOWDOWN_RATIO/1000;
        //sim.air_dencity = 10.0f;
        
        POGEL::OBJECT* ring = new POGEL::OBJECT();
        ring->setname("border");
        
        //addCylinder(ring, 16, 1, 20.0f, 50.0f, 20.0f, defaultimg, 4.0f, 4.0f, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,10.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
        //addDisk(ring, 16, 1, 50.0f, 0.0f, defaultimg,1, 1, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, true, MATRIX(VERTEX(0.0f,-0.01f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
        //addDisk(ring, 3, 1, 200.0f, 0.0f, defaultimg,1, 1, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, true, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
        //addDisk(ring, 16, 1, 20.0f, 2.0f, defaultimg,1, 1, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, true, MATRIX(VERTEX(0.0f,2.01f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
        //addCylinder(ring, 16, 1, 2.01f, 20.0f, 20.0f, defaultimg, 1.0f, 1.0f, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,1.005,0.0f), VERTEX(0.0f,0.0f,0.0f)));
        //addCylinder(ring, 4, 1, 5.0f, 20.0f, 5.0f, defaultimg, 1.0f, 1.0f, 0|TRIANGLE_LIT|TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,-2.5f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
        
        //addCube(ring, 20.0f,200.0f,200.0f, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX(POGEL::POINT(0.0f,0.0f,0.0f),POGEL::POINT(0.0f,0.0f,0.0f)));
        addCube(ring, 30.0f,30.0f,30.0f, defaultimg, 1,1,0,POGEL::MATRIX(POGEL::POINT(0.0f,0.0f,0.0f),POGEL::POINT(0.0f,0.0f,0.0f)));
        //
        /*addCube(ring, 20.0f,20.0f,20.0f, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX(POGEL::POINT(20.0f,10.0f,0.0f),POGEL::POINT(0.0f,0.0f,0.0f)));
        addCube(ring, 20.0f,20.0f,20.0f, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX(POGEL::POINT(-20.0f,10.0f,0.0f),POGEL::POINT(0.0f,0.0f,0.0f)));*/
        
        //addSphere(ring,4,4, 20.0f, defaultimg,1,1, 0 | TRIANGLE_VERTEX_NORMALS, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
        
        //addCylinder(ring, 16, 1, 20.0f, 20.0f, 20.0f, defaultimg, 4.0f, 4.0f, 0 | TRIANGLE_LIT | TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
        //addDisk(ring, 16, 1, 20.0f, 17.5f, defaultimg,1, 1, 0 | TRIANGLE_LIT, true, MATRIX(VERTEX(0.0f,0.0f,10.0f), VERTEX(0.0f,0.0f,180.0f)));
        //addDisk(ring, 16, 1, 20.0f, 0.0f, defaultimg,1, 1, 0 | TRIANGLE_LIT, true, MATRIX(VERTEX(0.0f,0.0f,-10.0f), VERTEX(0.0f,0.0f,180.0f)));
        
        ring->setproperties(0);
        ring->moveto(POGEL::POINT(0.0f,0.0f,0.0f));
        ring->turnto(POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0))*0);
        ring->build();
        border = new POGEL::PHYSICS::SOLID(ring, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(), 1|PHYSICS_SOLID_CONCAVE|0);
        //border->spin = POGEL::VECTOR(POGEL::FloatRand(1.0f),POGEL::FloatRand(1.0f),POGEL::FloatRand(1.0f))*1.0f;
        //border->spin = POGEL::VECTOR(0.0f,0.0f,1.0f);
        //border->direction = POGEL::VECTOR(0,1,0)/10;
        border->behavior.bounce = 1.0f;
        border->behavior.friction = 1.0f;
        border->behavior.mass = 1.0f;
        
        border->behavior.magnetic = false;
        border->behavior.charge = -0.01f;
        border->build();
        //border->visable = false;
        //border->spin = POGEL::VECTOR(0.0f,1.0f,0.0f);
        sim.addSolid(border);
        
        border->visable = false;
        
        
        POGEL::OBJECT* sp = new POGEL::OBJECT();
        sp->setname("ball");
        addSphere(sp,2,32, 2.75f, defaultimg,1,1, 0 | TRIANGLE_LIT, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
        //addCube(sp, 4,4,4, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX());
        sp->setproperties(0);
        sp->moveto(POGEL::POINT(-4.0f,-7.0f,0.0f));
        sp->turnto(POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0)));
        //sp->turnto(POGEL::POINT(0,0,0));
        sp->build();
        ball = new POGEL::PHYSICS::SOLID(sp, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1, 0, 1, 1, 1, 1, false, 0), 1|PHYSICS_SOLID_CONVEX|0);
        ball->behavior.bounce = 0.5f;
        ball->behavior.friction = 1.0f;
        ball->behavior.mass = 1.0f;
        ball->build();
        sim.addSolid(ball);
        
        ball->visable = true;
        
        POGEL::OBJECT* sq = new POGEL::OBJECT();
        sp->setname("box");
        //addSphere(sq,10,10, 3.0f, defaultimg,1,1, 0 | TRIANGLE_VERTEX_NORMALS, MATRIX(POINT(0.0f,0.0f,0.0f), POINT(0.0f,0.0f,0.0f)));
        addCube(sq, 4,4,4, defaultimg, 1,1,0|TRIANGLE_LIT,POGEL::MATRIX());
        sq->setproperties(0);
        sq->moveto(POGEL::POINT(4.0f,-7.0f,0.0f));
        sq->turnto(POGEL::POINT(POGEL::FloatRand(360.0),POGEL::FloatRand(360.0),POGEL::FloatRand(360.0)));
        //sq->turnto(POGEL::POINT(0,0,0));
        sq->build();
        box = new POGEL::PHYSICS::SOLID(sq, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1, 0, 1, 1, 1, 1, false, 0), 1|PHYSICS_SOLID_CONVEX|0);
        box->behavior.bounce = 0.5f;
        box->behavior.friction = 1.0f;
        box->behavior.mass = 1.0f;
        box->build();
        sim.addSolid(box);
        
        box->visable = true;
        
        POGEL::InitFps();
        printf("\n");
        //camrot.x = 90.0f;
        
        //for(int i = 0; i < 10; i++)
        
        /*keys['/']=true;
       	sim.increment();
       	keys['/']=false;*/
}

//unsigned long frames=0;

bool keypres, go = false;
POGEL::POINT camrot, campos;
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
		
        glTranslatef(0.0f+campos.x,0.0f+campos.y,-40.0f+campos.z);
        //glRotatef( 90.0f,  1.0f, 0.0f, 0.0f );
        glRotatef( camrot.x + ((float)frames*x)*0.0f,  1.0f, 0.0f, 0.0f );
        glRotatef( camrot.y + ((float)frames*y)*0.0f,  0.0f, 1.0f, 0.0f );
        glRotatef( camrot.z + ((float)frames*z)*0.0f,  0.0f, 0.0f, 1.0f );
        //glRotatef( 90.0f,  0.0f, 1.0f, 0.0f );
        //message("%ld: ",frames);
        
        POGEL::IncrementFps();
        if(frames%frameskip == 0)
        POGEL::PrintFps();
        
        glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
        glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
        
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
        if(dimlock) printf("hel\n");
        
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
                
        if(frames%frameskip == 0) {
                sim.draw();
                //unsigned int op = POGEL::getproperties();
                //POGEL::addproperty(POGEL_BOUNDING);
                //border->bounding.draw(POGEL::POINT());
                //POGEL::setproperties(op);
        }
        if(keypres) {
        				//if(POGEL::GetTimePassed() < 1.0f)
                        sim.increment();
                        keypres = false;
                }
                else if(go) {
                //if(POGEL::GetTimePassed() < 1.0f)
                        sim.increment();
                }
        
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
        	sim.gravity = POGEL::VECTOR(1.0f,0.0f,0.0f).normal()*9.8f*PARTICLE_SLOWDOWN_RATIO;
        if(keys['X'])
        	sim.gravity = POGEL::VECTOR(-1.0f,0.0f,0.0f).normal()*9.8f*PARTICLE_SLOWDOWN_RATIO;
        if(keys['y'])
        	sim.gravity = POGEL::VECTOR(0.0f,1.0f,0.0f).normal()*9.8f*PARTICLE_SLOWDOWN_RATIO;
        if(keys['Y'])
        	sim.gravity = POGEL::VECTOR(0.0f,-1.0f,0.0f).normal()*9.8f*PARTICLE_SLOWDOWN_RATIO;
        if(keys['z'])
        	sim.gravity = POGEL::VECTOR(0.0f,0.0f,1.0f).normal()*9.8f*PARTICLE_SLOWDOWN_RATIO;
        if(keys['Z'])
        	sim.gravity = POGEL::VECTOR(0.0f,0.0f,-1.0f).normal()*9.8f*PARTICLE_SLOWDOWN_RATIO;
        
        if(keys['p']) {
        	keys['p'] = false;
        	p = !p;
        }
        if(p)
        	switch(POGEL::frames/150%4) {
        		case 0:
        			sim.gravity = POGEL::VECTOR(0.0f,-1.0f,0.0f).normal()*9.8f*PARTICLE_SLOWDOWN_RATIO;
        		break;
        		case 1:
        			sim.gravity = POGEL::VECTOR(1.0f,0.0f,0.0f).normal()*9.8f*PARTICLE_SLOWDOWN_RATIO;
        		break;
        		case 2:
        			sim.gravity = POGEL::VECTOR(0.0f,1.0f,0.0f).normal()*9.8f*PARTICLE_SLOWDOWN_RATIO;
        		break;
        		case 3:
        			sim.gravity = POGEL::VECTOR(-1.0f,0.0f,0.0f).normal()*9.8f*PARTICLE_SLOWDOWN_RATIO;
        		break;
        	}
        	
        	
        //sim.gravity.print();
        sphs[0]->position.print();
        POGEL::message("duration = %f\n", POGEL::duration);
        
        //message("\n");
        
        // since this is double buffered, swap the buffers to display what just got drawn.
        if(frames%frameskip == 0) {
        glutSwapBuffers();
        }
        
        //printf("POGEL::getproperties() = %u\n", POGEL::getproperties());
}
