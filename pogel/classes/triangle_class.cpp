#include <GL/gl.h>
#include <stdio.h>
#include <stdlib.h>
#include "triangle_class.h"

POGEL::TRIANGLE::TRIANGLE(std::string s, POGEL::IMAGE* im) {
	texture = im;
	sscanf(s.c_str(), "{[%u],", &properties);
	for(int i = 0; i < 3; i++) {
		char* vt = POGEL::string("0 %d",i);
		std::string v = POGEL::getStringComponentLevel('{','}', s, vt);
		vertex[i] = POGEL::VERTEX(v);
		free(vt);
	}
	normal = POGEL::VECTOR(POGEL::getStringComponentLevel('{','}', s, "0 3"));
};

POGEL::TRIANGLE::TRIANGLE(std::string s) {
	texture = NULL;
	sscanf(s.c_str(), "{[%u],", &properties);
	for(int i = 0; i < 3; i++) {
		char* vt = POGEL::string("0 %d",i);
		std::string v = POGEL::getStringComponentLevel('{','}', s, vt);
		vertex[i] = POGEL::VERTEX(v);
		free(vt);
	}
	normal = POGEL::VECTOR(POGEL::getStringComponentLevel('{','}', s, "0 3"));
};

void POGEL::TRIANGLE::load(POGEL::VERTEX a,POGEL::VERTEX b,POGEL::VERTEX c,POGEL::IMAGE *tex,unsigned int prop) {
	POGEL::VECTOR vct[2];

	vertex[0]=a;
	vertex[1]=b;
	vertex[2]=c;
	texture=tex;
	properties=prop;
	
	vct[0].frompoints(vertex[0].topoint(),vertex[1].topoint());
	vct[0].normalize();
	vct[1].frompoints(vertex[0].topoint(),vertex[2].topoint());
	vct[1].normalize();
	
	vct[0].dodotproduct(vct[1]);
	normal=vct[0];
	normal.normalize();
	bounding = POGEL::BOUNDING(BOUNDING_TRIANGLE);
	getbounding();
};

void POGEL::TRIANGLE::load(POGEL::VERTEX* verts,POGEL::IMAGE *tex,unsigned int prop) {
	if(verts!=NULL)
		load(verts[0],verts[1],verts[2],tex,prop);
	else printf("triangle loading failed, null pointer to vertex array.\n");
};

void POGEL::TRIANGLE::scroll_tex_values(float s, float t) {
	for(int i=0;i<3;i++) vertex[i].scroll_tex_values(s,t);
};

POGEL::TRIANGLE POGEL::TRIANGLE::transform(POGEL::MATRIX* m) {
	POGEL::TRIANGLE t(vertex, texture, properties);
	m->transformTriangle(&t);
	t.getbounding();
	return t;
};

POGEL::POINT POGEL::TRIANGLE::middle() {
	POGEL::POINT p;
	p.x=(vertex[0].x+vertex[1].x+vertex[2].x)/3;
	p.y=(vertex[0].y+vertex[1].y+vertex[2].y)/3;
	p.z=(vertex[0].z+vertex[1].z+vertex[2].z)/3;
	return p;
};

bool POGEL::TRIANGLE::isinfront(POGEL::POINT p) {
	POGEL::POINT closest = middle();
	POGEL::POINT points[] = {
		vertex[0].topoint(), \
		vertex[1].topoint(), \
		vertex[2].topoint(), \
		((vertex[0]+vertex[1])/2.0f).topoint(), \
		((vertex[0]+vertex[2])/2.0f).topoint(), \
		((vertex[1]+vertex[2])/2.0f).topoint(), \
		(vertex[0].topoint()+closest)/2.0f, \
		(vertex[1].topoint()+closest)/2.0f, \
		(vertex[2].topoint()+closest)/2.0f
	};
	
	for(int i=0;i<9;i++) // go through all the refrence points on the triangle
		// find the closest one, but one that is not too close
		if(p.distance(points[i]) < p.distance(closest) && !( p.distance(points[i]) < normal.getdistance() ))
			closest = points[i];
	
	float distance_closest = p.distance(closest);
	float distance_positive = p.distance(closest+normal.topoint());
	float distance_negative = p.distance(closest-normal.topoint());
	
	
	/*
	
	if( distance_closest > normal.getdistance() )
		return distance_positive <= distance_negative;
	else
		return ( distance_closest <= distance_negative || distance_closest >= distance_positive );
	
	*/
	
	return (	distance_closest > normal.getdistance() ? \
				distance_positive <= distance_negative : \
				( distance_closest <= distance_negative || distance_closest >= distance_positive ) \
			);
};

bool POGEL::TRIANGLE::distcheck(POGEL::POINT p, float dist) {
	POGEL::POINT closest = middle();
	POGEL::POINT points[] = {
		middle(), \
		vertex[0].topoint(), \
		vertex[1].topoint(), \
		vertex[2].topoint(), \
		((vertex[0]+vertex[1])/2.0f).topoint(), \
		((vertex[0]+vertex[2])/2.0f).topoint(), \
		((vertex[1]+vertex[2])/2.0f).topoint(), \
		(vertex[0].topoint()+closest)/2.0f, \
		(vertex[1].topoint()+closest)/2.0f, \
		(vertex[2].topoint()+closest)/2.0f
	};
	
	for(int i=0;i<1;i++)
		if(p.distance(points[i]) <= dist)
			return true;
	return false;
};

float POGEL::TRIANGLE::distance(POGEL::POINT p) {
	POGEL::POINT mid = middle();
	float dist = 0.0f;// p.distance(mid);
	POGEL::POINT points[] = {
		mid, \
		vertex[0].topoint(), \
		vertex[1].topoint(), \
		vertex[2].topoint(), \
		((vertex[0]+vertex[1])/2.0f).topoint(), \
		((vertex[0]+vertex[2])/2.0f).topoint(), \
		((vertex[1]+vertex[2])/2.0f).topoint(), \
		(vertex[0].topoint()+mid)/2.0f, \
		(vertex[1].topoint()+mid)/2.0f, \
		(vertex[2].topoint()+mid)/2.0f
	};
	
	for(int i=0;i<10;i++)
		if(p.distance(points[i]) < dist)
			dist = p.distance(points[i]);
		//dist += p.distance(points[i]);
	return dist/10.0f;
};

void POGEL::TRIANGLE::getbounding() {
	bounding.clear();
	POGEL::POINT mid = middle();
	for(int i=0;i<3;i++)
		bounding.addpoint(mid, vertex[i].topoint());
	bounding.fin();
};

void POGEL::TRIANGLE::draw() {
	#ifdef OPENGL
	if(texture!=NULL)
		texture->set();
	if(((hasproperty(TRIANGLE_LIT)) || (hasproperty(TRIANGLE_VERTEX_NORMALS))) /*&& !POGEL::hasproperty(POGEL_WIREFRAME)*/)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
	
	if(POGEL::hasproperty(POGEL_WIREFRAME))
		glBegin(GL_LINES);
	else
		glBegin(GL_TRIANGLES);
		
		if((hasproperty(TRIANGLE_LIT)) && !(hasproperty(TRIANGLE_VERTEX_NORMALS))) {
			if(hasproperty(TRIANGLE_INVERT_NORMALS))
				glNormal3f(-normal.x,-normal.y,-normal.z);
			else
				glNormal3f( normal.x, normal.y, normal.z);
		}
		for(unsigned int i = 0; i < ( POGEL::hasproperty(POGEL_WIREFRAME) ? 4 : 3 ) ; i++) {
			if(vertex[i%3].usable) {
				if(hasproperty(TRIANGLE_COLORED)) // the triangle will not be colored if GL_LIGHTING is enabled, dont know why.
					vertex[i%3].color.set();
				if(!(hasproperty(TRIANGLE_LIT)) && (hasproperty(TRIANGLE_VERTEX_NORMALS))) {
					if(hasproperty(TRIANGLE_INVERT_NORMALS))
						glNormal3f(-vertex[i%3].normal.x,-vertex[i%3].normal.y,-vertex[i%3].normal.z);
					else
						glNormal3f( vertex[i%3].normal.x, vertex[i%3].normal.y, vertex[i%3].normal.z);
				}
				if(texture!=NULL)
					glTexCoord2f(vertex[i%3].u,vertex[i%3].v); 
				glVertex3f(vertex[i%3].x,vertex[i%3].y,vertex[i%3].z);
			}
		}
	glEnd();
	#endif
};

