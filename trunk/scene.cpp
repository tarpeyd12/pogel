#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>		// Header File For The OpenGL32 Library
#include <GL/glu.h>		// Header File For The GLu32 Library
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "scene.h"
#include "window.h"

#include "pogel/pogel.h"

using namespace POGEL;

OBJECT obj;
OBJECT *circles;
#define			speed				2.0f
#define			scale				1.0f
#define			earthsize			0.25f*scale
#define			numrings			16
#define			ringwidth			0.05f*scale
#define			firstradius			earthsize*2.0f*scale
#define			ringheight			0.05f*scale
#define			divisions			32
#define			disk_lighting		TRIANGLE_LIT
#define			sphere_lighting		TRIANGLE_LIT
#define			cylinder_lighting	TRIANGLE_VERTEX_NORMALS
float ring_sizes[numrings+1];

IMAGE *earth;

GLfloat LightAmbient[]= { 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightDiffuse[]= { 5.0f, 5.0f, 5.0f, 5.0f };
GLfloat LightPosition[]= { 5.0f, 0.0f, 0.0f, 1.0f };

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
	
	for(int i=0;i<numrings+1;i++)
		ring_sizes[i]=firstradius+((float)i*ringwidth);
	
	earth=new IMAGE("Data/earth.bmp");
	IMAGE *glass_rings=new IMAGE("Data/Glass.bmp");
	//printf("obj = %p\n",&obj);
	obj.setname("earth");
	addSphere(&obj,divisions,divisions,earthsize, earth,1,1, 0 | sphere_lighting, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
	addCylinder(&obj, 8, 8, ring_sizes[0]*2.0f, ringheight/4.0f, ringheight/4.0f, glass_rings,5.0f/128.0f, 1, 0 | cylinder_lighting, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
	
	obj.setproperties(0 /*| OBJECT_DEBUG*/ | OBJECT_DRAW_DISPLAYLIST | OBJECT_DRAW_CHILDREN);
	obj.build();
	obj.moveto(POINT(0.0f,0.0f,0.0f));
	obj.turnto(POINT(0.0f,90.0f,0.0f));
	
	OBJECT tmp;
	OBJECT *cur;
	OBJECT *last=NULL;
	//circles=(OBJECT*)malloc(sizeof(OBJECT));
	for(int i=0;i<numrings;i++) {
		printf("building_%d\n",i);
		cur=new OBJECT(POGEL::string("%d",i));
		addDisk(
			cur, \
			divisions, 1, \
			ring_sizes[i+1]/1.0f, \
			ring_sizes[i+0]/1.0f, \
			glass_rings,8, 5.0f/128.0f, \
			0 | disk_lighting, \
			true, \
			MATRIX(VERTEX(0.0f,0.0f,ringheight/4.0f), VERTEX(0.0f,0.0f,0.0f))
		);
		addDisk(
			cur, \
			divisions, 1, \
			ring_sizes[i+1]/1.0f, \
			ring_sizes[i+0]/1.0f, \
			glass_rings,8, 5.0f/128.0f, \
			0 | disk_lighting | TRIANGLE_INVERT_NORMALS, \
			true, \
			MATRIX(VERTEX(0.0f,0.0f,-ringheight/4.0f), VERTEX(0.0f,0.0f,0.0f))
		);
		addCylinder(
			cur, \
			divisions, 1, \
			ringheight, \
			ring_sizes[i+0], \
			ring_sizes[i+0], \
			glass_rings, 8, 5.0f/128.0f, \
			0 | cylinder_lighting | TRIANGLE_INVERT_NORMALS, \
			MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,180.0f,0.0f))
		);
		addCylinder(
			cur, \
			divisions, 1, \
			ringheight, \
			ring_sizes[i+1], \
			ring_sizes[i+1], \
			glass_rings, 8, 5.0f/128.0f, \
			0 | cylinder_lighting, \
			MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(90.0f,180.0f,0.0f))
		);
		if(i==0)
			//cur->setproperties(0 | OBJECT_DRAW_DISPLAYLIST);
			cur->addobject(&obj);
		//else
			cur->setproperties(0 /*| OBJECT_DEBUG*/ | OBJECT_DRAW_DISPLAYLIST | OBJECT_DRAW_CHILDREN);
		if(i!=numrings-1) cur->build();
		if(i!=0) cur->addobject(last);
		last=cur;
	}
	circles=last;
	
	// verticle ring
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting | TRIANGLE_INVERT_NORMALS, true, MATRIX(VERTEX(ringheight/4.0f,0.0f,0.0f), VERTEX(0.0f,0.0f+90.0f,0.0f)));
	
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting, true, MATRIX(VERTEX(-ringheight/4.0f,0.0f,0.0f), VERTEX(0.0f,0.0f+90.0f,0.0f)));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+0], ring_sizes[(numrings-1)+0], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting | TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(0.0f,0.0f,90.0f)));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+1], ring_sizes[(numrings-1)+1], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting,MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(0.0f,0.0f,90.0f)));
	
	// horizontal ring
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting, true, MATRIX(VERTEX(0.0f,ringheight/4.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
	
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting | TRIANGLE_INVERT_NORMALS, true, MATRIX(VERTEX(0.0f,-ringheight/4.0f,0.0f), VERTEX(90.0f,0.0f,0.0f)));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+0], ring_sizes[(numrings-1)+0], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting | TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+1], ring_sizes[(numrings-1)+1], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting,MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(0.0f,0.0f,0.0f)));
	
	POINT rot(0.0f,0.0f,45.0f);
	POINT p = MATRIX(POINT(),rot).getrotation();
	VECTOR t(ringheight/4.0f,0.0f,0.0f);
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting, true, MATRIX(MATRIX(p,0).transformVector(t), VERTEX(rot.x-90.0f,rot.z+90.0f,rot.y+0.0f)));
	
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting | TRIANGLE_INVERT_NORMALS, true, MATRIX(MATRIX(p,0).transformVector(t*-1.0), VERTEX(rot.x-90.0f,rot.z+90.0f,rot.y+0.0f)));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+0], ring_sizes[(numrings-1)+0], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting | TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,0.0f,0.0f), rot));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+1], ring_sizes[(numrings-1)+1], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting,MATRIX(VERTEX(0.0f,0.0f,0.0f), rot));
	
	
	rot = POINT(0.0f,0.0f,-45.0f);
	p = MATRIX(POINT(),rot).getrotation();
	t = VECTOR(ringheight/4.0f,0.0f,0.0f);
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting, true, MATRIX(MATRIX(p,0).transformVector(t), VERTEX(rot.x-90.0f,rot.z+90.0f,rot.y+0.0f)));
	
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting | TRIANGLE_INVERT_NORMALS, true, MATRIX(MATRIX(p,0).transformVector(t*-1.0), VERTEX(rot.x-90.0f,rot.z+90.0f,rot.y+0.0f)));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+0], ring_sizes[(numrings-1)+0], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting | TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,0.0f,0.0f), rot));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+1], ring_sizes[(numrings-1)+1], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting,MATRIX(VERTEX(0.0f,0.0f,0.0f), rot));
	
	
	rot = POINT(0.0f,45.0f,0.0f);
	p = MATRIX(POINT(),rot).getrotation();
	t = VECTOR(ringheight/4.0f,0.0f,0.0f);
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting, true, MATRIX(MATRIX(p,0).transformVector(t), rot));
	
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting | TRIANGLE_INVERT_NORMALS, true, MATRIX(MATRIX(p,0).transformVector(t*-1.0), rot));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+0], ring_sizes[(numrings-1)+0], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting | TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(rot.x+90.0f,rot.z+0.0f,rot.y+90.0f)));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+1], ring_sizes[(numrings-1)+1], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting,MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(rot.x+90.0f,rot.z+0.0f,rot.y+90.0f)));
	
	rot = POINT(0.0f,-45.0f,0.0f);
	p = MATRIX(POINT(),rot).getrotation();
	t = VECTOR(ringheight/4.0f,0.0f,0.0f);
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting, true, MATRIX(MATRIX(p,0).transformVector(t), rot));
	
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting | TRIANGLE_INVERT_NORMALS, true, MATRIX(MATRIX(p,0).transformVector(t*-1.0), rot));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+0], ring_sizes[(numrings-1)+0], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting | TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(rot.x+90.0f,rot.z+0.0f,rot.y+90.0f)));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+1], ring_sizes[(numrings-1)+1], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting,MATRIX(VERTEX(0.0f,0.0f,0.0f), VERTEX(rot.x+90.0f,rot.z+0.0f,rot.y+90.0f)));
	
	
	rot = POINT(45.0f,0.0f,0.0f);
	p = MATRIX(POINT(),rot).getrotation();
	t = VECTOR(0.0f,0.0f,ringheight/4.0f);
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting, true, MATRIX(MATRIX(p,0).transformVector(t), VERTEX(rot.x+90.0f,rot.z+0.0f,rot.y+0.0f)));
	
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting | TRIANGLE_INVERT_NORMALS, true, MATRIX(MATRIX(p,0).transformVector(t*-1.0), VERTEX(rot.x+90.0f,rot.z+0.0f,rot.y+0.0f)));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+0], ring_sizes[(numrings-1)+0], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting | TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,0.0f,0.0f), rot));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+1], ring_sizes[(numrings-1)+1], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting,MATRIX(VERTEX(0.0f,0.0f,0.0f), rot));
	
	rot = POINT(-45.0f,0.0f,0.0f);
	p = MATRIX(POINT(),rot).getrotation();
	t = VECTOR(0.0f,0.0f,ringheight/4.0f);
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting, true, MATRIX(MATRIX(p,0).transformVector(t), VERTEX(rot.x+90.0f,rot.z+0.0f,rot.y+0.0f)));
	
	addDisk(circles, divisions, 1, ring_sizes[(numrings-1)+1]/1.0f, ring_sizes[(numrings-1)+0]/1.0f, glass_rings,8, 5.0f/128.0f, 0 | disk_lighting | TRIANGLE_INVERT_NORMALS, true, MATRIX(MATRIX(p,0).transformVector(t*-1.0), VERTEX(rot.x+90.0f,rot.z+0.0f,rot.y+0.0f)));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+0], ring_sizes[(numrings-1)+0], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting | TRIANGLE_INVERT_NORMALS, MATRIX(VERTEX(0.0f,0.0f,0.0f), rot));
	
	addCylinder(circles, divisions, 1, ringheight, ring_sizes[(numrings-1)+1], ring_sizes[(numrings-1)+1], glass_rings, 8, 5.0f/128.0f, 0 |cylinder_lighting,MATRIX(VERTEX(0.0f,0.0f,0.0f), rot));
	
	circles->build();
}
bool keypres, go = true;
POGEL::POINT camrot, campos;
/* The main drawing function. */
void DrawGLScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	
	glTranslatef(0.0f,0.0f,-5.0f);
	//rings.rotate(VECTOR(0.0f,0.6f,0.0f));
	//glRotatef(23.0f,0.0f,0.0f,-1.0f);
	//obj.draw();
	obj.rotate(VECTOR(0.0f*2.0f,0.2f*2.0f,0.0f*2.0f)*speed);
	circles->draw();
	circles->rotate(VECTOR(0.4f,0.5f,0.1f)*speed);
	for(int i=0;i<numrings-1;i++) {
		char *n = POGEL::string("%d",i);
		switch(i%4) {
			case 1:
				circles->getdecendant(n)->rotate(VECTOR(0.0f,0.2f*((float)numrings-(float)i),0.0f)*speed);
				//circles->getdecendant(string("%d",i))->rotate(VECTOR(0.0f,1.0f/2,0.0f));
			break;
			case 3:
				circles->getdecendant(n)->rotate(VECTOR(0.2f*((float)numrings-(float)i),0.0f,0.0f)*speed);
				//circles->getdecendant(string("%d",i))->rotate(VECTOR(-2.0f/2,0.0f,0.0f));
			break;
			
			default:
				circles->getdecendant(n)->rotate(VECTOR(0.0f,0.0f,0.2f*((float)numrings-(float)i))*speed);
				//circles->getdecendant(string("%d",i))->rotate(VECTOR(0.0f,0.0f,1.0f/2));
			break;
		}
		if(n!=NULL)free(n);
	}
	//obj.scroll_all_tex_values(0.0004f,0.0005f);
	// since this is double buffered, swap the buffers to display what just got drawn.
	glutSwapBuffers();
}

