#include <GL/gl.h>
#include <GL/glu.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <memory.h>
#include <assert.h>

#include "view_class.h"

int* POGEL::VIEW::screensizeX;
int* POGEL::VIEW::screensizeY;

POGEL::VIEW::VIEW() : POGEL::IMAGE() {
	settexsize(128,128);
	setviewoffset(0,0);
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
	#ifdef OPENGL
	data = (char*)new GLuint[((sizeX * sizeY)* 4 * sizeof(char))];
	memset(data,'\0',((sizeX * sizeY)* 4 * sizeof(char)));
	
	// Create Texture	
	glGenTextures(1, &base);
	glBindTexture(GL_TEXTURE_2D, base);
	
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
		
		case IMAGE_MIPMAP:
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);	
			glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
		break;
	}
	
	// 2d texture, level of detail 0 (normal), 3 components (red, green, blue), x size from image, y size from image, 
	// border 0 (normal), rgb color data, unsigned byte data, and finally the data itself.
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, sizeX, sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	
	delete data;
	#endif
	return base;
};

void POGEL::VIEW::startrender() {
	#ifdef OPENGL
	imgbgcolor.setasbgcolor(); // background color of the to be rendered texture
	if(!hasproperty(VIEW_NORESET))
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	else
		glClear(GL_DEPTH_BUFFER_BIT);
	glViewport(viewportX,viewportY,sizeX,sizeY);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(float)sizeX/(float)sizeY,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	#endif
};


unsigned int POGEL::VIEW::endrender() {
	#ifdef OPENGL
	if(!hasproperty(VIEW_NORESET)) {
		glBindTexture(GL_TEXTURE_2D,base);			// Bind To The Texture
		
		// Copy Our ViewPort To The Texture (From 0,0 To sizeX,sizeY... No Border)
		glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, viewportX, viewportY, sizeX, sizeY, 0);
		
	
		POGEL::VIEW::resetscreen();
	}
	//else glFlush();
	#endif
	return base;
};

void POGEL::VIEW::resetscreen() {
	#ifdef OPENGL
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
	glLoadIdentity();				// Reset The View
	
	glViewport(0, 0, *screensizeX ,*screensizeY);					// Set Viewport to what it *should* be
	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f,(float)*screensizeX/(float)*screensizeY,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	#endif
};

// the folowing code is from the Simple OpenGL Image Library (SOIL):

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef   signed short  int16;
typedef unsigned int   uint32;
typedef   signed int    int32;
typedef unsigned int   uint;

static void write8(FILE *f, int x) { uint8 z = (uint8) x; fwrite(&z,1,1,f); }

static void writefv(FILE *f, const char *fmt, va_list v) {
   while (*fmt) {
      switch (*fmt++) {
         case ' ': break;
         case '1': { uint8 x = va_arg(v, int); write8(f,x); break; }
         case '2': { int16 x = va_arg(v, int); write8(f,x); write8(f,x>>8); break; }
         case '4': { int32 x = va_arg(v, int); write8(f,x); write8(f,x>>8); write8(f,x>>16); write8(f,x>>24); break; }
         default:
            assert(0);
            va_end(v);
            return;
      }
   }
}

static void writef(FILE *f, const char *fmt, ...)
{
   va_list v;
   va_start(v, fmt);
   writefv(f,fmt,v);
   va_end(v);
}

static void write_pixels(FILE *f, int rgb_dir, int vdir, int x, int y, int comp, void *data, int write_alpha, int scanline_pad)
{
   uint8 bg[3] = { 255, 0, 255}, px[3];
   uint32 zero = 0;
   int i,j,k, j_end;

   if (vdir < 0)	j_end = -1, j = y-1;
   else				j_end =  y, j = 0;

   for (; j != j_end; j += vdir) {
      for (i=0; i < x; ++i) {
         uint8 *d = (uint8 *) data + (j*x+i)*comp;
         if (write_alpha < 0) fwrite(&d[comp-1], 1, 1, f);
         switch (comp) {
            case 1:
            case 2: writef(f, "111", d[0],d[0],d[0]); break;
            case 4:
               if (!write_alpha) {
                  for (k=0; k < 3; ++k) px[k] = bg[k] + ((d[k] - bg[k]) * d[3])/255;
                  writef(f, "111", px[1-rgb_dir],px[1],px[1+rgb_dir]);
                  break;
               }
               /* FALLTHROUGH */
            case 3: writef(f, "111", d[1-rgb_dir],d[1],d[1+rgb_dir]);  break;
         }
         if (write_alpha > 0) fwrite(&d[comp-1], 1, 1, f);
      }
      fwrite(&zero,scanline_pad,1,f);
   }
}

static int outfile(char const *filename, int rgb_dir, int vdir, int x, int y, int comp, void *data, int alpha, int pad, const char *fmt, ...)
{
   FILE *f = fopen(filename, "wb");
   if (f) {
      va_list v;
      va_start(v, fmt);
      writefv(f, fmt, v);
      va_end(v);
      write_pixels(f,rgb_dir,vdir,x,y,comp,data,alpha,pad);
      fclose(f);
   }
   return f != NULL;
}

int stbi_write_bmp(char const *filename, int x, int y, int comp, void *data)
{
   int pad = (-x*3) & 3;
   return outfile(filename,-1,-1,x,y,comp,data,0,pad,
           "11 4 22 4" "4 44 22 444444",
           'B', 'M', 14+40+(x*3+pad)*y, 0,0, 14+40,  // file header
            40, x,y, 1,24, 0,0,0,0,0,0);             // bitmap header
}

int stbi_write_tga(char const *filename, int x, int y, int comp, void *data)
{
   int has_alpha = !(comp & 1);
   return outfile(filename, -1,-1, x, y, comp, data, has_alpha, 0,
                  "111 221 2222 11", 0,0,2, 0,0,0, 0,0,x,y, 24+8*has_alpha, 8*has_alpha);
}

// ending the soil functions

void POGEL::VIEW::save(unsigned int fileformat, const char* filename) {
	/*int width = sizeX;
	int height = sizeY;*/
	#ifdef OPENGL
	int width = *screensizeX;
	int height = *screensizeY;
	
	unsigned char *img_rgb[width * height * 3];  // each pixel is three bytes (RGB)
	glReadPixels(0,0, width,height, GL_RGB, GL_UNSIGNED_BYTE, img_rgb);
	
	switch(fileformat) {
		default:
		case VIEW_SAVE_BMP: stbi_write_bmp(filename, width, height, 3, (void*)img_rgb); break;
		case VIEW_SAVE_TGA: stbi_write_tga(filename, width, height, 3, (void*)img_rgb); break;
	}
	#endif
};

