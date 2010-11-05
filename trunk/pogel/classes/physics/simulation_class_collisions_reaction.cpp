#include "simulation_class.h"

/* Just remember, in here, there is usualy no method to the madenss. */

void POGEL::PHYSICS::SIMULATION::reactcollision(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2, POGEL::VECTOR obj1vect, POGEL::VECTOR obj2vect, POGEL::POINT colpoint) {
	POGEL::POINT col = colpoint;
	if(POGEL::hasproperty(POGEL_COLLISIONS))
		POGEL::message("collision between \"%s\" and \"%s\", at <%0.3f,%0.3f,%0.3f>.\n", \
			obj1->getname(), obj2->getname(), colpoint.x, colpoint.y, colpoint.z);
	
	//(tr[0]+tr[1]).print();
	
	//if(!obj1->hasOption(PHYSICS_SOLID_STATIONARY) && !obj2->hasOption(PHYSICS_SOLID_STATIONARY)) {
		POGEL::VECTOR vtmp[2];
		POGEL::PHYSICS::calcInelasticDirections(obj1vect, obj1, obj2, vtmp);
		if(!obj1->hasOption(PHYSICS_SOLID_STATIONARY) && !obj2->hasOption(PHYSICS_SOLID_STATIONARY)) {
			obj1->direction = vtmp[0];
			obj2->direction = vtmp[1];
			
			POGEL::POINT p;
			p = POGEL::MATRIX(POGEL::POINT(),obj1->spin).transformPoint(col-obj1->position);
			obj1->direction -= (POGEL::VECTOR(p)*obj1->spin.getdistance())/PARTICLE_SLOWDOWN;
			//if(obj1->spin.getdistance() != 0.0f)
			//obj1->spin /= ((obj2->behavior.friction/1.0f)+(obj2->behavior.friction >= 0.0f ? 1.0f : -1.0f)); // compensate for friction
			
			p = POGEL::MATRIX(POGEL::POINT(),obj2->spin).transformPoint(col-obj2->position);
			obj2->direction -= (POGEL::VECTOR(p)*obj2->spin.getdistance())/PARTICLE_SLOWDOWN;
			//if(obj2->spin.getdistance() != 0.0f)
			//obj2->spin /= ((obj1->behavior.friction/1.0f)+(obj1->behavior.friction >= 0.0f ? 1.0f : -1.0f)); // compensate for friction
		}
		else if(obj1->hasOption(PHYSICS_SOLID_STATIONARY) && !obj2->hasOption(PHYSICS_SOLID_STATIONARY)) {
			//obj1->direction = vtmp[0];
			if(obj2->direction.getdistance() == 0.0f)
				obj2->direction = vtmp[1] + vtmp[0]*-1;
			else
				obj2->direction = vtmp[1];
			
			POGEL::POINT p = POGEL::MATRIX(POGEL::POINT(),obj2->spin).transformPoint(col-obj2->position);
			obj2->direction -= (POGEL::VECTOR(p)*obj2->spin.getdistance())/PARTICLE_SLOWDOWN;
			//if(obj2->spin.getdistance() != 0.0f)
			//obj2->spin /= ((obj1->behavior.friction/1.0f)+(obj1->behavior.friction >= 0.0f ? 1.0f : -1.0f)); // compensate for friction
		}
		else if(obj2->hasOption(PHYSICS_SOLID_STATIONARY) && !obj1->hasOption(PHYSICS_SOLID_STATIONARY)) {
			if(obj2->direction.getdistance() == 0.0f)
				obj1->direction = vtmp[0] + vtmp[1]*-1;
			else
				obj1->direction = vtmp[0];
			//obj2->direction = vtmp[1];
			
			POGEL::POINT p = POGEL::MATRIX(POGEL::POINT(),obj1->spin).transformPoint(col-obj1->position);
			obj1->direction -= (POGEL::VECTOR(p)*obj1->spin.getdistance())/PARTICLE_SLOWDOWN;
			//if(obj1->spin.getdistance() != 0.0f)
			//obj1->spin /= ((obj2->behavior.friction/1.0f)+(obj2->behavior.friction >= 0.0f ? 1.0f : -1.0f)); // compensate for friction
		}
		return;
	//}
};

bool boundingcheck(POGEL::PHYSICS::SOLID *obj1, POGEL::PHYSICS::SOLID *obj2) {
	// if eather are concave
	if(obj1->hasOption(PHYSICS_SOLID_CONCAVE) || obj2->hasOption(PHYSICS_SOLID_CONCAVE)) {
		// if obj1 is concave general and obj2 is convex sphere
		if(
			(obj1->hasOption(PHYSICS_SOLID_CONCAVE) && !obj1->hasOption(PHYSICS_SOLID_SPHERE) && obj2->hasOption(PHYSICS_SOLID_SPHERE)) ||
			(obj2->hasOption(PHYSICS_SOLID_CONCAVE) && !obj2->hasOption(PHYSICS_SOLID_SPHERE) && obj1->hasOption(PHYSICS_SOLID_SPHERE))
		)
			return true;
		// if obj1 is a concave sphere and obj2 is convex sphere
		else if(obj1->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE) && obj2->hasOption(PHYSICS_SOLID_SPHERE) && !obj2->hasOption(PHYSICS_SOLID_CONCAVE)) {
			if(obj2->position.distance(obj1->position) > fabs(obj2->bounding.maxdistance - obj1->bounding.maxdistance))
				return true;
			return false;
		}
		// reversed
		else
		if(obj2->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE) && obj1->hasOption(PHYSICS_SOLID_SPHERE) && !obj1->hasOption(PHYSICS_SOLID_CONCAVE)) {
			if(obj2->position.distance(obj1->position) > fabs(obj2->bounding.maxdistance - obj1->bounding.maxdistance))
				return true;
			return false;
		}
		// if obj1 is a concave sphere and obj2 is convex
		else
		if(obj1->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE) && !obj2->hasOption(PHYSICS_SOLID_CONCAVE)) {
			if(obj2->position.distance(obj1->position) > fabs(obj2->bounding.maxdistance - obj1->bounding.maxdistance))
				return true;
			return false;
		}
		// reversed
		else
		if(obj2->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE) && !obj1->hasOption(PHYSICS_SOLID_CONCAVE)) {
			if(obj2->position.distance(obj1->position) > fabs(obj2->bounding.maxdistance - obj1->bounding.maxdistance))
				return true;
			return false;
		}
		
		return false;
	}
	// if both are spheres
	else
	if(obj1->hasOption(PHYSICS_SOLID_SPHERE) && obj2->hasOption(PHYSICS_SOLID_SPHERE)) {
		if(obj1->position.distance(obj2->position) < (obj1->bounding.maxdistance + obj2->bounding.maxdistance))
			return true;
		return false;
	}
	
	// if are none of the above
	return obj1->bounding.checkbounding(obj2->bounding);
};


