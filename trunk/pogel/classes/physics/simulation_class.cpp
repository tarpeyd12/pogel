#include <string.h>

#include "simulation_class.h"

/* Just remember, in here, there is usualy no method to the madenss. */

POGEL::PHYSICS::SIMULATION::SIMULATION() : POGEL::PHYSICS::DYNAMICS() {
	precision=0.01f;
	deactivation = false;
};

bool POGEL::PHYSICS::SIMULATION::processcollision(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	if(obj1 == obj2 || (obj1->hasOption(PHYSICS_SOLID_STATIONARY) && obj2->hasOption(PHYSICS_SOLID_STATIONARY)))
		return false;
	
	/*if(obj1->hasOption(PHYSICS_SOLID_CONVEX) && obj2->hasOption(PHYSICS_SOLID_CONVEX))
		return processCONVEX(obj1, obj2);
	if(obj1->hasOption(PHYSICS_SOLID_CONCAVE) && obj2->hasOption(PHYSICS_SOLID_CONCAVE))
		return processCONCAVE(obj1, obj2);*/
	if(obj1->hasOption(PHYSICS_SOLID_SPHERE) && obj2->hasOption(PHYSICS_SOLID_SPHERE))
		return processSPHERE(obj1, obj2);
	
	return processGENERAL(obj1, obj2);
};

bool POGEL::PHYSICS::SIMULATION::processGENERAL(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	
	if(obj1->bounding.checkbounding(obj2->bounding)) {
		POGEL::VECTOR vct1, vct2;
		float area;
		POGEL::POINT col;
		POGEL::PHYSICS::solid_collision(obj1, obj2, &col, &vct1, &vct2, &area);
		reactcollision(obj1, obj2, vct1, vct2, col);
		return true;
	}
	
	return false;
};

bool POGEL::PHYSICS::SIMULATION::processSPHERE(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	
	if(obj1->position.distance(obj2->position) < (obj1->bounding.maxdistance + obj2->bounding.maxdistance)) {
		POGEL::VECTOR v(obj1->position, obj2->position);
		POGEL::POINT p = obj1->position + (v * obj1->bounding.maxdistance).topoint();
		reactcollision(obj1, obj2, POGEL::VECTOR(obj1->position, obj2->position), POGEL::VECTOR(obj2->position, obj1->position), p);
		return true;
	}	
	
	return false;
};

void POGEL::PHYSICS::SIMULATION::reactcollision(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2, POGEL::VECTOR obj1vect, POGEL::VECTOR obj2vect, POGEL::POINT colpoint) {
	
	POGEL::VECTOR tr[2];
	tr[0] = obj1vect;
	tr[1] = obj2vect;
	
	if(!obj1->hasOption(PHYSICS_SOLID_STATIONARY)) {
			//if(!obj1->samerotlegacy(precision)) {
				
				POGEL::POINT p = POGEL::MATRIX(POGEL::POINT(),obj1->spin).transformPoint(colpoint-obj1->position);
				obj1->direction -= (POGEL::VECTOR(p)*obj1->spin.getdistance())/PARTICLE_SLOWDOWN;
				
				if(obj1->spin.getdistance() != 0.0f)
				obj1->spin /= ((obj2->behavior.friction/1.0f)+(obj2->behavior.friction >= 0.0f ? 1.0f : -1.0f)); // compensate for friction
				
				/*POGEL::VECTOR pull = getpull(obj1);
				pull*=-1.0f;
				POGEL::VECTOR axis = POGEL::TRIANGLE(pull,POGEL::POINT(),POGEL::VECTOR(obj1->position,col),(POGEL::IMAGE*)NULL,0).normal;
				float angle = POGEL::VECTOR(obj1->position,col).normal().getangle(pull, axis);*/
				/*if(!POGEL::about(angle, 0.0f, precision*100))
				obj1->spin += POGEL::VECTOR(POGEL::POINT(),
				(POGEL::MATRIX(axis,angle).getrotation())*obj1->direction.getdistance()*obj2->behavior.bounce
				)/-PARTICLE_SLOWDOWN;*/
			//}
			//if(!obj1->sameposlegacy(precision)) {
				//POGEL::VECTOR bounce = (tr[0]*(obj2->behavior.bounce*1.0f*0.8f+obj1->behavior.bounce*1.0f*0.8f+obj1->direction.getdistance()*0.8f) + obj1->direction*0.0f);
				POGEL::VECTOR bounce = tr[0].normal()*obj1->direction.getdistance();
				//if(obj1->direction.getdistance() != 0.0f)
				//obj1->force -= tr[1]*((obj2->behavior.friction/1.0f)+(obj2->behavior.friction >= 0.0f ? 1.0f : -1.0f)) + obj1->direction; // compensate for friction
				obj1->force += tr[1].normal()*obj1->direction.getdistance();
				obj1->force -= ((tr[1].normal()*obj1->direction.getdistance())/((obj2->behavior.friction/1.0f)+(obj2->behavior.friction >= 0.0f ? 1.0f : -1.0f)) + obj1->direction)/10;
				obj1->direction /= ((obj2->behavior.friction/1.0f)+(obj2->behavior.friction >= 0.0f ? 1.0f : -1.0f));
				obj1->direction -= ((bounce )/1.0f)/20;
			//}
		}
		if(!obj2->hasOption(PHYSICS_SOLID_STATIONARY)) {
			//if(!obj2->samerotlegacy(precision)) {
				
				POGEL::POINT p = POGEL::MATRIX(POGEL::POINT(),obj2->spin).transformPoint(colpoint-obj2->position);
				obj2->direction -= (POGEL::VECTOR(p)*obj2->spin.getdistance())/PARTICLE_SLOWDOWN;
				
				if(obj2->spin.getdistance() != 0.0f)
				obj2->spin /= ((obj1->behavior.friction/1.0f)+(obj1->behavior.friction >= 0.0f ? 1.0f : -1.0f)); // compensate for friction
				
				/*POGEL::VECTOR pull = getpull(obj2);
				pull*=-1.0f;
				POGEL::VECTOR axis = POGEL::TRIANGLE(pull,POGEL::POINT(),POGEL::VECTOR(obj2->position,col),(POGEL::IMAGE*)NULL,0).normal;
				float angle = POGEL::VECTOR(obj2->position,col).normal().getangle(pull, axis);*/
				/*if(!POGEL::about(angle, 0.0f, precision*100))
				obj2->spin += POGEL::VECTOR(POGEL::POINT(),
				(POGEL::MATRIX(axis,angle).getrotation())*obj2->direction.getdistance()*obj1->behavior.bounce
				)/-PARTICLE_SLOWDOWN;*/
			//}
			//if(!obj2->sameposlegacy(precision)) {
				//POGEL::VECTOR bounce = (tr[1]*(obj1->behavior.bounce*1.0f*0.8f+obj2->behavior.bounce*1.0f*0.8f+obj1->direction.getdistance()*0.8f) + obj2->direction*0.0f);
				POGEL::VECTOR bounce = tr[1].normal()*obj2->direction.getdistance();
				//if(obj2->direction.getdistance() != 0.0f)
				//obj2->force -= tr[0]*((obj1->behavior.friction/1.0f)+(obj1->behavior.friction >= 0.0f ? 1.0f : -1.0f)) + obj2->direction; // compensate for friction
				obj2->force += tr[0].normal()*obj2->direction.getdistance();
				obj2->force -= ((tr[0].normal()*obj2->direction.getdistance())/((obj1->behavior.friction/1.0f)+(obj1->behavior.friction >= 0.0f ? 1.0f : -1.0f)) + obj2->direction)/10;
				obj2->direction /= ((obj1->behavior.friction/1.0f)+(obj1->behavior.friction >= 0.0f ? 1.0f : -1.0f));
				obj2->direction -= ((bounce )/1.0f)/20;
			//}
		}
	
	
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
		//objects[a]->step();
	}
	
	for(unsigned long a=0;a<numobjects;a++) {
		for(unsigned long b=a+1;b<numobjects;b++) {
			if(a!=b /*&& objects[a]->bounding.checkbounding(objects[b]->bounding) && objects[b]->bounding.checkbounding(objects[b]->position, objects[a]->position, objects[a]->bounding)*/ ) {
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
					glBegin(GL_LINES);
						glVertex3f(objects[a]->position.x,objects[a]->position.y,objects[a]->position.z);
						glVertex3f(objects[b]->position.x,objects[b]->position.y,objects[b]->position.z);
					glEnd();
					glLineWidth(1);
					glColor3f(1.0f,1.0f,1.0f);
					glEnable(GL_LIGHTING);
					glEnable(GL_TEXTURE_2D);
				}
				
				
			}
			/*if(objects[a]->position.distance(objects[b]->position) <= 1){
			POGEL::POINT tmp_1, tmp_2;
		POGEL::TRIANGLE tmptri;
		objects[a]->closest(objects[b], &tmp_1, &tmp_2, &tmptri, &tmptri);
		
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glLineWidth(2);
		glColor3f(0.0f,0.75f,0.75f);
		glBegin(GL_LINES);
			glVertex3f(tmp_1.x,tmp_1.y,tmp_1.z);
			glVertex3f(tmp_2.x,tmp_2.y,tmp_2.z);
		glEnd();
		glLineWidth(1);
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		}*/
		}
	}
	
	for(unsigned long a=0;a<numobjects;a++) objects[a]->step();
};

