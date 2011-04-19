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
		#ifdef OPENGL
		glPushMatrix();
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_LIGHTING);
			//glColor3f(0.0f,1.0f,1.0f);
			color.set();
			glTranslatef(mid.x, mid.y, mid.z);
			// the POGEL::LINE class cannot work here:
			glBegin(GL_LINES);
				glVertex3f(min.x,min.y,min.z); glVertex3f(min.x,min.y,max.z);
				glVertex3f(min.x,max.y,min.z); glVertex3f(min.x,max.y,max.z);
				glVertex3f(max.x,min.y,min.z); glVertex3f(max.x,min.y,max.z);
				glVertex3f(max.x,max.y,min.z); glVertex3f(max.x,max.y,max.z);
				glVertex3f(min.x,min.y,min.z); glVertex3f(max.x,min.y,min.z);
				glVertex3f(min.x,max.y,min.z); glVertex3f(max.x,max.y,min.z);
				glVertex3f(min.x,min.y,max.z); glVertex3f(max.x,min.y,max.z);
				glVertex3f(min.x,max.y,max.z); glVertex3f(max.x,max.y,max.z);
				glVertex3f(min.x,min.y,min.z); glVertex3f(min.x,max.y,min.z);
				glVertex3f(min.x,min.y,max.z); glVertex3f(min.x,max.y,max.z);
				glVertex3f(max.x,min.y,min.z); glVertex3f(max.x,max.y,min.z);
				glVertex3f(max.x,min.y,max.z); glVertex3f(max.x,max.y,max.z);
			glEnd();
			glColor3f(1.0f,1.0f,1.0f);
			glEnable(GL_LIGHTING);
			glEnable(GL_TEXTURE_2D);
		glPopMatrix();
		#endif
	}
};

void POGEL::BOUNDING::draw() {
	#ifdef OPENGL
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	//glColor3f(0.0f,1.0f,1.0f);
	color.set();
	glBegin(GL_QUADS);
		glVertex3f(min.x,min.y,min.z); glVertex3f(max.x,min.y,min.z);
		glVertex3f(max.x,max.y,min.z); glVertex3f(min.x,max.y,min.z);
		glVertex3f(min.x,min.y,max.z); glVertex3f(max.x,min.y,max.z);
		glVertex3f(max.x,max.y,max.z); glVertex3f(min.x,max.y,max.z);
		
		glVertex3f(min.x,min.y,min.z); glVertex3f(max.x,min.y,min.z);
		glVertex3f(max.x,min.y,max.z); glVertex3f(min.x,min.y,max.z);
		glVertex3f(min.x,max.y,min.z); glVertex3f(max.x,max.y,min.z);
		glVertex3f(max.x,max.y,max.z); glVertex3f(min.x,max.y,max.z);
		
		glVertex3f(min.x,min.y,min.z); glVertex3f(min.x,max.y,min.z);
		glVertex3f(min.x,max.y,max.z); glVertex3f(min.x,min.y,max.z);
		glVertex3f(max.x,min.y,min.z); glVertex3f(max.x,max.y,min.z);
		glVertex3f(max.x,max.y,max.z); glVertex3f(max.x,min.y,max.z);
	glEnd();
	glColor3f(1.0f,1.0f,1.0f);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	#endif
};

bool POGEL::BOUNDING::checkbounding(POGEL::POINT thiscenter, POGEL::POINT thatcenter, POGEL::BOUNDING thatbounding) {
	//if(thiscenter.distance(thatcenter) <= (maxdistance+thatbounding.maxdistance)) {
		bool x = false, y = false, z = false;
			x = ( (thatbounding.min.x+thatcenter.x <= max.x+thiscenter.x) && (thatbounding.max.x+thatcenter.x >= min.x+thiscenter.x) );
		if(x) {
			y = ( (thatbounding.min.y+thatcenter.y <= max.y+thiscenter.y) && (thatbounding.max.y+thatcenter.y >= min.y+thiscenter.y) );
		if(y) {
			z = ( (thatbounding.min.z+thatcenter.z <= max.z+thiscenter.z) && (thatbounding.max.z+thatcenter.z >= min.z+thiscenter.z) );
		if(z)
		if(x&&y&&z)
			return true;
		}}
	//}
	return false;
};

bool POGEL::BOUNDING::isinside(POGEL::POINT thiscenter, POGEL::POINT p) {
		bool x = false, y = false, z = false;
			x = ( (p.x <= max.x+thiscenter.x) && (p.x >= min.x+thiscenter.x) );
			y = ( (p.y <= max.y+thiscenter.y) && (p.y >= min.y+thiscenter.y) );
			z = ( (p.z <= max.z+thiscenter.z) && (p.z >= min.z+thiscenter.z) );
		if(x&&y&&z)
			return true;
	return false;
};

bool POGEL::BOUNDING::isoutside(POGEL::POINT thiscenter, POGEL::POINT p) {
		bool x = false, y = false, z = false;
			x = ( (p.x >= max.x+thiscenter.x) || (p.x <= min.x+thiscenter.x) );
			y = ( (p.y >= max.y+thiscenter.y) || (p.y <= min.y+thiscenter.y) );
			z = ( (p.z >= max.z+thiscenter.z) || (p.z <= min.z+thiscenter.z) );
		if(x||y||z)
			return true;
	return false;
};

bool POGEL::BOUNDING::surrounds(POGEL::POINT thiscenter, POGEL::POINT thatcenter, POGEL::BOUNDING thatbounding) {
	return ( isinside(thiscenter, thatbounding.min+thatcenter) && isinside(thiscenter, thatbounding.max+thatcenter) );
};

