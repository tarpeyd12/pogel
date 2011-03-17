#ifndef _WINDOW_H
#define _WINDOW_H

#include "pogel/pogel.h"

/* ascii code for the escape key */
#define ESCAPE 27

#define	MOUSE_ROT_FUNC if(mouse_state == GLUT_DOWN && mouse_button == GLUT_LEFT_BUTTON)  camrot = POGEL::POINT( ((float(-1*(mouse_static_y-mouse_y))/float(screeny))*180 + prot.x), ((float(-1*(mouse_static_x-mouse_x))/float(screenx))*360 + prot.y), camrot.z);

/* The number of our GLUT window */
extern int window;
extern int screenx, screeny;
extern bool keys[256];

extern int mouse_x;
extern int mouse_y;
extern int mouse_static_x;
extern int mouse_static_y;
extern int mouse_button;
extern int mouse_state;
extern int prevmousex;
extern int prevmousey;
extern POGEL::POINT prot;


void ReSizeGLScene(int Width, int Height);
void quit();
void keyPressed(unsigned char key, int x, int y);
void keyUnpressed(unsigned char key, int x, int y);
void mouseClicked(int button, int state, int x, int y);
void mouseMoved(int x, int y);

extern void (*exfnc)(void);

#endif /* _WINDOW_H */
