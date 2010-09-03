#include <math.h>
#include "point_class.h"
#include "matrix_class.h"

void POGEL::POINT::draw() {
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glPointSize(5);
	glColor3f(0.0f,1.75f,0.75f);
	glBegin(GL_POINTS);
		glVertex3f(x,y,z);
	glEnd();
	glPointSize(1);
	glColor3f(1.0f,1.0f,1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
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
	float angle = POGEL::RadiansToDegrees(acos(tmp.dotproduct(other).getdistance()));
	tmp1 = *this;
	tmp1.normalize();
	return tmp1.dotproduct(other).getdistance() < 0.0f ? -angle : angle;
};

POGEL::VECTOR& POGEL::VECTOR::dotproduct(POGEL::VECTOR in) {
	POGEL::VECTOR a(x,y,z), b(in.x,in.y,in.z);
	//a.set_values(x,y,z);
	//b.set_values(in.x,in.y,in.z);
	
	x=((a.y*b.z)-(a.z*b.y));
	y=((a.z*b.x)-(a.x*b.z));
	z=((a.x*b.y)-(a.y*b.x));
	return *this;
};

void POGEL::VECTOR::frompoints(POGEL::POINT a, POGEL::POINT b) {
	x=b.x-a.x;
	y=b.y-a.y;
	z=b.z-a.z;
};

