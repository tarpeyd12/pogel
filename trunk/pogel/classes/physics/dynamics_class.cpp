#include <string.h>

#include "dynamics_class.h"

POGEL::PHYSICS::DYNAMICS::DYNAMICS() {
	numobjects=0;
	//objects=(POGEL::PHYSICS::SOLID**)NULL;
	boundingskips = 5;
	oltmp = NULL;
	ot = NULL;
};

unsigned long POGEL::PHYSICS::DYNAMICS::addSolid(POGEL::PHYSICS::SOLID* obj) {
	if(obj->behavior.magnetic) addproperty(DYNAMICS_HAS_MAGNETIC_OBJECT);
	objectmasses.addsingularity(POGEL::PHYSICS::SINGULARITY(&obj->position, &obj->behavior.mass));
	obj->container = this;
	obj->makebounding();
	objects.add(obj);
	return numobjects++;
};

unsigned long POGEL::PHYSICS::DYNAMICS::addSolidHoldGravity(POGEL::PHYSICS::SOLID* obj) {
	if(obj->behavior.magnetic) addproperty(DYNAMICS_HAS_MAGNETIC_OBJECT);
	obj->container = this;
	obj->makebounding();
	objects.add(obj);
	return numobjects++;
};

void POGEL::PHYSICS::DYNAMICS::addSolidsGravity(POGEL::PHYSICS::SOLID* obj) {
	objectmasses.addsingularity(POGEL::PHYSICS::SINGULARITY(&obj->position, &obj->behavior.mass));
};

void POGEL::PHYSICS::DYNAMICS::addSolids(POGEL::PHYSICS::SOLID **obj, unsigned long num) {
	if(obj == (POGEL::PHYSICS::SOLID**)NULL)
		POGEL::fatality(POGEL_FATALITY_NULL_ARRAY_POINTER_RETNUM,"%s to Solid Object(s).",POGEL_FATALITY_NULL_ARRAY_POINTER_STRING);
	for(unsigned long i=0;i<num;i++)
		if(obj[i] == (POGEL::PHYSICS::SOLID*)NULL)
			POGEL::fatality(POGEL_FATALITY_NULL_OBJECT_POINTER_RETNUM|POGEL_FATALITY_NULL_LIST_POINTER_RETNUM,"%s & %s to Solid Object.",POGEL_FATALITY_NULL_OBJECT_POINTER_STRING, POGEL_FATALITY_NULL_LIST_POINTER_STRING);
		else addSolid(obj[i]);
};

POGEL::PHYSICS::SOLID* POGEL::PHYSICS::DYNAMICS::getSolid(char* n) {
	for(unsigned int i = 0; i < objects.length(); i++)
		if( strlen(n) == strlen(objects[i]->getname()) && !strncmp(objects[i]->getname(), n, strlen(n)) )
			return objects[i];
	return NULL;
};

void POGEL::PHYSICS::DYNAMICS::removeSolid(POGEL::PHYSICS::SOLID* obj) {
	if(obj == NULL) return;
	for(unsigned int i = 0; i < objects.length(); i++)
		if(obj == objects[i]) {
			objects[i]->container = NULL;
			objects.remove(i); objectmasses.remove(i); numobjects--;
			return;
		}
};

void POGEL::PHYSICS::DYNAMICS::removeSolidKeepGravity(POGEL::PHYSICS::SOLID* obj) {
	if(obj == NULL) return;
	for(unsigned int i = 0; i < objects.length(); i++)
		if(obj == objects[i]) {
			objects[i]->container = NULL;
			objects.remove(i); numobjects--;
			return;
		}
};

POGEL::VECTOR POGEL::PHYSICS::DYNAMICS::getpull(POGEL::PHYSICS::SOLID* obj) {
	POGEL::VECTOR pull;
	POGEL::PHYSICS::GRAVITYCLUSTER pulls;
	
	if(hasproperty(DYNAMICS_HAS_MAGNETIC_OBJECT)) // TODO: get the real magnetic physics ...
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
	//pull += objectmasses.getpull(obj->position, obj->behavior.mass);
	pull += ot->getpull(obj->position, obj->behavior.mass);
	return pull/PARTICLE_SLOWDOWN*(POGEL::hasproperty(POGEL_TIMEBASIS) ? PARTICLE_SLOWDOWN_RATIO : 1);
};

void POGEL::PHYSICS::DYNAMICS::increment() {
	buildot();
	
	for(unsigned long a=0;a<numobjects;a++) {
		//POGEL::message("Adding forces to object[%u] in the simulation ...\r", a);
		if(objects[a]->hasOption(PHYSICS_SOLID_VOLITAL) && !objects[a]->hasOption(PHYSICS_SOLID_STATIONARY)) {
			objects[a]->direction += getpull(objects[a]);
			float airslowdown = ( ( objects[a]->behavior.air_friction * air_dencity ) / PARTICLE_SLOWDOWN ) + 1.0f;
			objects[a]->spin /= airslowdown;
			objects[a]->direction /= airslowdown;
		}
		//objects[a]->step();
	}
	for(unsigned long a=0;a<numobjects;a++) objects[a]->step();
	//POGEL::message("\n");
	destroyot();
};

void POGEL::PHYSICS::DYNAMICS::draw() {
	for(unsigned long i = 0; i < numobjects; i++) {
		//POGEL::message("Drawing object[%u] in the simulation ...\r", i);
		objects[i]->draw();
	}
	//POGEL::message("\n");
};

void POGEL::PHYSICS::DYNAMICS::drawGravityGrid(float mass, float sps, POGEL::POINT center, unsigned int grd) {
	unsigned long numpoints = grd*grd*grd;
	POGEL::POINT prev_pv, prev_pv_a[grd], prev_pv_b[grd*grd];
	float pntms = mass*PARTICLE_SLOWDOWN_RATIO;
	for(unsigned long i = 0; i < numpoints; i++) {
		unsigned int imgrd = i%grd;
		POGEL::POINT p(
			(((float)(imgrd)*sps) - ((float(grd)*sps)/2.0f - sps/2.0f) +center.x),
			(((float)((i/grd)%grd)*sps) - ((float(grd)*sps)/2.0f - sps/2.0f) +center.y),
			((float)(i/(grd*grd))*(sps) - sps*float(grd)/2.0f + sps/2.0f +center.z)
		);
		POGEL::VECTOR v = objectmasses.getpull(p, pntms)/(1);
		if(v.getdistance() > sps*2) v = v.normal()*sps*2;
		POGEL::COLOR c(1,.25,0,v.getdistance()/(sps*4));
		POGEL::POINT pv = p+v;
		if(c.a > 0.05 && c.a < sps*4) {
			pv.draw(4, c);
			if(imgrd)
				POGEL::LINE(prev_pv, pv, 2, c).draw();
			if((i/grd)%grd)
				POGEL::LINE(prev_pv_a[imgrd], pv, 2, c).draw();
			if(i/(grd*grd))
				POGEL::LINE(prev_pv_b[i%(grd*grd)], pv, 2, c).draw();
		}
		prev_pv_b[i%(grd*grd)] = prev_pv_a[imgrd] = prev_pv = pv;
	}
};

