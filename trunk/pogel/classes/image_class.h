#ifndef _IMAGE_CLASS_H
#define _IMAGE_CLASS_H

//class POGEL::IMAGE;

#include "../pogel_internals.h"

namespace POGEL {
class IMAGE {
		/* properties */
		char *data; // the bytes of data in the pixels
		unsigned short int channels; // the number of bytes per pixel
		GLuint base; // the opengl texture identification number
	public:
		unsigned long sizeX;
		unsigned long sizeY;
		
		/* constructors */
		IMAGE();
		IMAGE(const char*);

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
};
}

#endif /* _IMAGE_CLASS_H */
