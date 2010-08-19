#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "view_class.h"

POGEL::VIEW::VIEW() : POGEL::IMAGE(){
	settexsize(128,128);
	renderfunc = NULL;
	sceneinit = NULL;
	imgbgcolor = POGEL::COLOR(0.5f,0.5f,0.5f,0.5f);
	setfilter(IMAGE_LINEAR);
};

POGEL::VIEW::~VIEW() {
	renderfunc = NULL;
	sceneinit = NULL;
};

GLuint POGEL::VIEW::build() {
	data = (char*)new GLuint[((sizeX * sizeY)* 3 * sizeof(char))];
	memset(data,'\0',((sizeX * sizeY)* 3 * sizeof(char)));
	
	// Create Texture	
	glGenTextures(1, &base);
	glBindTexture(GL_TEXTURE_2D, base);   // 2d texture (x and y size)
	
	if(getfilter() == IMAGE_MIPMAP) {
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		
		//gluBuild2DMipmaps(GL_TEXTURE_2D, 3, sizeX, sizeY, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else {
		switch(getfilter()) {
			default:
			case IMAGE_NEAREST:
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // scale linearly when image bigger than texture
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // scale linearly when image smaller than texture
			break;
			
			case IMAGE_LINEAR:
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smaller than texture
			break;
		}
		
		// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
		// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
		//glTexImage2D(GL_TEXTURE_2D, 0, 3, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	
	delete data;
	
	return base;
};

void POGEL::VIEW::startrender() {
	//glClearColor(0.5f, 0.0f, 0.5f, 0.5f); // background color of the to be rendered texture
	imgbgcolor.setasbgcolor(); // background color of the to be rendered texture
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
		// Clear The Screen And The Depth Buffer
	glLoadIdentity();
	glViewport(0,0,sizeX,sizeY);					// Set Our Viewport to match texture size
};


GLuint POGEL::VIEW::endrender() {
	glBindTexture(GL_TEXTURE_2D,base);			// Bind To The Texture

	// Copy Our ViewPort To The Texture (From 0,0 To sizeX,sizeY... No Border)
	glCopyTexImage2D(GL_TEXTURE_2D, (getfilter() == IMAGE_MIPMAP ? 3 : 0), GL_RGB, 0, 0, sizeX, sizeY, 0);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	
	glViewport(0, 0, screensizeX ,screensizeY);					// Set Viewport to what it *should* be
	
	return base;
};

