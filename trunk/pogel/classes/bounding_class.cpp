#include "bounding_class.h"

void POGEL::BOUNDING::addpoint(POGEL::POINT middle, POGEL::POINT point) {
	if(isactual) {
		float dist = middle.distance(point);
		if(dist > maxdistance) maxdistance = dist;
	}
	
	if(numpoints==0) {
		if(middle.x <= max.x) max.x = middle.x;
		if(middle.y <= max.y) max.y = middle.y;
		if(middle.z <= max.z) max.z = middle.z;
		
		if(middle.x >= min.x) min.x = middle.x;
		if(middle.y >= min.y) min.y = middle.y;
		if(middle.z >= min.z) min.z = middle.z;
		//max=middle;
		//min=middle;
	}
	
	if(point.x >= max.x/* && point.x >= min.x*/) max.x = point.x;
	if(point.x <= min.x/* && point.x <= max.x*/) min.x = point.x;
	
	if(point.y >= max.y/* && point.y >= min.y*/) max.y = point.y;
	if(point.y <= min.y/* && point.y <= max.y*/) min.y = point.y;
	
	if(point.z >= max.z/* && point.z >= min.z*/) max.z = point.z;
	if(point.z <= min.z/* && point.z <= max.z*/) min.z = point.z;
	
	numpoints++;
};

void POGEL::BOUNDING::fin(float f) {
	max.x+=f;
	max.y+=f;
	max.z+=f;
	
	min.x-=f;
	min.y-=f;
	min.z-=f;
};

void POGEL::BOUNDING::offset(POGEL::POINT offs) {
	max += offs;
	min += offs;
};

void POGEL::BOUNDING::draw(POGEL::POINT mid) {
	if(POGEL::hasproperty(POGEL_BOUNDING)) {
		glPushMatrix();
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			//glColor3f(0.0f,1.0f,1.0f);
			color.set();
			glTranslatef(mid.x, mid.y, mid.z);
			// I don't know if the 'begin' and 'end's slow it down
			glBegin(GL_LINES);
				glVertex3f(min.x,min.y,min.z);
				glVertex3f(min.x,min.y,max.z);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(min.x,max.y,min.z);
				glVertex3f(min.x,max.y,max.z);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(max.x,min.y,min.z);
				glVertex3f(max.x,min.y,max.z);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(max.x,max.y,min.z);
				glVertex3f(max.x,max.y,max.z);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(min.x,min.y,min.z);
				glVertex3f(max.x,min.y,min.z);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(min.x,max.y,min.z);
				glVertex3f(max.x,max.y,min.z);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(min.x,min.y,max.z);
				glVertex3f(max.x,min.y,max.z);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(min.x,max.y,max.z);
				glVertex3f(max.x,max.y,max.z);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(min.x,min.y,min.z);
				glVertex3f(min.x,max.y,min.z);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(min.x,min.y,max.z);
				glVertex3f(min.x,max.y,max.z);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(max.x,min.y,min.z);
				glVertex3f(max.x,max.y,min.z);
			glEnd();
			glBegin(GL_LINES);
				glVertex3f(max.x,min.y,max.z);
				glVertex3f(max.x,max.y,max.z);
			glEnd();
			glColor3f(1.0f,1.0f,1.0f);
			glEnable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
		glPopMatrix();
	}
};

bool POGEL::BOUNDING::checkbounding(POGEL::POINT thiscenter, POGEL::POINT thatcenter, POGEL::BOUNDING thatbounding) {
	if(thiscenter.distance(thatcenter) <= (maxdistance+thatbounding.maxdistance)) {
			bool x = ( (thatbounding.min.x+thatcenter.x <= max.x+thiscenter.x) && (thatbounding.max.x+thatcenter.x >= min.x+thiscenter.x) );
			bool y = ( (thatbounding.min.y+thatcenter.y <= max.y+thiscenter.y) && (thatbounding.max.y+thatcenter.y >= min.y+thiscenter.y) );
			bool z = ( (thatbounding.min.z+thatcenter.z <= max.z+thiscenter.z) && (thatbounding.max.z+thatcenter.z >= min.z+thiscenter.z) );
			if( x && y && z )
				return true;
	}
	return false;
};

