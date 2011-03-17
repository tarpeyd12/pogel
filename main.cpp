//
// This code was created by Jeff Molofee '99 (ported to Linux/GLUT by Richard Campbell '99)
//
// If you've found this code useful, please let me know.
//
// Visit me at www.demonews.com/hosted/nehe 
// (email Richard Campbell at ulmont@bellsouth.net)
//
#include <GL/glut.h>    // Header File For The GLUT Library 
#include <GL/gl.h>		// Header File For The OpenGL32 Library
#include <GL/glu.h>		// Header File For The GLu32 Library
#include <stdio.h>      // Header file for standard file i/o.
#include <stdlib.h>     // Header file for malloc/free.
#include <unistd.h>     // needed to sleep.
#include <math.h>

#include "scene.h"
#include "window.h"
#include "pogel/pogel.h"

int numcmdargs;
char** cmdargs;

int main(int argc, char **argv) 
{  
    /* Initialize GLUT state - glut will take any command line arguments that pertain to it or 
       X Windows - look at its documentation at http://reality.sgi.com/mjk/spec3/spec3.html */
    glutInit(&argc, argv);
    POGEL::getcmd(argc,argv);
    numcmdargs = argc;
    cmdargs = argv;

    /* Select type of Display mode:   
     Double buffer 
     RGBA color
     Alpha components supported 
     Depth buffer */  
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);  
	
	int sx=800, sy=600;
	
	for(int i = 0; i < argc; i++)
    	if(!strncmp(argv[i], "-winsize=%dx%d", 8)) {
    		sscanf(argv[i], "-winsize=%dx%d", &sx, &sy); break;
    	}
	
    /* get a 640 x 480 window */
    glutInitWindowSize(sx, sy);

    /* the window starts at the upper left corner of the screen */
    glutInitWindowPosition(0, 0);  

    /* Open a window */  
    window = glutCreateWindow("Physical Open Generative Engine Library example(s)");  

    /* Register the function to do all our OpenGL drawing. */
    glutDisplayFunc(&DrawGLScene);  

    /* Go fullscreen.  This is as soon as possible. */
    for(int i = 0; i < argc; i++)
    	if(strlen(argv[i]) == 11 && !strncmp(argv[i], "-fullscreen", 11))
    		glutFullScreen();

    /* Even if there are no events, redraw our gl scene. */
    glutIdleFunc(&DrawGLScene);

    /* Register the function called when our window is resized. */
    glutReshapeFunc(&ReSizeGLScene);

    /* Register the function called when the keyboard is pressed. */
    glutKeyboardFunc(&keyPressed);
    glutKeyboardUpFunc(&keyUnpressed);
    glutMouseFunc(&mouseClicked);
    glutMotionFunc(&mouseMoved);
    glutPassiveMotionFunc(&mouseMoved);

    /* Initialize our window. */
    InitGL(sx, sy);
  
    /* Start Event Processing Engine */  
    glutMainLoop();  

    return 0;
}

