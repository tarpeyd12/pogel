#ifndef _WINDOW_H
#define _WINDOW_H

/* ascii code for the escape key */
#define ESCAPE 27

/* The number of our GLUT window */
extern int window;
extern int screenx, screeny;

void ReSizeGLScene(int Width, int Height);
void keyPressed(unsigned char key, int x, int y);

#endif /* _WINDOW_H */
