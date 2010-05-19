#include <string.h>

#include "simulation_class.h"

POGEL::PHYSICS::SIMULATION::SIMULATION() : POGEL::PHYSICS::DYNAMICS() {
	precision=0.01f;
	deactivation = false;
};

bool POGEL::PHYSICS::SIMULATION::processcollision(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	
	if(obj1 == obj2)
		return false;
	bool vectcol = false;
	POGEL::TRIANGLE tri;
	POGEL::POINT c1, c2;
	/*if(POGEL::PHYSICS::solid_line_collision(obj1, obj2->position, obj2->position+obj2->direction.normal()*PARTICLE_SLOWDOWN, &tri, &c1, &c2) || POGEL::PHYSICS::solid_line_collision(obj2, obj1->position, obj1->position+obj1->direction.normal()*PARTICLE_SLOWDOWN, &tri, &c1, &c2)) {
		
		if(!obj1->hasOption(PHYSICS_SOLID_STATIONARY)) {
			POGEL::PHYSICS::solid_line_collision(PHYSICS_LINESOLID_COLLISION_GREATEST, obj1, obj1->position, obj1->position+obj1->direction*PARTICLE_SLOWDOWN, &tri, &c1, &c2);
			if(POGEL::PHYSICS::solid_line_collision(PHYSICS_LINESOLID_COLLISION_LEAST, obj2, c2, c2+obj1->direction*PARTICLE_SLOWDOWN, &tri, &c1, &c1))
			{
				POGEL::VECTOR vct(c1, c2);
				obj1->translate(vct);
				vectcol = true;
			}
		}
		
		if(!obj2->hasOption(PHYSICS_SOLID_STATIONARY)) {
			POGEL::PHYSICS::solid_line_collision(PHYSICS_LINESOLID_COLLISION_GREATEST, obj2, obj2->position, obj2->position+obj2->direction*PARTICLE_SLOWDOWN, &tri, &c1, &c2);
			if(POGEL::PHYSICS::solid_line_collision(PHYSICS_LINESOLID_COLLISION_LEAST, obj1, c2, c2+obj2->direction*PARTICLE_SLOWDOWN, &tri, &c1, &c1))
			{
				POGEL::VECTOR vct(c1, c2);
				obj2->translate(vct);
				vectcol = true;
			}
		}
	}*/
	
	
	POGEL::POINT col;
	POGEL::VECTOR vct1, vct2;
	float area;
	
	if(vectcol||POGEL::PHYSICS::solid_collision(obj1, obj2, &col, &vct1, &vct2, &area)) {
		POGEL::message("collision between \"%s\" and \"%s\".\n", obj1->getname(), obj2->getname());
		POGEL::VECTOR tr[2];
		
		unsigned long bup = 0;
		while(POGEL::PHYSICS::solid_collision(obj1, obj2, &col, &vct1, &vct2, &area) && bup < BUPMAX) {
			if(!obj1->hasOption(PHYSICS_SOLID_STATIONARY)) {
				tr[0]=vct2;// + obj1->direction*-1.0f;//-vct1;
				if(POGEL::about(tr[0].getdistance(), 0.0f, precision/obj1->bounding.maxdistance))
					tr[0] += POGEL::VECTOR(col,obj1->position);
				tr[0].normalize();
				obj1->translate(tr[0], precision/obj1->bounding.maxdistance);
				
				/*POGEL::VECTOR pull = getpull(obj1);
				pull*=1.0f;
				POGEL::VECTOR axis = POGEL::TRIANGLE(pull.normal(),POGEL::POINT(),POGEL::VECTOR(obj1->position,col),(POGEL::IMAGE*)NULL,0).normal;
				float angle = POGEL::VECTOR(obj1->position,col).normal().getangle(pull.normal(), axis);*/
				/*if(!((int)(angle/100)==0))
				obj1->rotate(POGEL::VECTOR(POGEL::MATRIX(axis,angle).getrotation()), precision/-10);
				POGEL::message("obj_1 angle = %f\n",angle);*/
			}
			
			if(!obj2->hasOption(PHYSICS_SOLID_STATIONARY)) {
				tr[1]=vct1;// + obj2->direction*-1.0f;//-vct2;
				if(POGEL::about(tr[1].getdistance(), 0.0f, precision/obj2->bounding.maxdistance))
					tr[1] += POGEL::VECTOR(col,obj2->position);
				tr[1].normalize();
				obj2->translate(tr[1], precision/obj2->bounding.maxdistance);
				
				/*POGEL::VECTOR pull = getpull(obj2);
				pull*=1.0f;
				POGEL::VECTOR axis = POGEL::TRIANGLE(pull.normal(),POGEL::POINT(),POGEL::VECTOR(obj2->position,col),(POGEL::IMAGE*)NULL,0).normal;
				float angle = POGEL::VECTOR(obj2->position,col).normal().getangle(pull.normal(), axis);*/
				/*if(!((int)(angle/100)==0))
				obj2->rotate(POGEL::VECTOR(POGEL::MATRIX(axis,angle).getrotation()), precision/-10);
				POGEL::message("obj_2 angle = %f\n",angle);*/
			}
			POGEL::message("bup = %ld\r",bup++);
		}
		
		if(!obj1->hasOption(PHYSICS_SOLID_STATIONARY)) {
			//if(!obj1->samerotlegacy(precision)) {
				
				POGEL::POINT p = POGEL::MATRIX(POGEL::POINT(),obj1->spin).transformPoint(col-obj1->position);
				obj1->direction -= (POGEL::VECTOR(p)*obj1->spin.getdistance())/PARTICLE_SLOWDOWN;
				
				//obj1->spin /= ((obj2->behavior.friction/1.0f)+(obj2->behavior.friction >= 0.0f ? 1.0f : -1.0f)); // compensate for friction
				
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
				POGEL::VECTOR bounce = (tr[0]*(obj2->behavior.bounce*1.0f*0.8f+obj1->behavior.bounce*1.0f*0.8f+obj1->direction.getdistance()*0.8f) + obj1->direction*2.0f);
				//POGEL::VECTOR bounce = tr[0].normal()*obj1->direction.getdistance();
				//obj1->direction /= ((obj2->behavior.friction/1.0f)+(obj2->behavior.friction >= 0.0f ? 1.0f : -1.0f)); // compensate for friction
				obj1->direction += ((bounce )/1.0f)/PARTICLE_SLOWDOWN;
			//}
		}
		if(!obj2->hasOption(PHYSICS_SOLID_STATIONARY)) {
			//if(!obj2->samerotlegacy(precision)) {
				
				POGEL::POINT p = POGEL::MATRIX(POGEL::POINT(),obj2->spin).transformPoint(col-obj2->position);
				obj2->direction -= (POGEL::VECTOR(p)*obj2->spin.getdistance())/PARTICLE_SLOWDOWN;
				
				//obj2->spin /= ((obj1->behavior.friction/1.0f)+(obj1->behavior.friction >= 0.0f ? 1.0f : -1.0f)); // compensate for friction
				
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
				POGEL::VECTOR bounce = (tr[1]*(obj1->behavior.bounce*1.0f*0.8f+obj2->behavior.bounce*1.0f*0.8f+obj1->direction.getdistance()*0.8f) + obj2->direction*2.0f);
				//POGEL::VECTOR bounce = tr[1].normal()*obj2->direction.getdistance();
				//obj2->direction /= ((obj1->behavior.friction/1.0f)+(obj1->behavior.friction >= 0.0f ? 1.0f : -1.0f)); // compensate for friction
				obj2->direction += ((bounce )/1.0f)/PARTICLE_SLOWDOWN;
			//}
		}
		POGEL::VECTOR tmp = obj1->direction;
		if(!obj1->hasOption(PHYSICS_SOLID_STATIONARY))
			obj1->direction += obj2->direction*((obj2->behavior.bounce*1.0f*0.8f+obj1->behavior.bounce*1.0f*0.8f+obj2->direction.getdistance()*0.8f)/PARTICLE_SLOWDOWN);
		if(!obj2->hasOption(PHYSICS_SOLID_STATIONARY))
			obj2->direction += tmp*((obj1->behavior.bounce*1.0f*0.8f+obj2->behavior.bounce*1.0f*0.8f+obj1->direction.getdistance()*0.8f)/PARTICLE_SLOWDOWN);
		
		return true;
	}
	
	return false;
};

void POGEL::PHYSICS::SIMULATION::increment() {
	
	for(unsigned long a=0;a<numobjects;a++) {
		for(unsigned long b=0;b<numobjects;b++) {
			if(a!=b && objects[a]->bounding.checkbounding(objects[b]->bounding) /*&& objects[b]->bounding.checkbounding(objects[b]->position, objects[a]->position, objects[a]->bounding)*/ ) {
				if( processcollision(objects[a], objects[b]) ) {
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
			}
		
		}
	//}
	
	//for(unsigned long a=0;a<numobjects;a++) {
		if(objects[a]->hasOption(PHYSICS_SOLID_VOLITAL) && !objects[a]->hasOption(PHYSICS_SOLID_STATIONARY)) {
			
			if(!objects[a]->sameposlegacy(precision))
				objects[a]->direction += getpull(objects[a]);
			else if(deactivation)
				objects[a]->direction = POGEL::VECTOR();
			
			float airslowdown = ( ( objects[a]->behavior.air_friction * air_dencity ) / PARTICLE_SLOWDOWN ) + 1.0f;
			objects[a]->spin /= airslowdown;
			objects[a]->direction /= airslowdown;
		}
		objects[a]->step();
	}
};

