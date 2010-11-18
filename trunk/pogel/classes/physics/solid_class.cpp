#include "../../pogel.h"
#include "../../pogel_internals.h"
#include "solid_class.h"

POGEL::PHYSICS::SOLID::SOLID() : POGEL::OBJECT() {
	behavior = POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES();
	physproperties = 0;
	maximumdistance = 0.0f;
	stepstaken = 0;
	objboundingskips = 0;
	stepsatboundingcheck = 0;
	bounding = POGEL::BOUNDING(BOUNDING_OBJECT);
	trail = new POGEL::POINT[PHYSICS_SOLID_TRAILSIZE];
	rots = new POGEL::POINT[PHYSICS_SOLID_TRAILSIZE];
	trailsize = PHYSICS_SOLID_TRAILSIZE;
	
	for( int i = 0; i < PHYSICS_SOLID_TRAILSIZE; i++ ) {
		trail[i] = POGEL::POINT();
		rots[i] = POGEL::POINT();
	}
	
	container = NULL;
	
	callback = NULL;
	function = NULL;
};

POGEL::PHYSICS::SOLID::SOLID(POGEL::OBJECT* obj, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES attr, unsigned int prop) : POGEL::OBJECT(obj) {
	behavior = attr;
	physproperties = prop;
	maximumdistance = 0.0f;
	stepstaken = 0;
	objboundingskips = 0;
	stepsatboundingcheck = 0;
	bounding = POGEL::BOUNDING(BOUNDING_OBJECT);
	//position=obj->position;
	//rotation=obj->rotation;
	trail = new POGEL::POINT[PHYSICS_SOLID_TRAILSIZE];
	rots = new POGEL::POINT[PHYSICS_SOLID_TRAILSIZE];
	trailsize = PHYSICS_SOLID_TRAILSIZE;
	
	for( int i = 0; i < PHYSICS_SOLID_TRAILSIZE; i++ ) {
		trail[i] = POGEL::POINT();
		rots[i] = POGEL::POINT();
	}
	
	container = NULL;
	
	callback = NULL;
	function = NULL;
};

POGEL::PHYSICS::SOLID::~SOLID() {
	if(trail!=NULL) delete trail;
	if(rots!=NULL) delete rots;
};

void POGEL::PHYSICS::SOLID::resizetrail(unsigned long size) {
	if(trail!=NULL) delete trail;
	if(rots!=NULL) delete rots;
	trailsize = size;
	trail = new POGEL::POINT[size];
	rots = new POGEL::POINT[size];
	for( unsigned long i = 0; i < trailsize; i++ ) {
		trail[i] = position*PARTICLE_SLOWDOWN_RATIO;
		rots[i] = rotation*PARTICLE_SLOWDOWN_RATIO;
	}
};

void POGEL::PHYSICS::SOLID::steppostrail() {
	trail[0] = position;
	for(unsigned long i = trailsize-1;i>0;i--) trail[i] = trail[i-1];
};

void POGEL::PHYSICS::SOLID::steprottrail() {
	rots[0] = rotation;
	for(unsigned long i = trailsize-1;i>0;i--) rots[i] = rots[i-1];
};

void POGEL::PHYSICS::SOLID::steptrail() {
	trail[0] = position;
	rots[0] = rotation;
	for(unsigned long i = trailsize-1;i>0;i--) {
		trail[i] = trail[i-1];
		rots[i] = rots[i-1];
	}
};

bool POGEL::PHYSICS::SOLID::sameposlegacy(float pres) {
	bool ret = false;
	bool legacy = true;
	if(trailsize < PHYSICS_SOLID_TRAILINDEX)
		return (POGEL::about(position.x, trail[trailsize-1].x, pres) && POGEL::about(position.y, trail[trailsize-1].y, pres) && POGEL::about(position.z, trail[trailsize-1].z, pres));
	if(POGEL::frames<(trailsize/PHYSICS_SOLID_TRAILINDEX))
		return false;
	for(unsigned long i=0;i<trailsize && i<stepstaken+1;i+=(trailsize/PHYSICS_SOLID_TRAILINDEX))
		//ret = legacy && (POGEL::about(position.x, trail[i].x, pres) && POGEL::about(position.y, trail[i].y, pres) && POGEL::about(position.z, trail[i].z, pres));
		if(legacy && (POGEL::about(position.x, trail[i].x, pres) && POGEL::about(position.y, trail[i].y, pres) && POGEL::about(position.z, trail[i].z, pres)))
			ret = true;
		else {
			ret = false;
			legacy = false;
			return false;
		}
	return ret;
};

bool POGEL::PHYSICS::SOLID::samerotlegacy(float pres) {
	bool ret = false;
	bool legacy = true;
	if(trailsize < PHYSICS_SOLID_TRAILINDEX)
		return (POGEL::about(rotation.x, rots[trailsize-1].x, pres) && POGEL::about(rotation.y, rots[trailsize-1].y, pres) && POGEL::about(rotation.z, rots[trailsize-1].z, pres));
	if(POGEL::frames<(trailsize/PHYSICS_SOLID_TRAILINDEX))
		return false;
	for(unsigned long i=0;i<trailsize && i<stepstaken+1;i+=(trailsize/PHYSICS_SOLID_TRAILINDEX))
		//ret = legacy && (POGEL::about(rotation.x, rots[i].x, pres) && POGEL::about(rotation.y, rots[i].y, pres) && POGEL::about(rotation.z, rots[i].z, pres));
		if(legacy && (POGEL::about(rotation.x, rots[i].x, pres) && POGEL::about(rotation.y, rots[i].y, pres) && POGEL::about(rotation.z, rots[i].z, pres)))
			ret = true;
		else {
			ret = false;
			legacy = false;
			return false;
		}
	return ret;
};

bool POGEL::PHYSICS::SOLID::samelegacy(float pres) {
	bool ret = false;
	bool legacy = true;
	if(trailsize < PHYSICS_SOLID_TRAILINDEX)
		return (POGEL::about(position.x, trail[trailsize-1].x, pres) && POGEL::about(position.y, trail[trailsize-1].y, pres) && POGEL::about(position.z, trail[trailsize-1].z, pres)) && (POGEL::about(rotation.x, rots[trailsize-1].x, pres) && POGEL::about(rotation.y, rots[trailsize-1].y, pres) && POGEL::about(rotation.z, rots[trailsize-1].z, pres));
	if(POGEL::frames<(trailsize/PHYSICS_SOLID_TRAILINDEX))
		return false;
	for(unsigned long i=0;i<trailsize && i<stepstaken+1;i+=(trailsize/PHYSICS_SOLID_TRAILINDEX))
		if(legacy && (POGEL::about(position.x, trail[i].x, pres) && POGEL::about(position.y, trail[i].y, pres) && POGEL::about(position.z, trail[i].z, pres)) && (POGEL::about(rotation.x, rots[i].x, pres) && POGEL::about(rotation.y, rots[i].y, pres) && POGEL::about(rotation.z, rots[i].z, pres)))
			ret = true;
		else {
			ret = false;
			legacy = false;
			return false;
		}
	return ret;
};

void POGEL::PHYSICS::SOLID::offsettrail(POGEL::VECTOR v) {
	for(unsigned long i = trailsize-1;i>0;i--) trail[i] += v;
}

void POGEL::PHYSICS::SOLID::getbounding() {
	setboundingskips();
	//refbounding.draw(position);
	float r = (POGEL::hasproperty(POGEL_TIMEBASIS) ? POGEL::GetSecondsPerFrame() : 1);
	bool os = !bounding.surrounds(position,position,refbounding) || !bounding.surrounds(position,position+direction.topoint()*r,refbounding);
	if((stepstaken % objboundingskips == 0 && POGEL::frames > 0) || stepstaken <= 1 || os) {
		if(stepstaken > 0 && hasOption(PHYSICS_SOLID_SPHERE)) {
			float max = refbounding.maxdistance;
			bounding.clear();
			refbounding.clear();
			bounding.addpoint(POGEL::POINT(), position*0 + POGEL::POINT( 1, 0, 0)*max);
			bounding.addpoint(POGEL::POINT(), position*0 + POGEL::POINT( 0, 1, 0)*max);
			bounding.addpoint(POGEL::POINT(), position*0 + POGEL::POINT( 0, 0, 1)*max);
			bounding.addpoint(POGEL::POINT(), position*0 + POGEL::POINT(-1, 0, 0)*max);
			bounding.addpoint(POGEL::POINT(), position*0 + POGEL::POINT( 0,-1, 0)*max);
			bounding.addpoint(POGEL::POINT(), position*0 + POGEL::POINT( 0, 0,-1)*max);
			
			refbounding = bounding;
			bounding.addpoint(POGEL::POINT(), direction.topoint()*(float)objboundingskips*r + POGEL::POINT( 1, 0, 0)*max);
			bounding.addpoint(POGEL::POINT(), direction.topoint()*(float)objboundingskips*r + POGEL::POINT( 0, 1, 0)*max);
			bounding.addpoint(POGEL::POINT(), direction.topoint()*(float)objboundingskips*r + POGEL::POINT( 0, 0, 1)*max);
			bounding.addpoint(POGEL::POINT(), direction.topoint()*(float)objboundingskips*r + POGEL::POINT(-1, 0, 0)*max);
			bounding.addpoint(POGEL::POINT(), direction.topoint()*(float)objboundingskips*r + POGEL::POINT( 0,-1, 0)*max);
			bounding.addpoint(POGEL::POINT(), direction.topoint()*(float)objboundingskips*r + POGEL::POINT( 0, 0,-1)*max);
			
			bounding.finishactual();
			bounding.fin();
			bounding.offset(position);
			refbounding.offset(POGEL::POINT());
			bounding.maxdistance = max;
			refbounding.maxdistance = max;
			stepsatboundingcheck = stepstaken;
			return;
		}
		//setboundingskips();
		bounding.clear();
		refbounding.clear();
		POGEL::MATRIX mat1(rotation, MATRIX_CONSTRUCT_ROTATION);
		POGEL::MATRIX mat2(direction.topoint()*(float)objboundingskips, rotation + spin.topoint()*(float)objboundingskips);
		for( unsigned long t = 0 ; t < numfaces ; t+=2 )
			for( unsigned int v = 0 ; v < 3 ; v++ ) {
				bounding.addpoint(POGEL::POINT(), mat1.transformPoint(face[t].vertex[v].topoint()));
				refbounding.addpoint(POGEL::POINT(), mat1.transformPoint(face[t].vertex[v].topoint()));
				bounding.finishactual();
				bounding.addpoint(POGEL::POINT(), mat2.transformPoint(face[t].vertex[v].topoint()));
				bounding.unsetactual();
			}
		
		refbounding.offset(POGEL::POINT());
		
		bounding.fin();
		bounding.offset(position);
		
		stepsatboundingcheck = stepstaken;
	}
};

void POGEL::PHYSICS::SOLID::setboundingskips() {
	if( 
		stepstaken >= (objboundingskips/1+stepsatboundingcheck) || \
		stepstaken <= 1 || \
		(bounding.surrounds(position,POGEL::POINT(),refbounding) || bounding.surrounds(position,direction.topoint(),refbounding)) || \
		(bounding.isinside(position,position) || bounding.isinside(position,position+direction.topoint()))
	) {
		if(container != NULL && (container->boundingskips) > 0)
			objboundingskips = container->boundingskips;
		else
			if(direction.getdistance() == 0.0f)
				objboundingskips = 1;
			else
				objboundingskips = (unsigned long)(1.0f/(direction.getdistance()));
	}
	//POGEL::message("%s skps = %u\n",getname(),objboundingskips);
	if(objboundingskips < 1)  objboundingskips = 1;
	if(objboundingskips > 50) objboundingskips = 1;
};

void POGEL::PHYSICS::SOLID::build()  {
	POGEL::OBJECT::build();
	getbounding();
};

void POGEL::PHYSICS::SOLID::draw() {
	unsigned int prp = POGEL::getproperties();
	if(POGEL::hasproperty(POGEL_LABEL)) POGEL::removeproperty(POGEL_LABEL);
	POGEL::OBJECT::draw();
	POGEL::setproperties(prp);
	glLineWidth(3);
	bounding.draw(POGEL::POINT());
	glLineWidth(1);
	
	if(POGEL::hasproperty(POGEL_LABEL)) {
		POGEL::LINE( position, 
			position + (
				direction*(POGEL::hasproperty(POGEL_TIMEBASIS) ? 
				POGEL::GetSecondsPerFrame() : 1)
				).topoint(), 
			1, POGEL::COLOR( 1,.5, 0, 1)
		).draw();
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		if(behavior.magnetic && behavior.charge != 0.0f) {
			if(behavior.charge < 0.0f)	glColor3f(1,.5,.2);
			else						glColor3f(.5,1,.2);
		}	else						glColor3f(.2,.5,1);
		glPointSize(4);
		glBegin(GL_POINTS); glVertex3f(position.x, position.y, position.z); glEnd();
		glPointSize(1);
		POGEL::COLOR(1,1,1,1).set();
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
	}
	
	if(POGEL::hasproperty(POGEL_TRAILS)) {
		#ifdef SOLID_DISPLAY_ROTATION_TRAIL
			POGEL::MATRIX mat[2];
			mat[1] = POGEL::MATRIX(rots[0], MATRIX_CONSTRUCT_ROTATION);
			float len = bounding.maxdistance*1.1f;
			
			#ifdef SOLID_DISPLAY_STIPPLED_NEGATIVE_ROTATION_TRAIL
				glLineStipple(2, 0x1111);
			#endif /* SOLID_DISPLAY_STIPPLED_NEGATIVE_ROTATION_TRAIL */
		#endif /* SOLID_DISPLAY_ROTATION_TRAIL */
		
		for(unsigned int i = 0 ; i < trailsize - 1 && i < stepstaken ; i++) {	
			float color = 1.0f;
			#ifdef SOLID_DISPLAY_TRAIL_FADING
				color = ((float)((trailsize-1)-i)/(float)(trailsize-1));
			#endif /* SOLID_DISPLAY_TRAIL_FADING */
			
			POGEL::LINE(trail[i],trail[i+1],3,POGEL::COLOR(1,1,0,color)).draw(); // draw the position trail
			
			#ifdef SOLID_DISPLAY_ROTATION_TRAIL // draw the rotation trail
				mat[0] = mat[1];
				mat[1] = POGEL::MATRIX(rots[i+1], MATRIX_CONSTRUCT_ROTATION);
				POGEL::POINT x[3], y[3], z[3];
				x[0] = y[0] = z[0] = POGEL::POINT();
				
				for(int a = 0; a < 2; a++) {
					x[a+1] = mat[a+1].transformPoint(POGEL::POINT(len,0,0));
					y[a+1] = mat[a+1].transformPoint(POGEL::POINT(0,len,0));
					z[a+1] = mat[a+1].transformPoint(POGEL::POINT(0,0,len));
					
					POGEL::LINE(trail[i]+x[a], trail[i+a]+x[a+1], POGEL::COLOR(1,0,0,color)).draw(); // x axis positive
					POGEL::LINE(trail[i]+y[a], trail[i+a]+y[a+1], POGEL::COLOR(0,1,0,color)).draw(); // y axis positive
					POGEL::LINE(trail[i]+z[a], trail[i+a]+z[a+1], POGEL::COLOR(0,0,1,color)).draw(); // z axis positive
					
					#ifdef SOLID_DISPLAY_NEGATIVE_ROTATION_TRAIL // the negative rotation trail
						#ifdef SOLID_DISPLAY_STIPPLED_NEGATIVE_ROTATION_TRAIL
							glEnable(GL_LINE_STIPPLE);
						#endif
						
						POGEL::LINE(trail[i]-x[a], trail[i+a]-x[a+1], POGEL::COLOR( 1,.5,.5,color)).draw(); // x axis negative
						POGEL::LINE(trail[i]-y[a], trail[i+a]-y[a+1], POGEL::COLOR(.5, 1,.5,color)).draw(); // y axis negative
						POGEL::LINE(trail[i]-z[a], trail[i+a]-z[a+1], POGEL::COLOR(.5,.5, 1,color)).draw(); // z axis negative
						
						#ifdef SOLID_DISPLAY_STIPPLED_NEGATIVE_ROTATION_TRAIL
							glDisable(GL_LINE_STIPPLE);
						#endif /* SOLID_DISPLAY_STIPPLED_NEGATIVE_ROTATION_TRAIL */
					#endif /* SOLID_DISPLAY_NEGATIVE_ROTATION_TRAIL */
				}
			#endif /* SOLID_DISPLAY_ROTATION_TRAIL */
		}
		
		POGEL::COLOR(1,1,1,1).set(); // catch any erronious coloration mistakes
		
		#ifdef SOLID_DISPLAY_STIPPLED_NEGATIVE_ROTATION_TRAIL
			glDisable(GL_LINE_STIPPLE);
			glLineStipple(1, 0xFFFF);
		#endif /* SOLID_DISPLAY_STIPPLED_NEGATIVE_ROTATION_TRAIL */
	}
}

void POGEL::PHYSICS::SOLID::step() {
	increment();
	steptrail();
	//if(container != NULL && (POGEL::frames)%(container->boundingskips) == 0)
	getbounding();
	if(function != NULL) function(this);
};

void POGEL::PHYSICS::SOLID::closest(POGEL::POINT point, POGEL::POINT* objpt, POGEL::TRIANGLE* tri) {
	float origdist = position.distance(point);
	float dist = origdist;
	unsigned long triindex = 0;
	POGEL::MATRIX mat(position, rotation);
	
	origdist = point_triangle_distance(point, mat.transformTriangle(gettriangle(0)), NULL);
	
	// *objpt = position;
	 point_triangle_distance(point, mat.transformTriangle(gettriangle(0)), objpt);
	*tri = mat.transformTriangle(gettriangle(0));
	
	for(unsigned long a = 0; a < getnumfaces(); a++) {
		POGEL::TRIANGLE tritmp = mat.transformTriangle(gettriangle(a));
		
		if(false/*||tritmp.distcheck(point, origdist) */|| dist > point_triangle_distance(point, tritmp, NULL)) {
			POGEL::POINT pointtmp1 = point;
			POGEL::POINT res2d, res3d;
			
			bool col = POGEL::PHYSICS::line_triangle_collision(
				point,
				point+(tritmp.normal.topoint()*origdist)*(tritmp.isinfront(point) ? -1.0f : 1.0f),
				tritmp,
				&res2d,
				&res3d
			);
			
			if(col && res2d.z < dist) {
				dist = res2d.z;
				*objpt = res3d;
				//if(tritmp.distance(point) < POGEL::MATRIX(position, rotation).transformTriangle(gettriangle(triindex)).distance(point)) {
					*tri = tritmp;
					triindex = a;
				//}
			}
			else {
				POGEL::POINT p;
				float d = point_triangle_distance(point, tritmp, &p);
				if(d < dist) {
					dist = d;
					*objpt = p;
					//if(tritmp.distance(pointtmp1) < POGEL::MATRIX(position, rotation).transformTriangle(gettriangle(triindex)).distance(pointtmp1)) {
						*tri = tritmp;
						triindex = a;
					//}
				}
			}
		}
	}
	//POGEL::LINE(*objpt, point, POGEL::COLOR(1,0,0,0.5)).draw();
};

void POGEL::PHYSICS::SOLID::closest(POGEL::PHYSICS::SOLID* other, POGEL::POINT* obj1pt, POGEL::POINT* obj2pt, POGEL::TRIANGLE* tri1, POGEL::TRIANGLE* tri2) {
	
	*obj1pt = this->position, *obj2pt = other->position;
	POGEL::POINT obj1ptold = *obj2pt, obj2ptold = *obj1pt;
	unsigned int c = 0;
	while(c++ < 5 && obj1ptold.distance(*obj1pt) > 0.0f && obj2ptold.distance(*obj2pt) > 0.0f && (obj1ptold != *obj1pt && obj2ptold != *obj2pt)) {
		//POGEL::message("%f, %f\n", obj1ptold.distance(*obj1pt), obj2ptold.distance(*obj2pt));
		obj1ptold = *obj1pt;
		obj2ptold = *obj2pt;
		other->closest(*obj1pt, obj2pt, tri2);
		this->closest(*obj2pt, obj1pt, tri1);
	}
	//POGEL::LINE(*obj1pt, *obj2pt, POGEL::COLOR(0,1,0,1)).draw();
};

