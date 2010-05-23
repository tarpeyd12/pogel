#include "../../pogel.h"
#include "../../pogel_internals.h"
#include "solid_class.h"

POGEL::PHYSICS::SOLID::SOLID() : POGEL::OBJECT() {
	behavior = POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES();
	physproperties = 0;
	maximumdistance = 0.0f;
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
};

POGEL::PHYSICS::SOLID::SOLID(POGEL::OBJECT* obj, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES attr, unsigned int prop) : POGEL::OBJECT(obj) {
	behavior = attr;
	physproperties = prop;
	maximumdistance = 0.0f;
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
};

POGEL::PHYSICS::SOLID::~SOLID() {
	delete trail;
	delete rots;
};

void POGEL::PHYSICS::SOLID::resizetrail(unsigned long size) {
	delete trail;
	delete rots;
	
	trailsize = size;
	
	trail = new POGEL::POINT[size];
	rots = new POGEL::POINT[size];
	
	for( int i = 0; i < trailsize; i++ ) {
		trail[i] = position*PARTICLE_SLOWDOWN;
		rots[i] = rotation*PARTICLE_SLOWDOWN;
	}
};

void POGEL::PHYSICS::SOLID::steppostrail() {
	trail[0] = position;
	//rots[0] = rotation;
	for(int i = trailsize-1;i>0;i--) {
		trail[i] = trail[i-1];
		//rots[i] = rots[i-1];
	}
};

void POGEL::PHYSICS::SOLID::steprottrail() {
	//trail[0] = position;
	rots[0] = rotation;
	for(int i = trailsize-1;i>0;i--) {
		//trail[i] = trail[i-1];
		rots[i] = rots[i-1];
	}
};

void POGEL::PHYSICS::SOLID::steptrail() {
	trail[0] = position;
	rots[0] = rotation;
	for(int i = trailsize-1;i>0;i--) {
		trail[i] = trail[i-1];
		rots[i] = rots[i-1];
	}
};

bool POGEL::PHYSICS::SOLID::sameposlegacy(float pres) {
	bool ret = false;
	bool legacy = true;
	if(POGEL::frames<(trailsize/PHYSICS_SOLID_TRAILINDEX))
		return false;
	for(unsigned long i=0;i<trailsize && i<POGEL::frames+1;i+=(trailsize/PHYSICS_SOLID_TRAILINDEX))
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
	if(POGEL::frames<(trailsize/PHYSICS_SOLID_TRAILINDEX))
		return false;
	for(unsigned long i=0;i<trailsize && i<POGEL::frames+1;i+=(trailsize/PHYSICS_SOLID_TRAILINDEX))
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
	if(POGEL::frames<(trailsize/PHYSICS_SOLID_TRAILINDEX))
		return false;
	for(unsigned long i=0;i<trailsize && i<POGEL::frames+1;i+=(trailsize/PHYSICS_SOLID_TRAILINDEX))
		//ret = legacy && (POGEL::about(position.x, trail[i].x, pres) && POGEL::about(position.y, trail[i].y, pres) && POGEL::about(position.z, trail[i].z, pres)) && (POGEL::about(rotation.x, rots[i].x, pres) && POGEL::about(rotation.y, rots[i].y, pres) && POGEL::about(rotation.z, rots[i].z, pres));
		if(legacy && (POGEL::about(position.x, trail[i].x, pres) && POGEL::about(position.y, trail[i].y, pres) && POGEL::about(position.z, trail[i].z, pres)) && (POGEL::about(rotation.x, rots[i].x, pres) && POGEL::about(rotation.y, rots[i].y, pres) && POGEL::about(rotation.z, rots[i].z, pres)))
			ret = true;
		else {
			ret = false;
			legacy = false;
			return false;
		}
	return ret;
};

void POGEL::PHYSICS::SOLID::getbounding() {
	if(container != NULL && (POGEL::frames)%(container->boundingskips) == 0) {
		bounding.clear();
		POGEL::MATRIX mat(rotation, MATRIX_CONSTRUCT_ROTATION);
		for( int t = 0 ; t < numfaces ; t++ )
			for( int v = 0 ; v < 3 ; v++ )
				bounding.addpoint(POGEL::POINT(), mat.transformPoint(face[t].vertex[v].topoint()));
		
		bounding.finishactual();
		
		mat = POGEL::MATRIX(direction.topoint()*(float)container->boundingskips, rotation + spin.topoint()*(float)container->boundingskips);
		for( int t = 0 ; t < numfaces ; t++ )
			for( int v = 0 ; v < 3 ; v++ )
				bounding.addpoint(POGEL::POINT(), mat.transformPoint(face[t].vertex[v].topoint()));
		
		bounding.fin();
		bounding.offset(position);
	}
};

void POGEL::PHYSICS::SOLID::build()  {
	POGEL::OBJECT::build();
	if(container != NULL)
		getbounding();
};

void POGEL::PHYSICS::SOLID::draw() {
	POGEL::OBJECT::draw();
	//if(container != NULL && (POGEL::frames)%(container->boundingskips) == 0)
	//getbounding();
	glLineWidth(3);
	bounding.draw(POGEL::POINT());
	glLineWidth(1);
	
	if(POGEL::hasproperty(POGEL_TRAILS)) {
		float len = bounding.maxdistance*1.1f;//0.5f;
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		#ifdef SOLID_DISPLAY_STIPPLED_NEGATIVE_ROTATION_TRAIL
		//glEnable(GL_LINE_STIPPLE);
		glLineStipple(2, 0x1111);
		#endif
		
		
		#ifdef SOLID_DISPLAY_ROTATION_TRAIL
		POGEL::MATRIX mat[2];
		mat[1] = POGEL::MATRIX(rots[0], MATRIX_CONSTRUCT_ROTATION);
		#endif /* SOLID_DISPLAY_ROTATION_TRAIL */
		
		for(unsigned int i=0; i<trailsize-1 && i<POGEL::frames; i++) {
			#ifdef SOLID_DISPLAY_TRAIL_FADING
				float color = ((float)((trailsize-1)-i)/(float)(trailsize-1));
			#else
				float color = 1.0f;
			#endif /* SOLID_DISPLAY_TRAIL_FADING */
			
			// draw the position trail
			glColor3f(1.0f*color,1.0f*color,0.0f);
			glLineWidth(4);
			glBegin(GL_LINES);
				glVertex3f(trail[i+0].x, trail[i+0].y, trail[i+0].z);
				glVertex3f(trail[i+1].x, trail[i+1].y, trail[i+1].z);
			glEnd();
			glLineWidth(1);
			
			//glColor3f(1.0f,1.0f,1.0f);
			
			// draw the rotation trail
			#ifdef SOLID_DISPLAY_ROTATION_TRAIL
				mat[0] = mat[1];
				//mat[0] = POGEL::MATRIX(rots[i+0], MATRIX_CONSTRUCT_ROTATION);
				mat[1] = POGEL::MATRIX(rots[i+1], MATRIX_CONSTRUCT_ROTATION);
				
				POGEL::POINT x[2], y[2], z[2];
				x[0] = mat[0].transformPoint(POGEL::POINT(1.0f,0.0f,0.0f)*len);
				x[1] = mat[1].transformPoint(POGEL::POINT(1.0f,0.0f,0.0f)*len);
				
				y[0] = mat[0].transformPoint(POGEL::POINT(0.0f,1.0f,0.0f)*len);
				y[1] = mat[1].transformPoint(POGEL::POINT(0.0f,1.0f,0.0f)*len);
				
				z[0] = mat[0].transformPoint(POGEL::POINT(0.0f,0.0f,1.0f)*len);
				z[1] = mat[1].transformPoint(POGEL::POINT(0.0f,0.0f,1.0f)*len);
				
				// x axis positive
				glColor3f(1.0f*color,0.0f,0.0f);
				glBegin(GL_LINES);
					glVertex3f(trail[i+0].x, trail[i+0].y, trail[i+0].z);
					glVertex3f(x[0].x+trail[i+0].x, x[0].y+trail[i+0].y, x[0].z+trail[i+0].z);
				/*glEnd();
				glBegin(GL_LINES);*/
					glVertex3f(x[0].x+trail[i+0].x, x[0].y+trail[i+0].y, x[0].z+trail[i+0].z);
					glVertex3f(x[1].x+trail[i+1].x, x[1].y+trail[i+1].y, x[1].z+trail[i+1].z);
				glEnd();
				
				// y axis positive
				glColor3f(0.0f,1.0f*color,0.0f);
				glBegin(GL_LINES);
					glVertex3f(trail[i+0].x, trail[i+0].y, trail[i+0].z);
					glVertex3f(y[0].x+trail[i+0].x, y[0].y+trail[i+0].y, y[0].z+trail[i+0].z);
				/*glEnd();
				glBegin(GL_LINES);*/
					glVertex3f(y[0].x+trail[i+0].x, y[0].y+trail[i+0].y, y[0].z+trail[i+0].z);
					glVertex3f(y[1].x+trail[i+1].x, y[1].y+trail[i+1].y, y[1].z+trail[i+1].z);
				glEnd();
				
				// z axis positive
				glColor3f(0.0f,0.0f,1.0f*color);
				glBegin(GL_LINES);
					glVertex3f(trail[i+0].x, trail[i+0].y, trail[i+0].z);
					glVertex3f(z[0].x+trail[i+0].x, z[0].y+trail[i+0].y, z[0].z+trail[i+0].z);
				/*glEnd();
				glBegin(GL_LINES);*/
					glVertex3f(z[0].x+trail[i+0].x, z[0].y+trail[i+0].y, z[0].z+trail[i+0].z);
					glVertex3f(z[1].x+trail[i+1].x, z[1].y+trail[i+1].y, z[1].z+trail[i+1].z);
				glEnd();
				
				
				// the negative rotation trail
				#ifdef SOLID_DISPLAY_NEGATIVE_ROTATION_TRAIL
					#ifdef SOLID_DISPLAY_STIPPLED_NEGATIVE_ROTATION_TRAIL
					//glLineStipple(3, 0x6666);
					glEnable(GL_LINE_STIPPLE);
					#endif
					// x axis negative
					glColor3f(1.0f*color,0.5f*color,0.5f*color);
					glBegin(GL_LINES);
						glVertex3f(trail[i+0].x, trail[i+0].y, trail[i+0].z);
						glVertex3f(trail[i+0].x-x[0].x, trail[i+0].y-x[0].y, trail[i+0].z-x[0].z);
					/*glEnd();
					glBegin(GL_LINES);*/
						glVertex3f(trail[i+0].x-x[0].x, trail[i+0].y-x[0].y, trail[i+0].z-x[0].z);
						glVertex3f(trail[i+1].x-x[1].x, trail[i+1].y-x[1].y, trail[i+1].z-x[1].z);
					glEnd();
					
					// y axis negative
					glColor3f(0.5f*color,1.0f*color,0.5f*color);
					glBegin(GL_LINES);
						glVertex3f(trail[i+0].x, trail[i+0].y, trail[i+0].z);
						glVertex3f(trail[i+0].x-y[0].x, trail[i+0].y-y[0].y, trail[i+0].z-y[0].z);
					/*glEnd();
					glBegin(GL_LINES);*/
						glVertex3f(trail[i+0].x-y[0].x, trail[i+0].y-y[0].y, trail[i+0].z-y[0].z);
						glVertex3f(trail[i+1].x-y[1].x, trail[i+1].y-y[1].y, trail[i+1].z-y[1].z);
					glEnd();
				
					// z axis negative
					glColor3f(0.5f*color,0.5f*color,1.0f*color);
					glBegin(GL_LINES);
						glVertex3f(trail[i+0].x, trail[i+0].y, trail[i+0].z);
						glVertex3f(trail[i+0].x-z[0].x, trail[i+0].y-z[0].y, trail[i+0].z-z[0].z);
					/*glEnd();
					glBegin(GL_LINES);*/
						glVertex3f(trail[i+0].x-z[0].x, trail[i+0].y-z[0].y, trail[i+0].z-z[0].z);
						glVertex3f(trail[i+1].x-z[1].x, trail[i+1].y-z[1].y, trail[i+1].z-z[1].z);
					glEnd();
					#ifdef SOLID_DISPLAY_STIPPLED_NEGATIVE_ROTATION_TRAIL
					//glLineStipple(1, 0xffff);
					glDisable(GL_LINE_STIPPLE);
					#endif
				#endif /* SOLID_DISPLAY_NEGATIVE_ROTATION_TRAIL */
				
			#endif /* SOLID_DISPLAY_ROTATION_TRAIL */
		}
		glColor3f(1.0f,1.0f,1.0f);
		#ifdef SOLID_DISPLAY_STIPPLED_NEGATIVE_ROTATION_TRAIL
		glDisable(GL_LINE_STIPPLE);
		glLineStipple(1, 0xFFFF);
		#endif
		glEnable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
	}
}

void POGEL::PHYSICS::SOLID::step() {
	increment();
	steptrail();
	if(container != NULL && (POGEL::frames)%(container->boundingskips) == 0)
	getbounding();
};

void POGEL::PHYSICS::SOLID::closest(POGEL::PHYSICS::SOLID* other, POGEL::POINT* obj1pt, POGEL::POINT* obj2pt, POGEL::TRIANGLE* tri) {
	
	//POGEL::TRIANGLE ;
	//POGEL::POINT ;
	float origdist = this->position.distance(other->position);
	float dist = origdist;
	//unsigned long obj1triindex, obj2triindex;
	
	for(unsigned long a = 0; a < this->getnumfaces(); a++) {
		
		POGEL::TRIANGLE obj1tritmp = POGEL::MATRIX(this->position, this->rotation).transformTriangle(this->gettriangle(a));
		
		if(obj1tritmp.distcheck(other->position, dist))
			for(unsigned long b = 0; b < other->getnumfaces(); b++) {
				
				POGEL::TRIANGLE obj2tritmp = POGEL::MATRIX(other->position, other->rotation).transformTriangle(other->gettriangle(b));
				
				if(obj2tritmp.distcheck(this->position, dist)) {
					for(unsigned int c = 0; c < 3; c++) {
						POGEL::POINT pointtmp1 = obj1tritmp.vertex[c].topoint();
						POGEL::POINT res2d, res3d;
						
						bool col = POGEL::PHYSICS::line_triangle_collision(pointtmp1, pointtmp1+(obj2tritmp.normal.topoint()*origdist)*(obj2tritmp.isinfront(pointtmp1) ? -1.0f : 1.0f), obj2tritmp, &res2d, &res3d);
						if(col && res2d.z < dist) {
							dist = res2d.z;
							*obj1pt = pointtmp1;
							*obj2pt = res3d;
							*tri = obj2tritmp;
						}
						else {
							for(unsigned int d = 0; d < 3; d++) {
								POGEL::POINT pointtmp2 = obj2tritmp.vertex[d].topoint();
								if(pointtmp2.distance(pointtmp1) < dist) {
									dist = pointtmp2.distance(pointtmp1);
									*obj1pt = pointtmp1;
									*obj2pt = pointtmp2;
									*tri = obj1tritmp;
								}
							}
						}
					}
					
					for(unsigned int c = 0; c < 3; c++) {
						POGEL::POINT pointtmp2 = obj2tritmp.vertex[c].topoint();
						POGEL::POINT res2d, res3d;
						
						bool col = POGEL::PHYSICS::line_triangle_collision(pointtmp2, pointtmp2+(obj1tritmp.normal.topoint()*origdist)*(obj1tritmp.isinfront(pointtmp2) ? -1.0f : 1.0f), obj1tritmp, &res2d, &res3d);
						if(col && res2d.z < dist) {
							dist = res2d.z;
							*obj2pt = pointtmp2;
							*obj1pt = res3d;
							*tri = obj1tritmp;
						}
						else {
							for(unsigned int d = 0; d < 3; d++) {
								POGEL::POINT pointtmp1 = obj1tritmp.vertex[d].topoint();
								if(pointtmp1.distance(pointtmp2) < dist) {
									dist = pointtmp1.distance(pointtmp2);
									*obj1pt = pointtmp1;
									*obj2pt = pointtmp2;
									*tri = obj2tritmp;
								}
							}
						}
					}
				}
				
			}
	}
	
};

