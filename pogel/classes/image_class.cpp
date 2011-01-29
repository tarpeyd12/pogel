#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include "image_class.h"

POGEL::IMAGE::IMAGE() {
	data=(char*)NULL;
	base=(GLuint)NULL;
	sizeX=0;
	sizeY=0;
	setfilter(IMAGE_LINEAR);
};

POGEL::IMAGE::IMAGE(const char *filename) {
	//load(filename);
	loadandbuild(filename);
};

POGEL::IMAGE::IMAGE(const char *filename, int filter) {
	setfilter(filter);
	//load(filename);
	loadandbuild(filename);
};

POGEL::IMAGE::~IMAGE() {
	//free(data);
	delete[] data;
	base=(GLuint)NULL;
};

int POGEL::IMAGE::load(const char *filename) {
	FILE *file;
	unsigned long size;                 // size of the image in bytes.
	unsigned long i;                    // standard counter.
	unsigned short int planes;          // number of planes in image (must be 1) 
	unsigned short int bpp;             // number of bits per pixel (must be 24)
	char temp;                          // temporary color storage for bgr-rgb conversion.
	// make sure the file is there.
	if ((file = fopen(filename, "rb"))==NULL)
	{
		POGEL::error("File Not Found : %s\n",filename);
		return false;
	}
	// seek through the bmp header, up to the width/height:
	fseek(file, 18, SEEK_CUR);
	// read the width
	if ((i = fread(&sizeX, 4, 1, file)) != 1) {
		POGEL::error("Error reading width from %s.\n", filename);
		return false;
	}
	POGEL::message("Width of %s: %lu\n", filename, sizeX);
	// read the height 
	if ((i = fread(&sizeY, 4, 1, file)) != 1) {
		POGEL::error("Error reading height from %s.\n", filename);
		return false;
	}
	POGEL::message("Height of %s: %lu\n", filename, sizeY);
	// calculate the size (assuming 24 bits or 3 bytes per pixel).
	size = sizeX * sizeY * 3;

	// read the planes
	if ((fread(&planes, 2, 1, file)) != 1) {
		POGEL::error("Error reading planes from %s.\n", filename);
		return false;
	}
	if (planes != 1) {
		POGEL::error("Planes from %s is not 1: %u\n", filename, planes);
		return false;
	}

	// read the bpp
	if ((i = fread(&bpp, 2, 1, file)) != 1) {
		POGEL::error("Error reading bpp from %s.\n", filename);
		return false;
	}
	if (bpp != 24) {
		POGEL::error("Bpp from %s is not 24: %u\n", filename, bpp);
		return false;
	}
	
	// seek past the rest of the bitmap header.
	fseek(file, 24, SEEK_CUR);

	// read the data. 
	data = (char *) malloc(size);
	if (data == NULL) {
		POGEL::error("Error allocating memory for color-corrected image data");
		return false;	
	}

	if ((i = fread(data, size, 1, file)) != 1) {
		POGEL::error("Error reading image data from %s.\n", filename);
		return false;
	}

	for (i=0;i<size;i+=3) { // reverse all of the colors. (bgr -> rgb)
		temp = data[i];
		data[i] = data[i+2];
		data[i+2] = temp;
	}
    
	// we're done.
	return true;
	
};

GLuint POGEL::IMAGE::build() {
	// Create Texture	
	glGenTextures(1, &base);
	glBindTexture(GL_TEXTURE_2D, base);   // 2d texture (x and y size)
	
	if(getfilter() == IMAGE_MIPMAP) {
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, sizeX, sizeY, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	else {
		switch(getfilter()) {
			default:
			case IMAGE_NEAREST:
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST); // scale linearly when image bigger than texture
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST); // scale linearly when image smalled than textur
			break;
			
			case IMAGE_LINEAR:
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than textur
			break;
		}
		
		// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
		// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
		glTexImage2D(GL_TEXTURE_2D, 0, 3, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	}
	
	return base;
};

GLuint POGEL::IMAGE::loadandbuild(const char *filename) {
	load(filename);
	return build();
	//return getbase();
};

void POGEL::IMAGE::setfilter(int type) {
	filtertype = type;
};

