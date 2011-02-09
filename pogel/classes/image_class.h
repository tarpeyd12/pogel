#ifndef _IMAGE_CLASS_H
#define _IMAGE_CLASS_H

namespace POGEL {
class IMAGE;
}

//#include "view_class.h"

#include "../pogel_internals.h"

#define				IMAGE_NEAREST		0
#define				IMAGE_LINEAR		1
#define				IMAGE_MIPMAP		2

namespace POGEL {
class IMAGE {
	protected:
		/* properties */
		int filtertype;
		char *data; // the bytes of data in the pixels
		unsigned short int channels; // the number of bytes per pixel
		unsigned int base; // the opengl texture identification number
	public:
		unsigned long sizeX;
		unsigned long sizeY;
		
		/* constructors */
		IMAGE();
		IMAGE(const char*);
		IMAGE(const char*,int);

		/* deconstructor */
		~IMAGE();
		
		/* methods */
		int load(const char*);
		unsigned int build();
		unsigned int loadandbuild(const char*);
		unsigned int getbase() {return base;}
		void set() {
			#ifdef OPENGL
			glBindTexture(GL_TEXTURE_2D, getbase());
			#endif
		}
		unsigned long getheight() {return sizeY;}
		unsigned long getwidth() {return sizeX;}
		void setfilter(int t) { filtertype = t; }
		int getfilter() { return filtertype; }
		
};
}

#endif /* _IMAGE_CLASS_H */
