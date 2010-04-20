#include <math.h>
#include "point_class.h"
#include "matrix_class.h"

POGEL::VECTOR::VECTOR(POGEL::POINT p1, POGEL::POINT p2) {
	this->frompoints(p1, p2);
};

void POGEL::VECTOR::normalize() {
	float len=(float)sqrt(x*x+y*y+z*z);
	x/=len; y/=len; z/=len;
};

POGEL::VECTOR POGEL::VECTOR::normal() {
	return *this/(float)sqrt(x*x+y*y+z*z);
};

float POGEL::VECTOR::getdistance() {
	return (float)sqrt(x*x+y*y+z*z);
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

