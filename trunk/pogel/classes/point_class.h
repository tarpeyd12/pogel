#ifndef _POINT_CLASS_H
#define _POINT_CLASS_H

#include <stdio.h>
#include <math.h>

//class POGEL::POINT;
//class POGEL::VECTOR;
//class POGEL::VERTEX;

#include "misc_class.h"
#include "../pogel_internals.h"

namespace POGEL {
class POINT {
	public:
		float x;
		float y;
		float z;
		
		POINT() {x=0.0f; y=0.0f; z=0.0f;}
		POINT(float a, float b, float c)
			{ x=a; y=b; z=c; }
		
		POINT(std::string s) { sscanf(s.c_str(), "{[%f],[%f],[%f]}", &x, &y, &z); }
		
		void get_values(float *a, float *b, float *c) { *a=x; *b=y; *c=z; }
		void set_values(float a, float b, float c)    {  x=a;  y=b;  z=c; }
		
		void print() { POGEL::message(" %7.3f, %7.3f, %7.3f", x, y, z); }
		
		std::string toString() {
			char *sx=POGEL::string("%0.27f",x), *sy=POGEL::string("%0.27f",y), *sz=POGEL::string("%0.27f",z);
			std::string s = "{["+std::string(sx)+"],["+std::string(sy)+"],["+std::string(sz)+"]}";
			free(sx); free(sy); free(sz);
			return s;
		}
			
		void draw();
		void draw(unsigned int);
		void draw(unsigned int, POGEL::COLOR);
		void drawto(POGEL::POINT);
		
		void translate() {glTranslatef(x,y,z);}
		
		float distance(POGEL::POINT p) { return (float)(sqrt( (p.x-x)*(p.x-x) + (p.y-y)*(p.y-y) + (p.z-z)*(p.z-z) )  ); }
		
		POGEL::POINT operator*(float a) { return POGEL::POINT(x*a, y*a, z*a); }
		POGEL::POINT operator/(float a) { return POGEL::POINT(x/a, y/a, z/a); }
		
		POGEL::POINT operator*(POGEL::POINT p) { return POGEL::POINT(x*p.x, y*p.y, z*p.z); }
		POGEL::POINT operator/(POGEL::POINT p) { return POGEL::POINT(x/p.x, y/p.y, z/p.z); }
		
		POGEL::POINT operator+(POGEL::POINT p) { return POGEL::POINT(x+p.x, y+p.y, z+p.z); }
		POGEL::POINT operator-(POGEL::POINT p) { return POGEL::POINT(x-p.x, y-p.y, z-p.z); }
		POGEL::POINT& operator=(const POGEL::POINT& p) { x=p.x; y=p.y; z=p.z; return (*this); }
		
		POGEL::POINT& operator*=(const float& a) { x*=a; y*=a; z*=a; return (*this); }
		POGEL::POINT& operator/=(const float& a) { x/=a; y/=a; z/=a; return (*this); }
		
		POGEL::POINT& operator*=(const POGEL::POINT& p) { x*=p.z; y*=p.y; z*=p.z; return (*this); }
		POGEL::POINT& operator/=(const POGEL::POINT& p) { x/=p.x; y/=p.y; z/=p.z; return (*this); }
		
		POGEL::POINT& operator+=(const POGEL::POINT& p) { x+=p.x; y+=p.y; z+=p.z; return (*this); }
		POGEL::POINT& operator-=(const POGEL::POINT& p) { x-=p.x; y-=p.y; z-=p.z; return (*this); }
		
		bool operator==(POGEL::POINT p) { return (p.x==x && p.y==y && p.z==z); }
		bool operator!=(POGEL::POINT p) { return !((*this)==p); }
		
		bool isbad() { return (isnan(x) || isnan(y) || isnan(z)); }
};

class VECTOR: public POGEL::POINT {
	public:
		VECTOR() {x=0.0f; y=0.0f; z=0.0f;}
		VECTOR(float a, float b, float c) {x=a; y=b; z=c;}
		VECTOR(POGEL::POINT p) {x=p.x; y=p.y; z=p.z;}
		VECTOR(POGEL::POINT,POGEL::POINT);
		
		VECTOR(std::string s) { sscanf(s.c_str(), "{[%f],[%f],[%f]}", &x, &y, &z); }
		
		void normalize();
		POGEL::VECTOR normal();
		float getdistance();
		float getangle(POGEL::VECTOR,POGEL::VECTOR);
		POGEL::VECTOR& dodotproduct(POGEL::VECTOR);
		float dotproduct(POGEL::VECTOR);
		void frompoints(POGEL::POINT,POGEL::POINT);
		
		POGEL::POINT topoint() {POGEL::POINT p(x,y,z); return p;}
		
		POGEL::VECTOR operator*(float a) { return POGEL::VECTOR(x*a, y*a, z*a); }
		POGEL::VECTOR operator/(float a) { return POGEL::VECTOR(x/a, y/a, z/a); }
		
		POGEL::VECTOR operator*(POGEL::VECTOR p) { return POGEL::VECTOR(x*p.x, y*p.y, z*p.z); }
		POGEL::VECTOR operator/(POGEL::VECTOR p) { return POGEL::VECTOR(x/p.x, y/p.y, z/p.z); }
		
		POGEL::VECTOR operator+(POGEL::VECTOR p) { return POGEL::VECTOR(x+p.x, y+p.y, z+p.z); }
		POGEL::VECTOR operator-(POGEL::VECTOR p) { return POGEL::VECTOR(x-p.x, y-p.y, z-p.z); }
		
		POGEL::VECTOR& operator=(const POGEL::VECTOR p) { x=p.x; y=p.y; z=p.z; return (*this); }
		
		POGEL::VECTOR& operator*=(const float& a) { x*=a; y*=a; z*=a; return (*this); }
		POGEL::VECTOR& operator/=(const float& a) { x/=a; y/=a; z/=a; return (*this); }
		
		POGEL::VECTOR& operator*=(const POGEL::VECTOR& p) { x*=p.z; y*=p.y; z*=p.z; return (*this); }
		POGEL::VECTOR& operator/=(const POGEL::VECTOR& p) { x/=p.x; y/=p.y; z/=p.z; return (*this); }
		
		POGEL::VECTOR& operator+=(const POGEL::VECTOR& p) { x+=p.x; y+=p.y; z+=p.z; return (*this); }
		POGEL::VECTOR& operator-=(const POGEL::VECTOR& p) { x-=p.x; y-=p.y; z-=p.z; return (*this); }
		
		bool operator==(POGEL::VECTOR p) { return (p.x==x && p.y==y && p.z==z); }
};

class VERTEX: public POGEL::POINT {
	public:
		float u;
		float v;
		POGEL::COLOR color;
		POGEL::VECTOR normal;
		bool usable;
		
		VERTEX() {x=0.0f; y=0.0f; z=0.0f; u=0.0f; v=0.0f; usable=true;}
		VERTEX(float a, float b, float c, float s, float t) {x=a; y=b; z=c; u=s; v=t; usable=true;}
		VERTEX(float a, float b, float c) {x=a; y=b; z=c; u=0.0f; v=0.0f; usable=true;}
		VERTEX(POGEL::POINT p) {x=p.x; y=p.y; z=p.z; u=0.0f; v=0.0f; usable=true;}
		
		VERTEX(std::string s) {
			std::string n = POGEL::getStringComponentLevel('{','}', s, "0 0");
			std::string c = POGEL::getStringComponentLevel('{','}', s, "0 1");
			std::string fmt = "{[%f],[%f],[%f],[%f],[%f],"+n+","+c+"}";
			sscanf(s.c_str(), fmt.c_str(), &x, &y, &z, &u, &v);
			normal = POGEL::VECTOR(n);
			color = POGEL::COLOR(c);
			usable=true;
		}
		
		std::string toString()
		{
			char *sx=POGEL::string("%0.27f",x), *sy=POGEL::string("%0.27f",y), *sz=POGEL::string("%0.27f",z);
			char *su=POGEL::string("%f",u), *sv=POGEL::string("%f",v);
			std::string s = "{["+std::string(sx)+"],["+std::string(sy)+"],["+std::string(sz)+"],["+std::string(su)+"],["+std::string(sv)+"],"+normal.toString()+","+color.toString()+"}";
			free(sx); free(sy); free(sz); free(su); free(sv);
			return s;
		}
		
		void get_values(float *a, float *b, float *c, float *s, float *t) {*a=x; *b=y; *c=z; *s=u; *t=v;}
		void set_values(float a, float b, float c, float s, float t) {x=a; y=b; z=c; u=s; v=t;}
		
		void get_point_values(float *a, float *b, float *c) {*a=x; *b=y; *c=z;}
		void set_point_values(float a, float b, float c) {x=a; y=b; z=c;}
		
		void get_tex_values(float *s, float *t) {*s=u; *t=v;}
		void set_tex_values(float s, float t) {u=s; v=t;}
		
		void scroll_tex_values(float s, float t) {u+=s; v+=t;}
		
		POGEL::POINT topoint() {POGEL::POINT p(x,y,z); return p;}
		
		POGEL::VERTEX operator*(float a) { return POGEL::VERTEX(x*a, y*a, z*a); }
		POGEL::VERTEX operator/(float a) { return POGEL::VERTEX(x/a, y/a, z/a); }
		
		POGEL::VERTEX operator*(POGEL::VERTEX p) { return POGEL::VERTEX(x*p.x, y*p.y, z*p.z); }
		POGEL::VERTEX operator/(POGEL::VERTEX p) { return POGEL::VERTEX(x/p.x, y/p.y, z/p.z); }
		
		POGEL::VERTEX operator+(POGEL::VERTEX p) { return POGEL::VERTEX(x+p.x, y+p.y, z+p.z); }
		POGEL::VERTEX operator-(POGEL::VERTEX p) { return POGEL::VERTEX(x-p.x, y-p.y, z-p.z); }
		POGEL::VERTEX& operator=(const POGEL::VERTEX& p) { x=p.x; y=p.y; z=p.z; u=p.u; v=p.v; usable=p.usable; color=p.color; normal=p.normal; return (*this); }
		
		POGEL::VERTEX& operator*=(const float& a) { x*=a; y*=a; z*=a; return (*this); }
		POGEL::VERTEX& operator/=(const float& a) { x/=a; y/=a; z/=a; return (*this); }
		
		POGEL::VERTEX& operator*=(const POGEL::VERTEX& p) { x*=p.z; y*=p.y; z*=p.z; return (*this); }
		POGEL::VERTEX& operator/=(const POGEL::VERTEX& p) { x/=p.x; y/=p.y; z/=p.z; return (*this); }
		
		POGEL::VERTEX& operator+=(const POGEL::VERTEX& p) { x+=p.x; y+=p.y; z+=p.z; usable=p.usable; color=p.color; normal=p.normal; return (*this); }
		POGEL::VERTEX& operator-=(const POGEL::VERTEX& p) { x-=p.x; y-=p.y; z-=p.z; usable=p.usable; color=p.color; normal=p.normal; return (*this); }
};
}

#endif /* _POINT_CLASS_H */
