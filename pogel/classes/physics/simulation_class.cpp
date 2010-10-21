#include <string.h>

#include "simulation_class.h"

/* Just remember, in here, there is usualy no method to the madenss. */

POGEL::PHYSICS::SIMULATION::SIMULATION() : POGEL::PHYSICS::DYNAMICS() {
	precision=0.01f;
	deactivation = false;
};

bool POGEL::PHYSICS::SIMULATION::processcollision(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	// if both are stationary do nothing
	if(obj1 == obj2 || (obj1->hasOption(PHYSICS_SOLID_STATIONARY) && obj2->hasOption(PHYSICS_SOLID_STATIONARY)))
		return false;
	
	// if either are concave do the concave checking
	if(obj1->hasOption(PHYSICS_SOLID_CONCAVE) || obj2->hasOption(PHYSICS_SOLID_CONCAVE)) {
		// concave sphere vs. assumed caonvex sphere
		if(
			(obj1->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE) && obj2->hasOption(PHYSICS_SOLID_SPHERE)) || 
			(obj2->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE) && obj1->hasOption(PHYSICS_SOLID_SPHERE))
		)
			return processCONCAVESPHERE(obj1, obj2);
		// concave sphere vs. assumed convex general shape
		if(
			(obj1->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE)) || 
			(obj2->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE))
		)
			return processCONCAVESPHEREGENERAL(obj1, obj2);
	}
	
	// convex sphere vs. assumed convex general shape
	if(
		(obj1->hasOption(PHYSICS_SOLID_SPHERE) && !obj2->hasOption(PHYSICS_SOLID_SPHERE)) || 
		(obj2->hasOption(PHYSICS_SOLID_SPHERE) && !obj1->hasOption(PHYSICS_SOLID_SPHERE))
	)
		return processSPHEREGENERAL(obj1, obj2);
	
	// convex sphere vs. convex sphere
	else if(obj1->hasOption(PHYSICS_SOLID_SPHERE) && obj2->hasOption(PHYSICS_SOLID_SPHERE))
		return processSPHERE(obj1, obj2);
	
	// convex general vs. convex general
	if(obj1->hasOption(PHYSICS_SOLID_CONVEX) && obj2->hasOption(PHYSICS_SOLID_CONVEX))
		return processCONVEX(obj1, obj2);
	/*if(obj1->hasOption(PHYSICS_SOLID_CONCAVE) && obj2->hasOption(PHYSICS_SOLID_CONCAVE))
		return processCONCAVE(obj1, obj2);*/
		
	// unknown general vs. unknown general
	return processGENERAL(obj1, obj2);
};


/* *************************** Does not work ******************************** */
bool POGEL::PHYSICS::SIMULATION::processCONVEX(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	POGEL::POINT p1, p2, tmp, tmp1, tmp2, obj1_ep, obj2_ep;
	POGEL::TRIANGLE t1, t2, tri;
	obj1->closest(obj2, &p1, &p2, &t1, &t2);
	
	obj1->closest(p2, &tmp1, &tri);
	if(tmp1 == obj1->position)
		POGEL::message("hello 1\n");
	obj2->closest(p1, &tmp2, &tri);
	if(tmp2 == obj2->position)
		POGEL::message("hello 2\n");
	
	//p1.draw();
	//p2.draw();
	//p1.drawto(p2);
	tmp1.drawto(tmp2);
	
	POGEL::PHYSICS::solid_line_collision(PHYSICS_LINESOLID_COLLISION_GREATEST, obj1, obj1->position, p1, &tri, &tmp, &obj1_ep);
	//obj1_ep.draw();
	float dst1 = tmp.z;
	
	POGEL::PHYSICS::solid_line_collision(PHYSICS_LINESOLID_COLLISION_GREATEST, obj2, obj2->position, p2, &tri, &tmp, &obj2_ep);
	//obj2_ep.draw();
	float dst2 = tmp.z;

	//POGEL::message("hello %f, %f\n", (obj1_ep.distance(obj2_ep) + dst1+dst2), obj1->position.distance(obj2->position));
	
	//if( (obj1_ep.distance(obj2_ep) + dst1+dst2) > obj1->position.distance(obj2->position)  ) {
	if(obj1->position.distance(obj2->position) < (dst1+dst2)) {
		POGEL::VECTOR v(p1,p2);
		//if(p1 != obj1->position && p2 != obj2->position && p1 != obj2->position && p2 != obj1->position) {
		//if(p1 != p2) {
			obj1->translate(v.normal() * p1.distance(p2)*-.5);
			obj2->translate(v.normal() * p1.distance(p2)*.5);
		//}
		reactcollision(obj1, obj2, (v.normal()+t1.normal).normal(), t2.normal, (p1+p2)/2.0f);
	
		//if(p1 == obj1->position || p2 == obj2->position)
		return true;
	}
	return false;
};



bool POGEL::PHYSICS::SIMULATION::processGENERAL(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	POGEL::VECTOR vct1, vct2;
	float area;
	POGEL::POINT col;
	if(obj1->bounding.checkbounding(obj2->bounding) && POGEL::PHYSICS::solid_collision(obj1, obj2, &col, &vct1, &vct2, &area)) {
		reactcollision(obj1, obj2, vct1, vct2, col);
		return true;
	}
	
	return false;
};

bool POGEL::PHYSICS::SIMULATION::processSPHERE(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	
	if(obj1->position.distance(obj2->position) < (obj1->bounding.maxdistance + obj2->bounding.maxdistance)) {
		POGEL::VECTOR v(obj1->position, obj2->position);
		POGEL::POINT p = obj1->position + (v.normal() * obj1->bounding.maxdistance).topoint();
		
		if(POGEL::hasproperty(POGEL_COLLISIONS))
			p.draw();
		
		float d = obj1->position.distance(obj2->position) - (obj1->bounding.maxdistance + obj2->bounding.maxdistance);
		
		obj1->translate(v.normal() * d *.5);
		obj2->translate(v.normal() * -d *.5);
		
		reactcollision(obj1, obj2, POGEL::VECTOR(obj1->position, obj2->position).normal(), POGEL::VECTOR(obj2->position, obj1->position).normal(), p);
		return true;
	}
	
	return false;
};

bool POGEL::PHYSICS::SIMULATION::processSPHEREGENERAL(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	
	if(obj2->hasOption(PHYSICS_SOLID_SPHERE) && !obj1->hasOption(PHYSICS_SOLID_SPHERE))
		return processSPHEREGENERAL(obj2, obj1);
	
	POGEL::POINT tmp_1, tmp_2;
	POGEL::TRIANGLE tmptri;
	
	obj2->closest(obj1->position, &tmp_2, &tmptri);
	if(obj1->position.distance(tmp_2) <= (obj1->bounding.maxdistance)) {
		if(POGEL::hasproperty(POGEL_COLLISIONS))
			tmp_2.draw();
		POGEL::VECTOR v = tmptri.normal * (tmptri.isinfront(obj1->position) ? 1 : -1);
		reactcollision(obj1, obj2, POGEL::VECTOR(tmp_2, obj2->position).normal()*1, v.normal()*-1, tmp_2);
		return true;
	}
	
	return false;
};

bool POGEL::PHYSICS::SIMULATION::processCONCAVESPHERE(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	
	if(obj2->hasOption(PHYSICS_SOLID_CONCAVE) && !obj1->hasOption(PHYSICS_SOLID_CONCAVE))
		return processCONCAVESPHERE(obj2, obj1);
	
	if(obj1->position.distance(obj2->position) > fabs(obj2->bounding.maxdistance - obj1->bounding.maxdistance)) {
		POGEL::VECTOR v(obj1->position, obj2->position);
		POGEL::POINT p = obj1->position + (v.normal() * obj1->bounding.maxdistance).topoint();		
		
		if(POGEL::hasproperty(POGEL_COLLISIONS))
			p.draw();
		
		float d = obj1->position.distance(obj2->position) - fabs(obj2->bounding.maxdistance - obj1->bounding.maxdistance);
		
		//if((obj2->position + obj2->direction.normal().topoint()).distance(obj1->position) < obj1->position.distance(obj2->position))
			//obj2->translate(v.normal() * d  );
		//else if((obj2->position - obj2->direction.normal().topoint()).distance(obj1->position) < obj1->position.distance(obj2->position))
			obj2->translate(v.normal() * -d  );
		
		reactcollision(obj1, obj2, POGEL::VECTOR(obj1->position, p).normal(), POGEL::VECTOR(obj2->position, p).normal(), p);
		return true;
	}
	
	return false;
};

bool POGEL::PHYSICS::SIMULATION::processCONCAVESPHEREGENERAL(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	
	if(obj2->hasOption(PHYSICS_SOLID_CONCAVE) && !obj1->hasOption(PHYSICS_SOLID_CONCAVE))
		return processCONCAVESPHEREGENERAL(obj2, obj1);
	
	POGEL::VECTOR v(obj1->position, obj2->position);
	v.normalize();
	v *= obj1->bounding.maxdistance*2;
	POGEL::POINT p = obj1->position + v.topoint();
	
	POGEL::POINT tmp;
	POGEL::TRIANGLE tmptri;
	
	obj2->closest(p, &tmp, &tmptri);
	
	if(obj1->position.distance(tmp) >= obj1->bounding.maxdistance) {
		if(POGEL::hasproperty(POGEL_COLLISIONS))
			p.draw();
		POGEL::VECTOR c = tmptri.normal * (tmptri.isinfront(obj1->position) ? 1 : -1);
		
		//float d = obj1->position.distance(tmp) - obj1->bounding.maxdistance;
		
		//obj2->translate(v.normal() * -d  );
		
		reactcollision(obj1, obj2, (v+c).normal(), (v+c).normal(), p);
		return true;
	}
	
	return false;
};

void POGEL::PHYSICS::SIMULATION::reactcollision(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2, POGEL::VECTOR obj1vect, POGEL::VECTOR obj2vect, POGEL::POINT colpoint) {
	
	if(POGEL::hasproperty(POGEL_COLLISIONS))
		POGEL::message("collision between \"%s\" and \"%s\", at <%0.3f,%0.3f,%0.3f>.\n", \
			obj1->getname(), obj2->getname(), colpoint.x, colpoint.y, colpoint.z);
	
	//(tr[0]+tr[1]).print();
	
	//if(!obj1->hasOption(PHYSICS_SOLID_STATIONARY) && !obj2->hasOption(PHYSICS_SOLID_STATIONARY)) {
		POGEL::VECTOR vtmp[2];
		POGEL::PHYSICS::calcElasticDirections(obj1vect, obj1, obj2, vtmp);
		if(!obj1->hasOption(PHYSICS_SOLID_STATIONARY) && !obj2->hasOption(PHYSICS_SOLID_STATIONARY)) {
			obj1->direction = vtmp[0];
			obj2->direction = vtmp[1];
		}
		else if(obj1->hasOption(PHYSICS_SOLID_STATIONARY) && !obj2->hasOption(PHYSICS_SOLID_STATIONARY)) {
			//obj1->direction = vtmp[0];
			obj2->direction = vtmp[1] + vtmp[0]*-1;
		}
		else if(obj2->hasOption(PHYSICS_SOLID_STATIONARY) && !obj1->hasOption(PHYSICS_SOLID_STATIONARY)) {
			obj1->direction = vtmp[0] + vtmp[1]*-1;
			//obj2->direction = vtmp[1];
		}
		return;
	//}
};

inline bool boundingcheck(POGEL::PHYSICS::SOLID *obj1, POGEL::PHYSICS::SOLID *obj2) {
	
	if(obj1->hasOption(PHYSICS_SOLID_CONCAVE) || obj2->hasOption(PHYSICS_SOLID_CONCAVE)) {
		if(obj1->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE) && obj2->hasOption(PHYSICS_SOLID_SPHERE) && !obj2->hasOption(PHYSICS_SOLID_CONCAVE)) {
			if(obj2->position.distance(obj1->position) > fabs(obj2->bounding.maxdistance - obj1->bounding.maxdistance))
				return true;
			return false;
		}
		else
		if(obj2->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE) && obj1->hasOption(PHYSICS_SOLID_SPHERE) && !obj1->hasOption(PHYSICS_SOLID_CONCAVE)) {
			if(obj2->position.distance(obj1->position) > fabs(obj2->bounding.maxdistance - obj1->bounding.maxdistance))
				return true;
			return false;
		}
		else
		if(obj1->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE) && !obj2->hasOption(PHYSICS_SOLID_CONCAVE)) {
			if(obj2->position.distance(obj1->position) > fabs(obj2->bounding.maxdistance - obj1->bounding.maxdistance))
				return true;
			return false;
		}
		else
		if(obj2->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE) && !obj1->hasOption(PHYSICS_SOLID_CONCAVE)) {
			if(obj2->position.distance(obj1->position) > fabs(obj2->bounding.maxdistance - obj1->bounding.maxdistance))
				return true;
			return false;
		}
		
		return false;
	}
	else
	if(obj1->hasOption(PHYSICS_SOLID_SPHERE) && obj2->hasOption(PHYSICS_SOLID_SPHERE)) {
		if(obj1->position.distance(obj2->position) < (obj1->bounding.maxdistance + obj2->bounding.maxdistance))
			return true;
		return false;
	}
	
	return obj1->bounding.checkbounding(obj2->bounding);
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
		for(unsigned long b=0;b<numobjects;b++) {
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

