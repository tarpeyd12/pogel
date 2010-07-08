#ifndef _IMAGE_CLASS_H
#define _IMAGE_CLASS_H

//class POGEL::IMAGE;

#include "../pogel_internals.h"

#define				IMAGE_NEAREST		0
#define				IMAGE_LINEAR		1
#define				IMAGE_MIPMAP		2

namespace POGEL {
class IMAGE {
		/* properties */
		int filtertype;
		char *data; // the bytes of data in the pixels
		unsigned short int channels; // the number of bytes per pixel
		GLuint base; // the opengl texture identification number
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
		GLuint build();
		GLuint loadandbuild(const char*);
		GLuint getbase() {return base;}
		void set() {glBindTexture(GL_TEXTURE_2D, getbase());}
		unsigned long getheight() {return sizeY;}
		unsigned long getwidth() {return sizeX;}
		void setfilter(int);
		int getfilter()
			{ return filtertype; }
};
}

#endif /* _IMAGE_CLASS_H */
