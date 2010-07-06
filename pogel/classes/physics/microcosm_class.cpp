#include "microcosm_class.h"

POGEL::PHYSICS::MICROCOSM::MICROCOSM() : POGEL::PHYSICS::SOLID(), POGEL::PHYSICS::SIMULATION() {
	properties = 0;
};

POGEL::PHYSICS::MICROCOSM::~MICROCOSM() {
	
};

POGEL::VECTOR POGEL::PHYSICS::MICROCOSM::getpull(POGEL::PHYSICS::SOLID* obj) {
	if(hasproperty(MICROCOSM_USE_MASTER_GRAVITY)) {
		POGEL::MATRIX mat = getancestorialmatrix();
		POGEL::PHYSICS::DYNAMICS* master = (static_cast<POGEL::PHYSICS::SOLID*>(this->getprogenitor()))->container;
		POGEL::POINT pos = obj->position;
		obj->position = mat.transformPoint(pos);
		POGEL::VECTOR pull = mat.transformVector(master->getpull(obj));
		obj->position = pos;
		return pull + POGEL::PHYSICS::SIMULATION::getpull(obj);
	}
	return POGEL::PHYSICS::SIMULATION::getpull(obj);
};

void POGEL::PHYSICS::MICROCOSM::increment() {
	// increment the microcosm as a solid
	POGEL::PHYSICS::SOLID::increment();
	
	// increment the simulation
	POGEL::PHYSICS::SIMULATION::increment();
	
	// save the current properties
	unsigned int prop = getOptions();
	POGEL::POINT pos = position;
	position = POGEL::POINT();
	POGEL::POINT rot = rotation;
	rotation = POGEL::POINT();
	// makes it that the simulation cannot move the microcosm
	addOption(PHYSICS_SOLID_STATIONARY);
	
	if(hasproperty(MICROCOSM_COLLIDE_WITH_PATRONS))
		for(unsigned long i = 0; i < numobjects; i++)
			processcollision(this, objects[i]);
	
	// reset the properties to what they were
	setOptions(prop);
	position = pos;
	rotation = rot;
}

void POGEL::PHYSICS::MICROCOSM::build() {
	POGEL::PHYSICS::SOLID::build();
};

void POGEL::PHYSICS::MICROCOSM::draw() {
	
	if(POGEL::hasproperty(POGEL_ANCESTORY)) {
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glLineWidth(2);
		glColor3f(0.75f,0.0f,0.75f);
		for(unsigned long i=0;i<numobjects;i++) {
			POGEL::POINT p = POGEL::MATRIX(position, rotation).transformPoint(objects[i]->position);
			glBegin(GL_LINES);
				glVertex3f(p.x,p.y,p.z);
				glVertex3f(position.x,position.y,position.z);
			glEnd();
		}
		glLineWidth(1);
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	}
	
	POGEL::PHYSICS::SOLID::draw();
	
	#ifdef OBJECT_USE_OPNEGL_MATRIX_RECURSION
		glPushMatrix();
	#endif /* OBJECT_USE_OPNEGL_MATRIX_RECURSION */
	glTranslatef(position.x, position.y, position.z);
	if(hasproperty(OBJECT_ROTATE_XYZ)) {
		glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
		glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
		glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
	}
	else {
		glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
		glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
		glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
	}
	
	
	POGEL::PHYSICS::SIMULATION::draw();
	/*for(unsigned long i=0;i<numobjects;i++)
		//if(objects[i] != this)
			objects[i]->draw();*/
	
	#ifdef OBJECT_USE_OPNEGL_MATRIX_RECURSION
		glPopMatrix();
	#else
	if(OBJECT_ROTATE_XYZ & properties) {
		glRotatef(rotation.z, 0.0f, 0.0f, -1.0f);
		glRotatef(rotation.y, 0.0f, -1.0f, 0.0f);
		glRotatef(rotation.x, -1.0f, 0.0f, 0.0f);
	}
	else {
		glRotatef(rotation.x, -1.0f, 0.0f, 0.0f);
		glRotatef(rotation.y, 0.0f, -1.0f, 0.0f);
		glRotatef(rotation.z, 0.0f, 0.0f, -1.0f);
	}
	glTranslatef(-position.x, -position.y, -position.z);
	#endif /* OBJECT_USE_OPNEGL_MATRIX_RECURSION */
}
