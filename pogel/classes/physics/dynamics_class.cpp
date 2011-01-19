#include <string.h>

#include "dynamics_class.h"

POGEL::PHYSICS::DYNAMICS::DYNAMICS() {
	numobjects=0;
	objects=(POGEL::PHYSICS::SOLID**)NULL;
	boundingskips = 5;
};

unsigned long POGEL::PHYSICS::DYNAMICS::addSolid(POGEL::PHYSICS::SOLID* obj) {
	//POGEL::PHYSICS::SOLID **tmp=(POGEL::PHYSICS::SOLID**)malloc(sizeof(POGEL::PHYSICS::SOLID*)*(numobjects+1));
	POGEL::PHYSICS::SOLID **tmp = new POGEL::PHYSICS::SOLID*[numobjects+1];
	for(unsigned long i=0;i<numobjects;i++)
		tmp[i]=objects[i];
	
	obj->container = this;
	obj->getbounding();
	
	tmp[numobjects]=obj;
	//for(int i=0;i<numobjects;i++) {
		//free(objects[i]);
	//}
	
	//printf("reallocating solid pointers from %p to: %p\n",objects,tmp);
	
	objectmasses.addsingularity(POGEL::PHYSICS::SINGULARITY(&obj->position, &obj->behavior.mass));
	
	if(obj->behavior.magnetic)
		addproperty(DYNAMICS_HAS_MAGNETIC_OBJECT);
	
	if(objects)
		delete[] objects;
	objects = NULL;
	
	objects=tmp;
	numobjects++;
	return numobjects-1;
};

void POGEL::PHYSICS::DYNAMICS::addSolids(POGEL::PHYSICS::SOLID **obj, unsigned long num) {
	if(obj == (POGEL::PHYSICS::SOLID**)NULL)
		POGEL::fatality(POGEL_FATALITY_NULL_ARRAY_POINTER_RETNUM,"%s to Solid Object(s).",POGEL_FATALITY_NULL_ARRAY_POINTER_STRING);
	for(unsigned long i=0;i<num;i++) {
		if(obj[i] == (POGEL::PHYSICS::SOLID*)NULL)
			POGEL::fatality(POGEL_FATALITY_NULL_OBJECT_POINTER_RETNUM|POGEL_FATALITY_NULL_LIST_POINTER_RETNUM,"%s & %s to Solid Object.",POGEL_FATALITY_NULL_OBJECT_POINTER_STRING, POGEL_FATALITY_NULL_LIST_POINTER_STRING);
		addSolid(obj[i]);
	}
};

POGEL::VECTOR POGEL::PHYSICS::DYNAMICS::getpull(POGEL::PHYSICS::SOLID* obj) {
	POGEL::VECTOR pull;
	POGEL::PHYSICS::GRAVITYCLUSTER pulls;
	
	if(hasproperty(DYNAMICS_HAS_MAGNETIC_OBJECT))
	for(unsigned long a=0;a<numobjects;a++) {
		//pulls.addsingularity(POGEL::PHYSICS::SINGULARITY(objects[a]->position, objects[a]->behavior.mass));
		// the magnetic charge attraction
		if(obj != objects[a] && obj->behavior.magnetic && objects[a]->behavior.magnetic) {
			//pull += POGEL::VECTOR(obj->position,objects[a]->position).normal()*(-obj->behavior.charge-objects[a]->behavior.charge);
			
			if((obj->behavior.charge < 0.0f && objects[a]->behavior.charge > 0.0f) || \
				(obj->behavior.charge > 0.0f && objects[a]->behavior.charge < 0.0f))
				pull += (POGEL::VECTOR(obj->position, objects[a]->position).normal()*(fabs(obj->behavior.charge) + \
					fabs(objects[a]->behavior.charge)))/(objects[a]->position.distance(obj->position));
			
			else if((obj->behavior.charge < 0.0f && objects[a]->behavior.charge < 0.0f) || \
				(obj->behavior.charge > 0.0f && objects[a]->behavior.charge > 0.0f))
				pull += (POGEL::VECTOR(objects[a]->position, obj->position).normal()*(fabs(obj->behavior.charge) + \
					fabs(objects[a]->behavior.charge)))/(objects[a]->position.distance(obj->position));
			
			else
				{}
		}
	}
	
	pull += gusts.getpull(obj->position, obj->behavior.mass);
	pull += singularities.getpull(obj->position, obj->behavior.mass);
	pull += gravity*obj->behavior.mass;
	pull += objectmasses.getpull(obj->position, obj->behavior.mass);
	return pull/PARTICLE_SLOWDOWN*(POGEL::hasproperty(POGEL_TIMEBASIS) ? PARTICLE_SLOWDOWN_RATIO : 1);
};

void POGEL::PHYSICS::DYNAMICS::increment() {
	
	for(unsigned long a=0;a<numobjects;a++) {
		if(objects[a]->hasOption(PHYSICS_SOLID_VOLITAL) && !objects[a]->hasOption(PHYSICS_SOLID_STATIONARY)) {
			objects[a]->direction += getpull(objects[a]);
			
			float airslowdown = ( ( objects[a]->behavior.air_friction * air_dencity ) / PARTICLE_SLOWDOWN ) + 1.0f;
			objects[a]->spin /= airslowdown;
			objects[a]->direction /= airslowdown;
		}
		objects[a]->step();
	}
};

void POGEL::PHYSICS::DYNAMICS::draw() {
	for(unsigned long i=0;i<numobjects;i++)
		objects[i]->draw();
};

void POGEL::PHYSICS::DYNAMICS::drawGravityGrid(float mass, float sps, POGEL::POINT center, unsigned int grd) {
	unsigned long numpoints = grd*grd;
	for(unsigned long i = 0; i < numpoints; i++) {
		POGEL::POINT p(
			((float)(i%grd)*sps)-( (float(grd)*sps)/2.0f-sps/2.0f),
			((float)((i/grd)%grd)*sps)-( (float(grd)*sps)/2.0f-sps/2.0f),
			((float)(i/(grd*grd))*(sps) - sps*float(grd)/2.0f + sps/2.0f)*0
		);
		p*=POGEL::POINT(0,1,1);
		p+=center;
		POGEL::VECTOR v = objectmasses.getpull(p, mass);
		POGEL::COLOR c(1,1,1,v.getdistance()*.75+.25);
		(p+(v.getdistance()>1 ? v.normal() : v)).draw(3, c);
		//POGEL::LINE(p, p+(v.getdistance()>1 ? v.normal() : v), c).draw();
	}
};

