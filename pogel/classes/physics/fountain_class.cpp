#include "fountain_class.h"

POGEL::PHYSICS::FOUNTAIN::FOUNTAIN() : POGEL::PHYSICS::SOLID(), POGEL::PHYSICS::DYNAMICS() {
	
};

POGEL::PHYSICS::FOUNTAIN::~FOUNTAIN() {
	
};

void POGEL::PHYSICS::FOUNTAIN::increment() {
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
	
	if(hasproperty(FOUNTAIN_USE_MASTER_GRAVITY)) {
		POGEL::MATRIX mat = POGEL::MATRIX(getancestorialmatrix().getrotation(), MATRIX_CONSTRUCT_ROTATION);
		POGEL::PHYSICS::DYNAMICS* master = (static_cast<POGEL::PHYSICS::SOLID*>(this->getprogenitor()))->container;
		gravity = mat.transformVector(master->gravity);
	}
	
	// increment the simulation
	POGEL::PHYSICS::DYNAMICS::increment();
	
	// reset the properties to what they were
	setOptions(prop);
	position = pos;
	rotation = rot;
}

void POGEL::PHYSICS::FOUNTAIN::build() {
	POGEL::PHYSICS::SOLID::build();
	
	if(hasproperty(FOUNTAIN_USE_MASTER_GRAVITY)) {
		POGEL::MATRIX mat = POGEL::MATRIX(getancestorialmatrix().getrotation(), MATRIX_CONSTRUCT_ROTATION);
		POGEL::PHYSICS::DYNAMICS* master = (static_cast<POGEL::PHYSICS::SOLID*>(this->getprogenitor()))->container;
		gravity = mat.transformVector(master->gravity);
	}
};

void POGEL::PHYSICS::FOUNTAIN::draw() {
	POGEL::PHYSICS::SOLID::draw();
	POGEL::PHYSICS::DYNAMICS::draw();
};
