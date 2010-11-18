#ifndef _LINE_CLASS_H
#define _LINE_CLASS_H

namespace POGEL {
class LINE;
}

#include "point_class.h"
#include "misc_class.h"
#include "../pogel_internals.h"

namespace POGEL {
class LINE {
	private:
		//unsigned int properties; // the mushed options
	public:
		POGEL::POINT ends[2]; // the 2 ends of the line
		POGEL::COLOR color;
		unsigned int line_width;
		
		bool stippled;
		unsigned int stp_width;
		GLushort patern;
		
		LINE() { 
				ends[0] = POGEL::POINT(); ends[1] = POGEL::POINT();
				line_width = 1;
				color = POGEL::COLOR(1,1,1,1);
				stippled = false; stp_width = 1;
				patern = 0xFFFF;
			}
		
		LINE(POGEL::POINT a, POGEL::POINT b) {
				ends[0] = a; ends[1] = b; 
				line_width = 1;
				color = POGEL::COLOR(1,1,1,1);
				stippled = false; stp_width = 1;
				patern = 0xFFFF;
			}
			
		LINE(POGEL::POINT a, POGEL::POINT b, unsigned int lw) {
				ends[0] = a; ends[1] = b;
				line_width = lw;
				color = POGEL::COLOR(1,1,1,1);
				stippled = false; stp_width = 1;
				patern = 0xFFFF;
			}
			
		LINE(POGEL::POINT a, POGEL::POINT b, POGEL::COLOR c) {
				ends[0] = a; ends[1] = b;
				line_width = 1;
				color = c;
				stippled = false; stp_width = 1;
				patern = 0xFFFF;
			}
			
		LINE(POGEL::POINT a, POGEL::POINT b, unsigned int lw, POGEL::COLOR c) {
				ends[0] = a; ends[1] = b;
				line_width = lw;
				color = c;
				stippled = false; stp_width = 1;
				patern = 0xFFFF;
			}
			
		
		LINE(POGEL::POINT a, POGEL::POINT b, unsigned int lw, POGEL::COLOR c, unsigned int wdth, GLushort pat) {
				ends[0] = a; ends[1] = b;
				line_width = lw;
				color = c;
				stippled = true; stp_width = wdth;
				patern = pat;
			}
		
		POGEL::POINT getStart()
			{ return ends[0]; }
		POGEL::POINT getEnd()
			{ return ends[1]; }
			
		POGEL::POINT getMiddle()
			{ return (getStart()+getEnd())/2; }
		
		//PROPERTIES_METHODS;
		
		void draw()
			{
				glDisable(GL_TEXTURE_2D);
				glDisable(GL_LIGHTING);
				glLineWidth(line_width);
				if(stippled) {
					glEnable(GL_LINE_STIPPLE);
					glLineStipple(stp_width, patern);
				}
				color.set();
				glBegin(GL_LINES);
					glVertex3f(ends[0].x,ends[0].y,ends[0].z);
					glVertex3f(ends[1].x,ends[1].y,ends[1].z);
				glEnd();
				glLineWidth(1);
				//glColor3f(1.0f,1.0f,1.0f);
				POGEL::COLOR(1,1,1,1).set();
				if(stippled) {
					glDisable(GL_LINE_STIPPLE);
					glLineStipple(1, 0xFFFF);
				}
				glEnable(GL_LIGHTING);
				glEnable(GL_TEXTURE_2D);
			}
};
}

#endif /* _LINE_CLASS_H */
