#include "fountain_class.h"

POGEL::PHYSICS::FOUNTAIN::FOUNTAIN() : POGEL::PHYSICS::DYNAMICS(), POGEL::PHYSICS::SOLID() {
	properties = 0;
};

POGEL::PHYSICS::FOUNTAIN::~FOUNTAIN() {
	
};

POGEL::VECTOR POGEL::PHYSICS::FOUNTAIN::getpull(POGEL::PHYSICS::SOLID* obj) {
	if(hasproperty(FOUNTAIN_USE_MASTER_GRAVITY)) {
		POGEL::MATRIX mat = getancestorialmatrix();
		POGEL::PHYSICS::DYNAMICS* master = (static_cast<POGEL::PHYSICS::SOLID*>(this->getprogenitor()))->container;
		POGEL::POINT pos = obj->position;
		obj->position = mat.transformPoint(pos);
		POGEL::VECTOR pull = mat.transformVector(master->getpull(obj));
		obj->position = pos;
		return pull + POGEL::PHYSICS::DYNAMICS::getpull(obj);
	}
	return POGEL::PHYSICS::DYNAMICS::getpull(obj);
};

void POGEL::PHYSICS::FOUNTAIN::increment() {
	if(hasproperty(FOUNTAIN_IGNORE_PARENT_TRANSLATION)) {
		for(unsigned long i = 0; i < numobjects; i++) {
			objects[i]->translate(direction*-1.0f);
			objects[i]->offsettrail(direction*-1.0f);
		}
	}
	
	// increment the microcosm as a solid
	POGEL::PHYSICS::SOLID::increment();
	
	// increment the simulation
	POGEL::PHYSICS::DYNAMICS::increment();
}

void POGEL::PHYSICS::FOUNTAIN::build() {
	POGEL::PHYSICS::SOLID::build();
};

void POGEL::PHYSICS::FOUNTAIN::draw() {
	#ifdef OPENGL
	if(POGEL::hasproperty(POGEL_ANCESTORY)) {
		POGEL::MATRIX mat(position, rotation);
		for(unsigned long i=0;i<numobjects;i++)
			POGEL::LINE(mat.transformPoint(objects[i]->position), position, 2, POGEL::COLOR(.75,0,.75,1)).draw();
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
	
	POGEL::PHYSICS::DYNAMICS::draw();
	
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
	#endif
};
