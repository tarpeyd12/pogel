#include "simulation_class.h"

/* Just remember, in here, there is usualy no method to the madenss. */

inline void unocupyobjs(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2, POGEL::VECTOR v, float d) {
	v.normalize();
	if(!obj1->hasOption(PHYSICS_SOLID_STATIONARY) && !obj2->hasOption(PHYSICS_SOLID_STATIONARY)) {
		obj1->force += v * d/2;
		obj2->force += v *-d/2;
		obj1->translate(v * d/2);
		obj2->translate(v *-d/2);
	}
	else if(!obj1->hasOption(PHYSICS_SOLID_STATIONARY) && obj2->hasOption(PHYSICS_SOLID_STATIONARY)) {
		obj1->force += v * d/2;
		obj1->translate(v * d);
	}
	else if(obj1->hasOption(PHYSICS_SOLID_STATIONARY) && !obj2->hasOption(PHYSICS_SOLID_STATIONARY)) {
		obj2->force += v *-d/2;
		obj2->translate(v *-d);
	}
};


bool POGEL::PHYSICS::SIMULATION::processcollision(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	// if both are stationary do nothing
	if(obj1 == obj2 || (obj1->hasOption(PHYSICS_SOLID_STATIONARY) && obj2->hasOption(PHYSICS_SOLID_STATIONARY)))
		return false;
	
	// if either are concave do the concave checking
	if(obj1->hasOption(PHYSICS_SOLID_CONCAVE) || obj2->hasOption(PHYSICS_SOLID_CONCAVE)) {
		// concave general shape vs. assumed convex sphere
		if(
			(obj1->hasOption(PHYSICS_SOLID_CONCAVE) && !obj1->hasOption(PHYSICS_SOLID_SPHERE) && obj2->hasOption(PHYSICS_SOLID_SPHERE)) ||
			(obj2->hasOption(PHYSICS_SOLID_CONCAVE) && !obj2->hasOption(PHYSICS_SOLID_SPHERE) && obj1->hasOption(PHYSICS_SOLID_SPHERE))
		)
			return processSPHERE_CONCAVEGENERAL(obj1, obj2);
		// concave sphere vs. assumed convex sphere
		if(
			(obj1->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE) && obj2->hasOption(PHYSICS_SOLID_SPHERE)) || 
			(obj2->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE) && obj1->hasOption(PHYSICS_SOLID_SPHERE))
		)
			return processCONCAVESPHERE_SPHERE(obj1, obj2);
		// concave sphere vs. assumed convex general shape
		if(
			(obj1->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE)) || 
			(obj2->hasOption(PHYSICS_SOLID_SPHERE|PHYSICS_SOLID_CONCAVE))
		)
			return processCONCAVESPHERE_GENERAL(obj1, obj2);
	}
	
	// convex sphere vs. convex general shape
	if(
		(obj1->hasOption(PHYSICS_SOLID_SPHERE) && !obj2->hasOption(PHYSICS_SOLID_SPHERE) && obj2->hasOption(PHYSICS_SOLID_CONVEX)) || 
		(obj2->hasOption(PHYSICS_SOLID_SPHERE) && !obj1->hasOption(PHYSICS_SOLID_SPHERE) && obj1->hasOption(PHYSICS_SOLID_CONVEX))
	)
		return processSPHERE_CONVEXGENERAL(obj1, obj2);
	
	// convex sphere vs. general shape
	else if(
		(obj1->hasOption(PHYSICS_SOLID_SPHERE) && !obj2->hasOption(PHYSICS_SOLID_SPHERE)) || 
		(obj2->hasOption(PHYSICS_SOLID_SPHERE) && !obj1->hasOption(PHYSICS_SOLID_SPHERE))
	)
		return processSPHERE_GENERAL(obj1, obj2);
	
	// convex sphere vs. convex sphere
	else if(obj1->hasOption(PHYSICS_SOLID_SPHERE) && obj2->hasOption(PHYSICS_SOLID_SPHERE))
		return processSPHERE_SPHERE(obj1, obj2);
	
	// convex general vs. convex general
	if(obj1->hasOption(PHYSICS_SOLID_CONVEX) && obj2->hasOption(PHYSICS_SOLID_CONVEX))
		return processCONVEX_CONVEX(obj1, obj2);
	/*if(obj1->hasOption(PHYSICS_SOLID_CONCAVE) && obj2->hasOption(PHYSICS_SOLID_CONCAVE))
		return processCONCAVE_CONCAVE(obj1, obj2);*/
		
	// unknown general vs. unknown general
	return processGENERAL_GENERAL(obj1, obj2);
};


/* *************************** Does not work ******************************** */
bool POGEL::PHYSICS::SIMULATION::processCONVEX_CONVEX(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
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
		unocupyobjs(obj1,obj2,v,p1.distance(p2));
			//obj1->translate(v.normal() * p1.distance(p2)*-.5);
			//obj2->translate(v.normal() * p1.distance(p2)*.5);
		//}
		reactcollision(obj1, obj2, (v.normal()+t1.normal).normal(), t2.normal, (p1+p2)/2.0f);
	
		//if(p1 == obj1->position || p2 == obj2->position)
		return true;
	}
	return false;
};



bool POGEL::PHYSICS::SIMULATION::processGENERAL_GENERAL(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	POGEL::VECTOR vct1, vct2;
	float area;
	POGEL::POINT col;
	if(obj1->bounding.checkbounding(obj2->bounding) && POGEL::PHYSICS::solid_collision(obj1, obj2, &col, &vct1, &vct2, &area)) {
		reactcollision(obj1, obj2, vct1, vct2, col);
		return true;
	}
	
	return false;
};

bool POGEL::PHYSICS::SIMULATION::processSPHERE_SPHERE(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	
	if(obj1->position.distance(obj2->position) < (obj1->bounding.maxdistance + obj2->bounding.maxdistance)) {
		POGEL::VECTOR v(obj1->position, obj2->position);
		POGEL::POINT p = obj1->position + (v.normal() * obj1->bounding.maxdistance).topoint();
		
		if(POGEL::hasproperty(POGEL_COLLISIONS))
			p.draw();
		
		float d = obj1->position.distance(obj2->position) - (obj1->bounding.maxdistance + obj2->bounding.maxdistance);
		
		unocupyobjs(obj1,obj2,v,d);
		
		reactcollision(obj1, obj2, POGEL::VECTOR(obj1->position, obj2->position).normal(), POGEL::VECTOR(obj2->position, obj1->position).normal(), p);
		return true;
	}
	
	return false;
};

bool POGEL::PHYSICS::SIMULATION::processSPHERE_GENERAL(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	// make object 1 the sphere
	if(obj2->hasOption(PHYSICS_SOLID_SPHERE) && !obj1->hasOption(PHYSICS_SOLID_SPHERE))
		return processSPHERE_GENERAL(obj2, obj1);
	
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

bool POGEL::PHYSICS::SIMULATION::processSPHERE_CONVEXGENERAL(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	// make object 1 the sphere
	if(obj2->hasOption(PHYSICS_SOLID_SPHERE) && !obj1->hasOption(PHYSICS_SOLID_SPHERE))
		return processSPHERE_CONVEXGENERAL(obj2, obj1);
	
	POGEL::POINT tmp_1, tmp_2;
	POGEL::TRIANGLE tmptri;
	
	POGEL::POINT c3d, c2d;
	
	obj2->closest(obj1->position, &tmp_2, &tmptri);
	
	POGEL::VECTOR v;
	float d;
	bool inside = false;
	
	if(tmptri.isinfront(obj2->position) == tmptri.isinfront(obj1->position) || tmp_2 == obj2->position) {
		POGEL::PHYSICS::solid_line_collision(PHYSICS_LINESOLID_COLLISION_GREATEST, obj2, obj2->position, obj1->position, &tmptri, &c2d, &tmp_2);
		v = obj1->direction.normal() - obj2->direction.normal();
		d = (obj1->bounding.maxdistance + tmp_2.distance(obj2->position)) - obj1->position.distance(obj2->position);
		unocupyobjs(obj1,obj2,v,d);
		obj2->closest(obj1->position, &tmp_2, &tmptri);
		inside = true;
	}
	
	if((obj1->position.distance(tmp_2) < obj1->bounding.maxdistance && tmptri.isinfront(obj1->position) != tmptri.isinfront(obj2->position)) || inside) {
		if(POGEL::hasproperty(POGEL_COLLISIONS))
			tmp_2.draw();
		v = POGEL::VECTOR(obj1->position, tmp_2).normal();
		d = obj1->position.distance(tmp_2);
		
		unocupyobjs(obj1,obj2,v,d);
		
		if(tmptri.isinfront(v.normal().topoint()+tmptri.middle()))
			unocupyobjs(obj1, obj2, v,  obj1->bounding.maxdistance);
		else
			unocupyobjs(obj1, obj2, v, -obj1->bounding.maxdistance);
			
		v = POGEL::VECTOR(obj1->position,obj2->position).normal()/1 + tmptri.normal*100000;
		v.normalize();
		reactcollision(obj1, obj2, v, v * -1, tmp_2);
		return true;
	}
	
	return false;
};

bool POGEL::PHYSICS::SIMULATION::processSPHERE_CONCAVEGENERAL(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	// make object 1 the sphere
	if(obj2->hasOption(PHYSICS_SOLID_SPHERE) && !obj1->hasOption(PHYSICS_SOLID_SPHERE))
		return processSPHERE_CONCAVEGENERAL(obj2, obj1);
	
	POGEL::POINT tmp_1, tmp_2;
	POGEL::TRIANGLE tmptri;
	
	POGEL::POINT c3d, c2d;
	
	POGEL::PHYSICS::solid_line_collision(PHYSICS_LINESOLID_COLLISION_LEAST, obj2, obj2->position, obj1->position, &tmptri, &c2d, &tmp_2);
	
	POGEL::VECTOR v;
	float d;
	bool inside = false;
	
	if(tmptri.isinfront(obj2->position) != tmptri.isinfront(obj1->position) || obj1->position.distance(obj2->position) > obj2->position.distance(tmp_2) ) {
		v = obj2->position - obj1->position;
		d = obj1->position.distance(obj2->position) - (obj1->bounding.maxdistance + tmp_2.distance(obj2->position));
		unocupyobjs(obj1,obj2,v,d);
		inside = true;
	}
	obj2->closest(obj1->position, &tmp_2, &tmptri);
	
	if((obj1->position.distance(tmp_2) < obj1->bounding.maxdistance && tmptri.isinfront(obj1->position) == tmptri.isinfront(obj2->position) && tmp_2 != obj2->position) || inside) {
		if(POGEL::hasproperty(POGEL_COLLISIONS))
			tmp_2.draw();
		v = POGEL::VECTOR(obj1->position, tmp_2);
		d = obj1->position.distance(tmp_2);
		
		unocupyobjs(obj1,obj2,v,d);
		
		if(tmptri.isinfront(v.normal()+tmptri.middle()))
			unocupyobjs(obj1, obj2, v, -obj1->bounding.maxdistance);
		else
			unocupyobjs(obj1, obj2, v,  obj1->bounding.maxdistance);
		
		v = POGEL::VECTOR(obj1->position,obj2->position).normal()/1 + tmptri.normal*100000;
		v.normalize();
		reactcollision(obj1, obj2, v, v * -1, tmp_2);
		return true;
	}
	
	return false;
};

bool POGEL::PHYSICS::SIMULATION::processCONCAVESPHERE_SPHERE(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	
	if(obj2->hasOption(PHYSICS_SOLID_CONCAVE) && !obj1->hasOption(PHYSICS_SOLID_CONCAVE))
		return processCONCAVESPHERE_SPHERE(obj2, obj1);
	
	if(obj1->position.distance(obj2->position) > fabs(obj2->bounding.maxdistance - obj1->bounding.maxdistance)) {
		POGEL::VECTOR v(obj1->position, obj2->position);
		POGEL::POINT p = obj1->position + (v.normal() * obj1->bounding.maxdistance).topoint();		
		
		if(POGEL::hasproperty(POGEL_COLLISIONS))
			p.draw();
		
		float d = obj1->position.distance(obj2->position) - fabs(obj2->bounding.maxdistance - obj1->bounding.maxdistance);
		
		//if((obj2->position + obj2->direction.normal().topoint()).distance(obj1->position) < obj1->position.distance(obj2->position))
			//obj2->translate(v.normal() * d  );
		//else if((obj2->position - obj2->direction.normal().topoint()).distance(obj1->position) < obj1->position.distance(obj2->position))
		unocupyobjs(obj1,obj2,v,d);
		
		reactcollision(obj1, obj2, POGEL::VECTOR(obj1->position, p).normal()*0, POGEL::VECTOR(obj2->position, p).normal()*0, p);
		return true;
	}
	
	return false;
};

bool POGEL::PHYSICS::SIMULATION::processCONCAVESPHERE_GENERAL(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2) {
	
	if(obj2->hasOption(PHYSICS_SOLID_CONCAVE) && !obj1->hasOption(PHYSICS_SOLID_CONCAVE))
		return processCONCAVESPHERE_GENERAL(obj2, obj1);
	
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

