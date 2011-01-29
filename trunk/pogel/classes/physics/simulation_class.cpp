#include <string.h>

#include "simulation_class.h"

/* Just remember, in here, there is usualy no method to the madenss. */

POGEL::PHYSICS::SIMULATION::SIMULATION() : POGEL::PHYSICS::DYNAMICS() {
	precision=0.01f;
	deactivation = false;
	inactive_index = 25;
	stepstaken = 0;
};

void POGEL::PHYSICS::SIMULATION::increment() {
	float r = (POGEL::hasproperty(POGEL_TIMEBASIS) ? POGEL::GetSecondsPerFrame() : 1);
	
	for(unsigned long a=0;a<numobjects;a++) {
		if(objects[a]->hasOption(PHYSICS_SOLID_VOLITAL) && !objects[a]->hasOption(PHYSICS_SOLID_STATIONARY)) {
			
			if(
				(deactivation && 
				((objects[a]->direction+objects[a]->force).getdistance() <= precision*r || 
				objects[a]->sameposlegacy(precision*r, inactive_index)) && 
				objects[a]->stepstaken > objects[a]->trailsize && 
				stepstaken > 100) ||
				objects[a]->napping()
			) {
				objects[a]->direction = POGEL::VECTOR();
				if(!objects[a]->napping())
					objects[a]->forcegetbounding();
				objects[a]->sleep();
			}
			else {
				objects[a]->direction += getpull(objects[a]);
				objects[a]->wake();
			}
			
			// TODO: get the air friction to work with timing
			float airslowdown = ( ( objects[a]->behavior.air_friction * air_dencity ) / PARTICLE_SLOWDOWN ) + 1.0f;
			objects[a]->spin /= airslowdown;
			objects[a]->direction /= airslowdown;
		}
		//objects[a]->increment();
		objects[a]->clearForce();
	}
	for(int g = 0; g < 1; g++)
	for( unsigned long a = 0; a < numobjects; a++ ) {
		if(!objects[a]->napping())
			for( unsigned long b = 0; b < numobjects; b++ )
				if( a!=b && boundingcheck(objects[a], objects[b]) ) {
					if( processcollision(objects[a], objects[b])) {
						if(objects[a]->callback != NULL) {
							char* n = new char[strlen(objects[b]->getname())+1];
							memset(n, '\0', strlen(objects[b]->getname())+1);
							strcpy(n, objects[b]->getname());
							objects[a]->callback(objects[a], n);
							delete[] n;
						}
						if(objects[b]->callback != NULL) {
							char* n = new char[strlen(objects[a]->getname())+1];
							memset(n, '\0', strlen(objects[a]->getname())+1);
							strcpy(n, objects[a]->getname());
							objects[b]->callback(objects[b], n);
							delete[] n;
						}
						//if(objects[a]->napping()) objects[a]->wake();
						if(objects[b]->napping()) objects[b]->wake();
					}
					
					if(POGEL::hasproperty(POGEL_PAIRS) && b > a) {
						if(objects[a]->hasOption(PHYSICS_SOLID_CONCAVE) && objects[a]->hasOption(PHYSICS_SOLID_SPHERE)) {
							POGEL::VECTOR vr(objects[a]->position, objects[b]->position);
							vr.normalize();
							vr *= objects[a]->bounding.maxdistance;
							vr += objects[a]->position;
							POGEL::LINE(vr.topoint(),objects[b]->position,1,POGEL::COLOR(1,.75,.75,1)).draw();
						}
						else if(objects[b]->hasOption(PHYSICS_SOLID_CONCAVE) && objects[b]->hasOption(PHYSICS_SOLID_SPHERE)) {
							POGEL::VECTOR vr(objects[b]->position, objects[a]->position);
							vr.normalize();
							vr *= objects[b]->bounding.maxdistance;
							vr += objects[b]->position;
							POGEL::LINE(objects[a]->position,vr.topoint(),1,POGEL::COLOR(1,.75,.75,1)).draw();
						}
						else if(objects[a]->hasOption(PHYSICS_SOLID_CONCAVE)) {
							
						}
						else if(objects[b]->hasOption(PHYSICS_SOLID_CONCAVE)) {
							
						}
						else {
							POGEL::LINE(objects[a]->position,objects[b]->position,1,POGEL::COLOR(1,.75,.75,1)).draw();
						}
					}
					
					
				}
	}
	
	for(unsigned long a=0;a<numobjects;a++) {
		if(!objects[a]->napping())
			objects[a]->step();
	}
	
	stepstaken++;
};

