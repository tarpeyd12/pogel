#ifndef _VIEW_CLASS_H
#define _VIEW_CLASS_H

/* the POGEL::VIEW class is an adaptation of the render to texture functions here:
 *  http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=36
 * also contains code from the Simple OpenGL Image Library:
 *  http://www.lonesock.net/soil.html
 */

namespace POGEL {
class VIEW;
}

#include "../pogel_internals.h"
#include "misc_class.h"
#include "image_class.h"

#define			VIEW_SAVE_BMP		1
#define			VIEW_SAVE_TGA		2

namespace POGEL {
class VIEW : public IMAGE {
	private:
		unsigned int properties; // the mushed properties
	protected:
		static int* screensizeX;
		static int* screensizeY;
		
		void (*renderfunc)(void);
		void (*sceneinit)(void);
	public:
		POGEL::COLOR imgbgcolor;
		
		VIEW();
		~VIEW();
		
		void settexsize(unsigned long x, unsigned long y)
			{ sizeX = x, sizeY = y; }
		void setretscreensize(int* x, int* y)
			{ screensizeX = x; screensizeY = y; }
		
		void setrenderfunc(void (*f)(void))
			{ renderfunc = f; }
		void setsceneinitfunc(void (*f)(void))
			{ sceneinit = f; }
		
		void setbgcolor(POGEL::COLOR c)
			{ imgbgcolor = c; }
		
		GLuint build();
		
		void initscene()
			{
				scenesetup();
				if(sceneinit != NULL)
					sceneinit();
			}
		
		GLuint render()
			{
				startrender();
				scene();
				if(renderfunc != NULL)
					renderfunc();
				return endrender();
			}
		
		virtual void scenesetup()
			{ }
		
		virtual void scene()
			{ }
		
		void startrender();
		GLuint endrender();
		
		void save(unsigned int, const char*);
		
		PROPERTIES_METHODS;
		
		
};
}

#endif /* _VIEW_CLASS_H*/
