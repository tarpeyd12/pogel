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
		
		float masses[2];
		masses[0] = obj1->behavior.mass;
		masses[1] = obj2->behavior.mass;
		
		if(obj1->hasOption(PHYSICS_SOLID_STATIONARY) && !obj2->hasOption(PHYSICS_SOLID_STATIONARY)) obj1->behavior.mass = masses[1];
		else
		if(obj2->hasOption(PHYSICS_SOLID_STATIONARY) && !obj1->hasOption(PHYSICS_SOLID_STATIONARY)) obj2->behavior.mass = masses[0];
		
		POGEL::VECTOR vel[] = { obj1->direction, obj2->direction };
		
		POGEL::VECTOR resvel[2];
		POGEL::PHYSICS::calcInelasticDirections(obj1vect, obj1, obj2, vtmp);
		resvel[0] = obj1->direction; resvel[1] = obj2->direction;
		//POGEL::PHYSICS::calcElasticDirections(obj1vect, obj1, obj2, vtmp);
		
		POGEL::MATRIX rmat[] = {
			POGEL::MATRIX(obj1->spin.topoint(), MATRIX_CONSTRUCT_ROTATION), 
			POGEL::MATRIX(obj2->spin.topoint(), MATRIX_CONSTRUCT_ROTATION) 
		};
		POGEL::POINT colp[] = { rmat[0].transformPoint(colpoint), rmat[1].transformPoint(colpoint) };
		POGEL::VECTOR spvel[] = { POGEL::VECTOR(colpoint,colp[0]), POGEL::VECTOR(colpoint,colp[1]) };
		
		obj1->behavior.mass = masses[0];
		obj2->behavior.mass = masses[1];
		
		if(!obj1->hasOption(PHYSICS_SOLID_STATIONARY) && !obj2->hasOption(PHYSICS_SOLID_STATIONARY)) {
			obj1->direction = vtmp[0];
			obj2->direction = vtmp[1];
			
			if(obj1->spin.getdistance() != 0.0f && !spvel[0].isbad() && obj2->direction.getdistance() < spvel[0].getdistance())
				obj2->direction += spvel[0]*obj2->direction.getdistance()*obj1->behavior.friction;
			if(obj2->spin.getdistance() != 0.0f && !spvel[1].isbad() && obj1->direction.getdistance() < spvel[1].getdistance())
				obj1->direction += spvel[1]*obj1->direction.getdistance()*obj2->behavior.friction;
			
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
			if(obj1->direction.getdistance() == 0.0f)
				obj2->direction = vtmp[1] + vtmp[0]*-1;
			else
				obj2->direction = vtmp[1];
			
			if(obj1->spin.getdistance() != 0.0f && !spvel[0].isbad() && obj2->direction.getdistance() < spvel[0].getdistance()) {
				obj2->direction += spvel[0]*obj2->direction.getdistance()*obj1->behavior.friction;
				//obj2->spin = (obj2->spin+obj1->spin)/2;
			}
			
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
			
			if(obj2->spin.getdistance() != 0.0f && !spvel[1].isbad() && obj1->direction.getdistance() < spvel[1].getdistance()) {
				obj1->direction += spvel[1]*obj1->direction.getdistance()*obj2->behavior.friction;
				//obj1->spin = (obj1->spin+obj2->spin)/2;
			}
			
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

