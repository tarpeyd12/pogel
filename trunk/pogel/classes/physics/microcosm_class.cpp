#include "microcosm_class.h"

POGEL::PHYSICS::MICROCOSM::MICROCOSM() : POGEL::PHYSICS::SOLID(), POGEL::PHYSICS::SIMULATION() {
	
};

POGEL::PHYSICS::MICROCOSM::~MICROCOSM() {
	
};

void POGEL::PHYSICS::MICROCOSM::increment() {
	// increment the microcosm as a solid
	POGEL::PHYSICS::SOLID::increment();
	
	// save the current properties
	unsigned int prop = getOptions();
	POGEL::POINT pos = position;
	position = POGEL::POINT();
	POGEL::POINT rot = rotation;
	rotation = POGEL::POINT();
	// makes it that the simulation cannot move the microcosm
	addOption(PHYSICS_SOLID_STATIONARY);
	
	if(hasproperty(MICROCOSM_USE_MASTER_GRAVITY)) {
		POGEL::MATRIX mat = POGEL::MATRIX(getancestorialmatrix().getrotation(), MATRIX_CONSTRUCT_ROTATION);
		POGEL::PHYSICS::DYNAMICS* master = (static_cast<POGEL::PHYSICS::SOLID*>(this->getprogenitor()))->container;
		gravity = mat.transformVector(master->gravity);
	}
	
	// increment the simulation
	POGEL::PHYSICS::SIMULATION::increment();
	
	// reset the properties to what they were
	setOptions(prop);
	position = pos;
	rotation = rot;
}

void POGEL::PHYSICS::MICROCOSM::build() {
	POGEL::PHYSICS::SOLID::build();
	
	if(hasproperty(MICROCOSM_COLLIDE_WITH_PATRONS))
		addSolid(this);
	
	if(hasproperty(MICROCOSM_USE_MASTER_GRAVITY)) {
		POGEL::MATRIX mat = POGEL::MATRIX(getancestorialmatrix().getrotation(), MATRIX_CONSTRUCT_ROTATION);
		POGEL::PHYSICS::DYNAMICS* master = (static_cast<POGEL::PHYSICS::SOLID*>(this->getprogenitor()))->container;
		gravity = mat.transformVector(master->gravity);
	}
};

void POGEL::PHYSICS::MICROCOSM::draw() {
	POGEL::PHYSICS::SOLID::draw();
	for(unsigned long i=0;i<numobjects;i++)
		if(objects[i] != this)
			objects[i]->draw();
}
