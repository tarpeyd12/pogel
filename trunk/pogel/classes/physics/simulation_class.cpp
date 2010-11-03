#include <string.h>

#include "simulation_class.h"

/* Just remember, in here, there is usualy no method to the madenss. */

POGEL::PHYSICS::SIMULATION::SIMULATION() : POGEL::PHYSICS::DYNAMICS() {
	precision=0.01f;
	deactivation = false;
};

void POGEL::PHYSICS::SIMULATION::increment() {
	
	for(unsigned long a=0;a<numobjects;a++) {
		if(objects[a]->hasOption(PHYSICS_SOLID_VOLITAL) && !objects[a]->hasOption(PHYSICS_SOLID_STATIONARY)) {
			
			if(!objects[a]->sameposlegacy(precision))
				objects[a]->direction += getpull(objects[a]);
			else if(deactivation)
				objects[a]->direction = POGEL::VECTOR();
			
			float airslowdown = ( ( objects[a]->behavior.air_friction * air_dencity ) / PARTICLE_SLOWDOWN ) + 1.0f;
			objects[a]->spin /= airslowdown;
			objects[a]->direction /= airslowdown;
		}
		//objects[a]->increment();
	//}
	
	//for(unsigned long a=0;a<numobjects;a++) {
		for(unsigned long b=a+1;b<numobjects;b++) {
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
				}
				
				if(POGEL::hasproperty(POGEL_PAIRS)) {
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_LIGHTING);
					glLineWidth(2);
					glColor3f(1.0f,0.75f,0.75f);
					if(objects[a]->hasOption(PHYSICS_SOLID_CONCAVE) && objects[a]->hasOption(PHYSICS_SOLID_SPHERE)) {
						POGEL::VECTOR vr(objects[a]->position, objects[b]->position);
						vr.normalize();
						vr *= objects[a]->bounding.maxdistance;
						vr += objects[a]->position;
						glBegin(GL_LINES);
							glVertex3f(vr.x,vr.y,vr.z);
							glVertex3f(objects[b]->position.x,objects[b]->position.y,objects[b]->position.z);
						glEnd();
					}
					else if(objects[b]->hasOption(PHYSICS_SOLID_CONCAVE) && objects[b]->hasOption(PHYSICS_SOLID_SPHERE)) {
						POGEL::VECTOR vr(objects[b]->position, objects[a]->position);
						vr.normalize();
						vr *= objects[b]->bounding.maxdistance;
						vr += objects[b]->position;
						glBegin(GL_LINES);
							glVertex3f(objects[a]->position.x,objects[a]->position.y,objects[a]->position.z);
							glVertex3f(vr.x,vr.y,vr.z);
						glEnd();
					}
					else {
						glBegin(GL_LINES);
							glVertex3f(objects[a]->position.x,objects[a]->position.y,objects[a]->position.z);
							glVertex3f(objects[b]->position.x,objects[b]->position.y,objects[b]->position.z);
						glEnd();
					}
					glLineWidth(1);
					glColor3f(1.0f,1.0f,1.0f);
					glEnable(GL_LIGHTING);
					glEnable(GL_TEXTURE_2D);
				}
				
				
			}
		}
	//}
	
	//for(unsigned long a=0;a<numobjects;a++) {
		objects[a]->step();
		objects[a]->clearForce();
	}
};

