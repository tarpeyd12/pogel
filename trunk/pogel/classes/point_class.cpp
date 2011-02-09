#include <math.h>
#include "point_class.h"
#include "matrix_class.h"

void POGEL::POINT::draw() {
	#ifdef OPENGL
	glDisable(GL_TEXTURE_2D); glDisable(GL_LIGHTING);
	glPointSize(5);
	POGEL::COLOR(0,1.75,.75,1).set();
	glBegin(GL_POINTS); glVertex3f(x,y,z); glEnd();
	glPointSize(1);
	POGEL::COLOR(1,1,1,1).set();
	glEnable(GL_LIGHTING); glEnable(GL_TEXTURE_2D);
	#endif
};

void POGEL::POINT::draw(unsigned int a) {
	#ifdef OPENGL
	glDisable(GL_TEXTURE_2D); glDisable(GL_LIGHTING);
	glPointSize(a);
	glBegin(GL_POINTS); glVertex3f(x,y,z); glEnd();
	glPointSize(1);
	glEnable(GL_LIGHTING); glEnable(GL_TEXTURE_2D);
	#endif
};

void POGEL::POINT::draw(unsigned int a, POGEL::COLOR color) {
	#ifdef OPENGL
	glDisable(GL_TEXTURE_2D); glDisable(GL_LIGHTING);
	glPointSize(a);
	color.set();
	glBegin(GL_POINTS); glVertex3f(x,y,z); glEnd();
	glPointSize(1);
	POGEL::COLOR(1,1,1,1).set();
	glEnable(GL_LIGHTING); glEnable(GL_TEXTURE_2D);
	#endif
};

void POGEL::POINT::drawto(POGEL::POINT p) {
	#ifdef OPENGL
	glDisable(GL_TEXTURE_2D); glDisable(GL_LIGHTING);
	POGEL::COLOR(0,1.75,.75,1).set();
	glBegin(GL_LINES);
		glVertex3f(x,y,z); glVertex3f(p.x,p.y,p.z);
	glEnd();
	POGEL::COLOR(1,1,1,1).set();
	glEnable(GL_LIGHTING); glEnable(GL_TEXTURE_2D);
	#endif
};

POGEL::VECTOR::VECTOR(POGEL::POINT p1, POGEL::POINT p2) {
	this->frompoints(p1, p2);
};

void POGEL::VECTOR::normalize() {
	float len=getdistance();
	if(len != 0.0f) {
		x/=len; y/=len; z/=len;
	}
};

POGEL::VECTOR POGEL::VECTOR::normal() {
	return *this/getdistance();
};

float POGEL::VECTOR::getdistance() {
	float r = (float)sqrt(x*x+y*y+z*z);
	if(isnan(r))
		return 0.0f;
	return r;
};

float POGEL::VECTOR::getangle(POGEL::VECTOR other, POGEL::VECTOR ref) {
	POGEL::VECTOR tmp, tmp1;
	tmp = *this;
	tmp.normalize();
	other.normalize();
	float angle = POGEL::RadiansToDegrees(acos(tmp.dodotproduct(other).getdistance()));
	tmp1 = *this;
	tmp1.normalize();
	return tmp1.dodotproduct(other).getdistance() < 0.0f ? -angle : angle;
};

POGEL::VECTOR& POGEL::VECTOR::dodotproduct(POGEL::VECTOR in) {
	POGEL::VECTOR a(x,y,z), b(in.x,in.y,in.z);
	//a.set_values(x,y,z);
	//b.set_values(in.x,in.y,in.z);
	
	x=((a.y*b.z)-(a.z*b.y));
	y=((a.z*b.x)-(a.x*b.z));
	z=((a.x*b.y)-(a.y*b.x));
	return *this;
};

float POGEL::VECTOR::dotproduct(POGEL::VECTOR a) {
	return (x*a.x)+(y*a.y)+(z*a.z);
};

void POGEL::VECTOR::frompoints(POGEL::POINT a, POGEL::POINT b) {
	x=b.x-a.x;
	y=b.y-a.y;
	z=b.z-a.z;
};

