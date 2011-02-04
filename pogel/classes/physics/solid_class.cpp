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
	
	force = POGEL::VECTOR();
	sleeping = false;
	cantsleep = false;
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
	
	force = POGEL::VECTOR();
	sleeping = false;
	cantsleep = false;
};

POGEL::PHYSICS::SOLID::~SOLID() {
	if(trail!=NULL) delete trail;
	if(rots!=NULL) delete rots;
};

unsigned long POGEL::PHYSICS::SOLID::getstepstaken() { return stepstaken; }
void POGEL::PHYSICS::SOLID::setstepstaken(unsigned long s) { stepstaken = s; }

void POGEL::PHYSICS::SOLID::setCallback(void (*func)(POGEL::PHYSICS::SOLID*,char*) ) { callback = func; };
void POGEL::PHYSICS::SOLID::setStepFunc(void (*func)(POGEL::PHYSICS::SOLID*) ) { function = func; };

bool POGEL::PHYSICS::SOLID::napping() { return sleeping; };
void POGEL::PHYSICS::SOLID::sleep() { if(!cantsleep) sleeping = true; };
void POGEL::PHYSICS::SOLID::wake() { if(!cantsleep) sleeping = false; };
void POGEL::PHYSICS::SOLID::forcesleep() { sleeping = true; };
void POGEL::PHYSICS::SOLID::forcewake() { sleeping = false; };
void POGEL::PHYSICS::SOLID::zombify() { cantsleep = true; };
void POGEL::PHYSICS::SOLID::unzombify() { cantsleep = false; };

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
	for(unsigned long i = trailsize-1;i>0;i--)
		trail[i] = trail[i-1];
};

void POGEL::PHYSICS::SOLID::steprottrail() {
	rots[0] = rotation;
	for(unsigned long i = trailsize-1;i>0;i--)
		rots[i] = rots[i-1];
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
	if(stepstaken<(trailsize/PHYSICS_SOLID_TRAILINDEX))
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
	if(stepstaken<(trailsize/PHYSICS_SOLID_TRAILINDEX))
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
	if(stepstaken<(trailsize/PHYSICS_SOLID_TRAILINDEX))
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

bool POGEL::PHYSICS::SOLID::sameposlegacy(float pres, unsigned long len) {
	bool ret = false;
	bool legacy = true;
	if(trailsize < len)
		return (POGEL::about(position.x, trail[trailsize-1].x, pres) && POGEL::about(position.y, trail[trailsize-1].y, pres) && POGEL::about(position.z, trail[trailsize-1].z, pres));
	if(stepstaken<len)
		return false;
	for(unsigned long i=0;i<len && i<stepstaken+1;i++)
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

bool POGEL::PHYSICS::SOLID::samerotlegacy(float pres, unsigned long len) {
	bool ret = false;
	bool legacy = true;
	if(trailsize < len)
		return (POGEL::about(rotation.x, rots[trailsize-1].x, pres) && POGEL::about(rotation.y, rots[trailsize-1].y, pres) && POGEL::about(rotation.z, rots[trailsize-1].z, pres));
	if(stepstaken<len)
		return false;
	for(unsigned long i=0;i<len && i<stepstaken+1;i++)
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

bool POGEL::PHYSICS::SOLID::samelegacy(float pres, unsigned long len) {
	bool ret = false;
	bool legacy = true;
	if(trailsize < len)
		return (POGEL::about(position.x, trail[trailsize-1].x, pres) && POGEL::about(position.y, trail[trailsize-1].y, pres) && POGEL::about(position.z, trail[trailsize-1].z, pres)) && (POGEL::about(rotation.x, rots[trailsize-1].x, pres) && POGEL::about(rotation.y, rots[trailsize-1].y, pres) && POGEL::about(rotation.z, rots[trailsize-1].z, pres));
	if(stepstaken<len)
		return false;
	for(unsigned long i=0;i<len && i<stepstaken+1;i++)
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
	bool os = false;
	os = os || !bounding.surrounds(POGEL::POINT(),position,refbounding);
	os = os || !bounding.surrounds(POGEL::POINT(),position+direction*r,refbounding);
	os = os || bounding.isoutside(POGEL::POINT(), position);
	
	if((stepstaken % objboundingskips == 0 && POGEL::frames > 0) || stepstaken <= 1 || os)
		forcegetbounding();
};

void POGEL::PHYSICS::SOLID::setboundingskips() {
	float r = (POGEL::hasproperty(POGEL_TIMEBASIS) ? POGEL::GetSecondsPerFrame() : 1);
	bool os = false;
	os = os || !bounding.surrounds(POGEL::POINT(),position,refbounding);
	os = os || !bounding.surrounds(POGEL::POINT(),position+direction*r,refbounding);
	os = os || bounding.isoutside(POGEL::POINT(), position);
	
	if(stepstaken >= (objboundingskips/1+stepsatboundingcheck) || stepstaken <= 1 || os) {
		if(container != NULL && (container->boundingskips) > 0)
			objboundingskips = container->boundingskips;
		else if(direction.getdistance() == 0.0f)
			objboundingskips = 1;
		else
			objboundingskips = (unsigned long)(1.0f/(direction.getdistance()*r))*2;
	}
	//POGEL::message("%s skps = %u\n",getname(),objboundingskips);
	if(objboundingskips < 1)  objboundingskips = 1;
	//if(objboundingskips > 50) objboundingskips = 1;
};

void POGEL::PHYSICS::SOLID::forcegetbounding() {
	float r = (POGEL::hasproperty(POGEL_TIMEBASIS) ? POGEL::GetSecondsPerFrame() : 1)*refbounding.maxdistance;
	
	if(stepstaken > 0 && hasOption(PHYSICS_SOLID_SPHERE)) {
		float max = refbounding.maxdistance;
		bounding.clear();
		bounding.addpoint(POGEL::POINT(), POGEL::POINT( max, 0, 0));
		bounding.addpoint(POGEL::POINT(), POGEL::POINT( 0, max, 0));
		bounding.addpoint(POGEL::POINT(), POGEL::POINT( 0, 0, max));
		bounding.addpoint(POGEL::POINT(), POGEL::POINT(-max, 0, 0));
		bounding.addpoint(POGEL::POINT(), POGEL::POINT( 0,-max, 0));
		bounding.addpoint(POGEL::POINT(), POGEL::POINT( 0, 0,-max));
		
		refbounding.clear(); refbounding = bounding; refbounding.color = BOUNDING_DEFAULT_COLOR;
		
		bounding.addpoint(POGEL::POINT(), direction*(float)objboundingskips*r + POGEL::POINT( max, 0, 0));
		bounding.addpoint(POGEL::POINT(), direction*(float)objboundingskips*r + POGEL::POINT( 0, max, 0));
		bounding.addpoint(POGEL::POINT(), direction*(float)objboundingskips*r + POGEL::POINT( 0, 0, max));
		bounding.addpoint(POGEL::POINT(), direction*(float)objboundingskips*r + POGEL::POINT(-max, 0, 0));
		bounding.addpoint(POGEL::POINT(), direction*(float)objboundingskips*r + POGEL::POINT( 0,-max, 0));
		bounding.addpoint(POGEL::POINT(), direction*(float)objboundingskips*r + POGEL::POINT( 0, 0,-max));
		
		bounding.finishactual();
		bounding.fin();
		
		bounding.offset(position);  refbounding.offset(POGEL::POINT());
		bounding.maxdistance = max; refbounding.maxdistance = max;
		
		stepsatboundingcheck = stepstaken;
		return;
	}
	
	bounding.clear(); refbounding.clear();
	POGEL::MATRIX mat1(rotation, MATRIX_CONSTRUCT_ROTATION);
	POGEL::MATRIX mat2(direction*(float)objboundingskips*r, rotation + spin.topoint()*(float)objboundingskips*r);
	for( unsigned long t = 0 ; t < getnumfaces() ; t++ )
		for( unsigned int v = 0 ; v < 3 ; v++ ) {
			bounding.addpoint(POGEL::POINT(), mat1.transformPoint(gettriangle(t).vertex[v].topoint()));
			refbounding.addpoint(POGEL::POINT(), mat1.transformPoint(gettriangle(t).vertex[v].topoint()));
			bounding.finishactual();
			bounding.addpoint(POGEL::POINT(), mat2.transformPoint(gettriangle(t).vertex[v].topoint()));
			bounding.unsetactual();
		}
	bounding.fin();
	bounding.offset(position);
	refbounding.offset(POGEL::POINT());
	
	stepsatboundingcheck = stepstaken;
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
	glLineWidth(2);
	bounding.draw(POGEL::POINT());
	glLineWidth(1);
	refbounding.draw(position);
	
	if(POGEL::hasproperty(POGEL_LABEL)) {
		POGEL::LINE( position, 
			position + (direction*2*(POGEL::hasproperty(POGEL_TIMEBASIS) ? POGEL::GetSecondsPerFrame() : 1) ), 
			1, POGEL::COLOR( 1,.5, 0, 1) ).draw();
		if(behavior.magnetic && behavior.charge != 0.0f) {
			if(behavior.charge < 0.0f)	position.draw(3, POGEL::COLOR( 1,.5,.2,1));
			else						position.draw(3, POGEL::COLOR(.5, 1,.2,1));
		}	else if(sleeping)			position.draw(3, POGEL::COLOR(1,0, .5,1));
			else						position.draw(3, POGEL::COLOR(.2,.5, 1,1));
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
			
			if((stepstaken-i)%20==0) trail[i].draw(3, POGEL::COLOR(1,1,0,color));
			
			if(trail[i].distance(trail[i+1]) > 1) continue;
			
			POGEL::LINE(trail[i], trail[i+1], 1, POGEL::COLOR(1,1,0,color)).draw(); // draw the position trail
			
			#ifdef SOLID_DISPLAY_ROTATION_TRAIL // draw the rotation trail
				mat[0] = mat[1];
				mat[1] = POGEL::MATRIX(rots[i+1], MATRIX_CONSTRUCT_ROTATION);
				POGEL::POINT x[3], y[3], z[3];
				x[0] = y[0] = z[0] = POGEL::POINT();
				
				for(int a = 0; a < 2; a++) {
					x[a+1] = mat[a+0].transformPoint(POGEL::POINT(len,0,0));
					y[a+1] = mat[a+0].transformPoint(POGEL::POINT(0,len,0));
					z[a+1] = mat[a+0].transformPoint(POGEL::POINT(0,0,len));
				}
				
				for(int a = ((stepstaken-i)%20==0 ? 0 : 1); a < 2; a++) {
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
};

void POGEL::PHYSICS::SOLID::increment() {
	float r = (POGEL::hasproperty(POGEL_TIMEBASIS) ? POGEL::GetSecondsPerFrame() : 1);
	if(!this->hasOption(PHYSICS_SOLID_STATIONARY)) {
		rotate(spin*r);
		//translate(force);//*r;
		translate(direction*r);
	}
	else {
		rotate(spin*r);
		translate(direction*r);
	}
	//force = POGEL::VECTOR();
	stepstaken++;
};

void POGEL::PHYSICS::SOLID::clearForce() { force = POGEL::VECTOR(); };
void POGEL::PHYSICS::SOLID::addForce() {
	if(!this->hasOption(PHYSICS_SOLID_STATIONARY)) direction += force;
};

void POGEL::PHYSICS::SOLID::step() {
	increment();
	steptrail();
	getbounding();
	if(function != NULL) function(this);
};

void POGEL::PHYSICS::SOLID::closest(POGEL::POINT point, POGEL::POINT* objpt, POGEL::TRIANGLE* tri) {
	if(getnumfaces() == 0)
		{ *objpt = position; tri = NULL; return; }
	
	float origdist;
	POGEL::MATRIX mat(position, rotation);
	
	origdist = point_triangle_distance(point, mat.transformTriangle(gettriangle(0)), NULL);
	
	point_triangle_distance(point, mat.transformTriangle(gettriangle(0)), objpt);
	*tri = mat.transformTriangle(gettriangle(0));
	
	float dist = origdist;
	for(unsigned long a = 0; a < getnumfaces(); a++) {
		POGEL::TRIANGLE tritmp = mat.transformTriangle(gettriangle(a));
		
		POGEL::POINT p;
		float d = point_triangle_distance(point, tritmp, &p);
		if(d < dist) {
			dist = d;
			*objpt = p;
			*tri = tritmp;
		}
	}
	//POGEL::LINE(*objpt, point, POGEL::COLOR(1,0,0,0.5)).draw();
};

void POGEL::PHYSICS::SOLID::closest(POGEL::PHYSICS::SOLID* other, POGEL::POINT* obj1pt, POGEL::POINT* obj2pt, POGEL::TRIANGLE* tri1, POGEL::TRIANGLE* tri2) {
	
	*obj1pt = this->position; *obj2pt = other->position;
	POGEL::POINT obj1ptold = *obj2pt, obj2ptold = *obj1pt;
	unsigned int c = 0;
	while(c++ < 5 && obj1ptold.distance(*obj1pt) > 0.0f && obj2ptold.distance(*obj2pt) > 0.0f && (obj1ptold != *obj1pt && obj2ptold != *obj2pt)) {
		//POGEL::message("%f, %f\n", obj1ptold.distance(*obj1pt), obj2ptold.distance(*obj2pt));
		obj1ptold = *obj1pt; obj2ptold = *obj2pt;
		other->closest(*obj1pt, obj2pt, tri2);
		this->closest(*obj2pt, obj1pt, tri1);
	}
	//POGEL::LINE(*obj1pt, *obj2pt, POGEL::COLOR(0,1,0,1)).draw();
};

std::string POGEL::PHYSICS::SOLID::toString() {
	/*std::string triangles;
	for(unsigned long a = 0; a < getnumfaces(); a++) {
		triangles += gettriangle(a).toString()+(a<getnumfaces()-1 ? ",":"");
	}*/
	char *pprp = POGEL::string("%u",physproperties), *prp = POGEL::string("%u",getproperties());
	std::string s = "{["+getsname()+"],["+std::string(pprp)+"],["+std::string(prp)+"],["+position.toString()+"],["+rotation.toString()+"],["+direction.toString()+"],["+spin.toString()+"],["+behavior.toString()+"]}";//,<"+triangles+">}";
	free(pprp); free(prp);
	return s;
};

