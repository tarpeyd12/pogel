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
			{x=a; y=b; z=c;}
		
		void get_values(float *a, float *b, float *c)
			{*a=x; *b=y; *c=z;}
		void set_values(float a, float b, float c)
			{x=a; y=b; z=c;}
		
		void print()
			{POGEL::message(" %7.3f, %7.3f, %7.3f", x, y, z);}
			
		void draw();
		
		void translate()
			{glTranslatef(x,y,z);}
		
		float distance(POGEL::POINT p)
			{return (float)(sqrt( (p.x-x)*(p.x-x) + (p.y-y)*(p.y-y) + (p.z-z)*(p.z-z) )  );}
		
		POGEL::POINT operator*(float a) {
			return POGEL::POINT(x*a, y*a, z*a);
		}
		POGEL::POINT operator/(float a) {
			return POGEL::POINT(x/a, y/a, z/a);
		}
		
		POGEL::POINT operator*(POGEL::POINT p) {
			return POGEL::POINT(x*p.x, y*p.y, z*p.z);
		}
		POGEL::POINT operator/(POGEL::POINT p) {
			return POGEL::POINT(x/p.x, y/p.y, z/p.z);
		}
		
		POGEL::POINT operator+(POGEL::POINT p) {
			return POGEL::POINT(x+p.x, y+p.y, z+p.z);
		}
		POGEL::POINT operator-(POGEL::POINT p) {
			return POGEL::POINT(x-p.x, y-p.y, z-p.z);
		}
		POGEL::POINT& operator=(const POGEL::POINT& p) {
			x=p.x; y=p.y; z=p.z;
			return (*this);
		}
		
		POGEL::POINT& operator*=(const float& a) {
			x*=a; y*=a; z*=a;
			return (*this);
		}
		POGEL::POINT& operator/=(const float& a) {
			x/=a; y/=a; z/=a;
			return (*this);
		}
		
		POGEL::POINT& operator*=(const POGEL::POINT& p) {
			x*=p.z; y*=p.y; z*=p.z;
			return (*this);
		}
		POGEL::POINT& operator/=(const POGEL::POINT& p) {
			x/=p.x; y/=p.y; z/=p.z;
			return (*this);
		}
		
		POGEL::POINT& operator+=(const POGEL::POINT& p) {
			x+=p.x; y+=p.y; z+=p.z;
			return (*this);
		}
		POGEL::POINT& operator-=(const POGEL::POINT& p) {
			x-=p.x; y-=p.y; z-=p.z;
			return (*this);
		}
		
		bool operator==(POGEL::POINT p) {
			if(p.x==x && p.y==y && p.z==z)
				return true;
			return false;
		}
		
		bool operator!=(POGEL::POINT p) {
			return !((*this)==p);
		}
		
		bool isbad() {
			bool ret = (isnan(x) || isnan(y) || isnan(z));
			//if(ret)
				//POGEL::message("(%c, %c, %c)\n", (isnan(x)?'n':' '), (isnan(y)?'n':' '), (isnan(z)?'n':' '));
				//POGEL::fatality(POGEL_FATALITY_NAN_POINT_RETNUM,"%s (%7.3f, %7.3f, %7.3f).",POGEL_FATALITY_NAN_POINT_STRING, x, y, z);
			return ret;
		}
};

class VECTOR: public POGEL::POINT {
	public:
		VECTOR() {x=0.0f; y=0.0f; z=0.0f;}
		VECTOR(float a, float b, float c)
			{x=a; y=b; z=c;}
		VECTOR(POGEL::POINT p)
			{x=p.x; y=p.y; z=p.z;}
		VECTOR(POGEL::POINT,POGEL::POINT);
		
		void normalize();
		POGEL::VECTOR normal();
		float getdistance();
		float getangle(POGEL::VECTOR,POGEL::VECTOR);
		POGEL::VECTOR& dotproduct(POGEL::VECTOR);
		void frompoints(POGEL::POINT,POGEL::POINT);
		
		POGEL::POINT topoint()
			{POGEL::POINT p(x,y,z); return p;}
		
		POGEL::VECTOR operator*(float a) {
			return POGEL::VECTOR(x*a, y*a, z*a);
		}
		POGEL::VECTOR operator/(float a) {
			return POGEL::VECTOR(x/a, y/a, z/a);
		}
		
		POGEL::VECTOR operator*(POGEL::VECTOR p) {
			return POGEL::VECTOR(x*p.x, y*p.y, z*p.z);
		}
		POGEL::VECTOR operator/(POGEL::VECTOR p) {
			return POGEL::VECTOR(x/p.x, y/p.y, z/p.z);
		}
		
		POGEL::VECTOR operator+(POGEL::VECTOR p) {
			return POGEL::VECTOR(x+p.x, y+p.y, z+p.z);
		}
		POGEL::VECTOR operator-(POGEL::VECTOR p) {
			return POGEL::VECTOR(x-p.x, y-p.y, z-p.z);
		}
		POGEL::VECTOR& operator=(const POGEL::VECTOR p) {
			x=p.x; y=p.y; z=p.z;
			return (*this);
		}
		
		POGEL::VECTOR& operator*=(const float& a) {
			x*=a; y*=a; z*=a;
			return (*this);
		}
		POGEL::VECTOR& operator/=(const float& a) {
			x/=a; y/=a; z/=a;
			return (*this);
		}
		
		POGEL::VECTOR& operator*=(const POGEL::VECTOR& p) {
			x*=p.z; y*=p.y; z*=p.z;
			return (*this);
		}
		POGEL::VECTOR& operator/=(const POGEL::VECTOR& p) {
			x/=p.x; y/=p.y; z/=p.z;
			return (*this);
		}
		
		POGEL::VECTOR& operator+=(const POGEL::VECTOR& p) {
			x+=p.x; y+=p.y; z+=p.z;
			return (*this);
		}
		POGEL::VECTOR& operator-=(const POGEL::VECTOR& p) {
			x-=p.x; y-=p.y; z-=p.z;
			return (*this);
		}
		
		bool operator==(POGEL::VECTOR p) {
			if(p.x==x && p.y==y && p.z==z)
				return true;
			return false;
		}
};

class VERTEX: public POGEL::POINT {
	public:
		float u;
		float v;
		POGEL::COLOR color;
		POGEL::VECTOR normal;
		bool usable;
		
		VERTEX() {x=0.0f; y=0.0f; z=0.0f; u=0.0f; v=0.0f; usable=true;}
		VERTEX(float a, float b, float c, float s, float t) 
			{x=a; y=b; z=c; u=s; v=t; usable=true;}
		VERTEX(float a, float b, float c) 
			{x=a; y=b; z=c; u=0.0f; v=0.0f; usable=true;}
		VERTEX(POGEL::POINT p)
			{x=p.x; y=p.y; z=p.z; u=0.0f; v=0.0f; usable=true;}
		
		void get_values(float *a, float *b, float *c, float *s, float *t)
			{*a=x; *b=y; *c=z; *s=u; *t=v;}
		void set_values(float a, float b, float c, float s, float t)
			{x=a; y=b; z=c; u=s; v=t;}
		
		void get_point_values(float *a, float *b, float *c)
			{*a=x; *b=y; *c=z;}
		void set_point_values(float a, float b, float c)
			{x=a; y=b; z=c;}
		
		void get_tex_values(float *s, float *t)
			{*s=u; *t=v;}
		void set_tex_values(float s, float t)
			{u=s; v=t;}
		
		void scroll_tex_values(float s, float t)
			{u+=s; v+=t;}
		
		POGEL::POINT topoint()
			{POGEL::POINT p(x,y,z); return p;}
		
		POGEL::VERTEX operator*(float a) {
			return POGEL::VERTEX(x*a, y*a, z*a);
		}
		POGEL::VERTEX operator/(float a) {
			return POGEL::VERTEX(x/a, y/a, z/a);
		}
		
		POGEL::VERTEX operator*(POGEL::VERTEX p) {
			return POGEL::VERTEX(x*p.x, y*p.y, z*p.z);
		}
		POGEL::VERTEX operator/(POGEL::VERTEX p) {
			return POGEL::VERTEX(x/p.x, y/p.y, z/p.z);
		}
		
		POGEL::VERTEX operator+(POGEL::VERTEX p) {
			return POGEL::VERTEX(x+p.x, y+p.y, z+p.z);
		}
		POGEL::VERTEX operator-(POGEL::VERTEX p) {
			return POGEL::VERTEX(x-p.x, y-p.y, z-p.z);
		}
		POGEL::VERTEX& operator=(const POGEL::VERTEX& p) {
			x=p.x; y=p.y; z=p.z; u=p.u; v=p.v; usable=p.usable; color=p.color; normal=p.normal;
			return (*this);
		}
		
		POGEL::VERTEX& operator*=(const float& a) {
			x*=a; y*=a; z*=a;
			return (*this);
		}
		POGEL::VERTEX& operator/=(const float& a) {
			x/=a; y/=a; z/=a;
			return (*this);
		}
		
		POGEL::VERTEX& operator*=(const POGEL::VERTEX& p) {
			x*=p.z; y*=p.y; z*=p.z;
			return (*this);
		}
		POGEL::VERTEX& operator/=(const POGEL::VERTEX& p) {
			x/=p.x; y/=p.y; z/=p.z;
			return (*this);
		}
		
		POGEL::VERTEX& operator+=(const POGEL::VERTEX& p) {
			x+=p.x; y+=p.y; z+=p.z; usable=p.usable; color=p.color; normal=p.normal;
			return (*this);
		}
		POGEL::VERTEX& operator-=(const POGEL::VERTEX& p) {
			x-=p.x; y-=p.y; z-=p.z; usable=p.usable; color=p.color; normal=p.normal;
			return (*this);
		}
};
}

#endif /* _POINT_CLASS_H */