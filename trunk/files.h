#ifndef _FILES_H
#define _FILES_H

extern GLuint texture[1];

/* Image type - contains height, width, and data */
struct Image {
	unsigned long sizeX;
	unsigned long sizeY;
	char *data;
};
typedef struct Image Image;

int ImageLoad(char *filename, Image *image);
GLuint LoadGLTexture(char* filename);
void LoadGLTextures();

#endif /* _FILES_H */
