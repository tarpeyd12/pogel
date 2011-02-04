#ifndef _MISC_CLASS_H
#define _MISC_CLASS_H

#include <GL/gl.h>

//class POGEL::COLOR;

#include "../pogel_internals.h"

namespace POGEL {
class COLOR {
	public:
		float r, g, b, a;
		
		COLOR() { r=0.0f; g=0.0f; b=0.0f; a=0.0f; }
		COLOR(float red, float green, float blue, float alpha) { r=red; g=green; b=blue; a=alpha; }
		
		void setcolors(float red, float green, float blue, float alpha) { r=red; g=green; b=blue; a=alpha; }
		void getcolors(float *red, float *green, float *blue, float *alpha) { *red=r; *green=g; *blue=b; *alpha=a; }
		void set() { glColor3f(r*a,g*a,b*a); }
		void setasbgcolor() { glClearColor(r, g, b, a); }
		
		/*POGEL::COLOR& operator=(const POGEL::COLOR& c)
			{ r = c.r; g = c.g; b = c.b; return *this; }*/
		
		std::string toString()
		{
			char *sr=POGEL::string("%0.27f",r), *sg=POGEL::string("%0.27f",g), *sb=POGEL::string("%0.27f",b), *sa=POGEL::string("%0.72f",a);
			std::string s = "{["+std::string(sr)+"],["+std::string(sg)+"],["+std::string(sb)+"],["+std::string(sa)+"]}";
			free(sr); free(sg); free(sb); free(sa);
			return s;
		}
};
}

#endif /* _MISC_CLASS_H */
