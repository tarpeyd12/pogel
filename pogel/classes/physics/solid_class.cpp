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
	delete trail;
	delete rots;
};

void POGEL::PHYSICS::SOLID::resizetrail(unsigned long size) {
	delete trail;
	delete rots;
	
	trailsize = size;
	
	trail = new POGEL::POINT[size];
	rots = new POGEL::POINT[size];
	
	for( unsigned long i = 0; i < trailsize; i++ ) {
		trail[i] = position*PARTICLE_SLOWDOWN;
		rots[i] = rotation*PARTICLE_SLOWDOWN;
	}
};

void POGEL::PHYSICS::SOLID::steppostrail() {
	trail[0] = position;
	//rots[0] = rotation;
	for(unsigned long i = trailsize-1;i>0;i--) {
		trail[i] = trail[i-1];
		//rots[i] = rots[i-1];
	}
};

void POGEL::PHYSICS::SOLID::steprottrail() {
	//trail[0] = position;
	rots[0] = rotation;
	for(unsigned long i = trailsize-1;i>0;i--) {
		//trail[i] = trail[i-1];
		rots[i] = rots[i-1];
	}
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
	if(POGEL::frames<(trailsize/PHYSICS_SOLID_TRAILINDEX))
		return false;
	for(unsigned long i=0;i<trailsize && i<stepstaken+1;i+=(trailsize/PHYSICS_SOLID_TRAILINDEX))
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

void POGEL::PHYSICS::SOLID::offsettrail(POGEL::VECTOR v) {
	for(unsigned long i = trailsize-1;i>0;i--)
		trail[i] += v;
}

void POGEL::PHYSICS::SOLID::getbounding() {
	
	/*if(stepstaken <= 1 && container != NULL)
		container->getpull(this);*/
		
	
	setboundingskips();
	//refbounding.draw(position);
	if( 
		(stepstaken % objboundingskips == 0 && POGEL::frames > 0) || \
		stepstaken <= 1 /*|| \
		(bounding.surrounds(position,position,refbounding) || bounding.surrounds(position,position+direction.topoint(),refbounding)) || \
		(bounding.isinside(position,position) || bounding.isinside(position,position+direction.topoint()))*/
	) {
		setboundingskips();
		bounding.clear();
		POGEL::MATRIX mat(rotation, MATRIX_CONSTRUCT_ROTATION);
		for( unsigned long t = 0 ; t < numfaces ; t++ )
			for( unsigned int v = 0 ; v < 3 ; v++ )
				bounding.addpoint(POGEL::POINT(), mat.transformPoint(face[t].vertex[v].topoint()));
		
		bounding.finishactual();
		refbounding.clear();
		refbounding = bounding;
		//refbounding.fin();
		refbounding.offset(POGEL::POINT());
		
		//if(objboundingskips > 1 || stepstaken == 0) {
			mat = POGEL::MATRIX(direction.topoint()*(float)objboundingskips, rotation + spin.topoint()*(float)objboundingskips);
			for( unsigned long t = 0 ; t < numfaces ; t++ )
				for( unsigned int v = 0 ; v < 3 ; v++ )
					bounding.addpoint(POGEL::POINT(), mat.transformPoint(face[t].vertex[v].topoint()));
		//}
		
		bounding.fin();
		bounding.offset(position);
		
		stepsatboundingcheck = stepstaken;
	}
};

void POGEL::PHYSICS::SOLID::setboundingskips() {
	if( 
		stepstaken >= (objboundingskips/10+stepsatboundingcheck) || \
		stepstaken <= 1 /*|| \
		(bounding.surrounds(position,position,refbounding) || bounding.surrounds(position,position+direction.topoint(),refbounding)) || \
		(bounding.isinside(position,position) || bounding.isinside(position,position+direction.topoint()))*/
	) {
		if(container != NULL && (container->boundingskips) > 0)
			objboundingskips = container->boundingskips;
		else
			if(direction.getdistance() == 0.0f)
				objboundingskips = 1;
			else
				objboundingskips = (unsigned long)(1/(direction.getdistance()));
	}
	
	//POGEL::message("%s skps = %u\n",getname(),objboundingskips);
	
	if(objboundingskips < 1 || objboundingskips > 10)
		objboundingskips = 1;
};

void POGEL::PHYSICS::SOLID::build()  {
	POGEL::OBJECT::build();
	//if(container != NULL)
		getbounding();
	//objboundingskips = 6;
};

void POGEL::PHYSICS::SOLID::draw() {
	POGEL::OBJECT::draw();
	//if(container != NULL && (POGEL::frames)%(container->boundingskips) == 0)
	getbounding();
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
		
		for(unsigned int i=0; i<trailsize-1 && i<stepstaken; i++) {
			#ifdef SOLID_DISPLAY_TRAIL_FADING
				float color = ((float)((trailsize-1)-i)/(float)(trailsize-1));
			#else
				float color = 1.0f;
			#endif /* SOLID_DISPLAY_TRAIL_FADING */
			
			// draw the position trail
			glColor3f(1.0f*color,1.0f*color,0.0f);
			glLineWidth(3);
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
	//if(container != NULL && (POGEL::frames)%(container->boundingskips) == 0)
	getbounding();
	if(function != NULL)
		function(this);
};

void POGEL::PHYSICS::SOLID::closest(POGEL::POINT point, POGEL::POINT* objpt, POGEL::TRIANGLE* tri) {
	*tri = POGEL::TRIANGLE();
	float origdist = position.distance(point);
	float dist = origdist;
	unsigned long triindex = 0;
	unsigned long ptcount = 0;
	
	for(unsigned long a = 0; a < getnumfaces(); a++) {
		POGEL::TRIANGLE tritmp = POGEL::MATRIX(position, rotation).transformTriangle(gettriangle(a));
		
		if(true||tritmp.distcheck(point, origdist)) {
				POGEL::POINT pointtmp1 = point;
				POGEL::POINT res2d, res3d;
				
				bool col = POGEL::PHYSICS::line_triangle_collision(point, point+(tritmp.normal.topoint()*origdist)*(tritmp.isinfront(point) ? -1.0f : 1.0f), tritmp, &res2d, &res3d);
				if(col && res2d.z < dist) {
					dist = res2d.z;
					*objpt = res3d;
					//if(tritmp.distance(point) < POGEL::MATRIX(position, rotation).transformTriangle(gettriangle(triindex)).distance(point)) {
						*tri = tritmp;
						triindex = a;
					//}
					ptcount = 1;
				}
				else if(col && res2d.z == dist) {
					ptcount++;
					//dist = res2d.z;
					*objpt += res3d;
					//if(tritmp.distance(point) < POGEL::MATRIX(position, rotation).transformTriangle(gettriangle(triindex)).distance(point)) {
						*tri = tritmp;
						triindex = a;
					//}
				}
				else {
					for(unsigned int c = 0; c < 3; c++) {
						POGEL::POINT pointtmp2 = tritmp.vertex[c].topoint();
						
						if(pointtmp2.distance(pointtmp1) < dist) {
							dist = pointtmp2.distance(pointtmp1);
							*objpt = pointtmp2;
							//if(tritmp.distance(pointtmp1) < POGEL::MATRIX(position, rotation).transformTriangle(gettriangle(triindex)).distance(pointtmp1)) {
								*tri = tritmp;
								triindex = a;
							//}
							ptcount = 1;
						}
						else if(pointtmp2.distance(pointtmp1) == dist) {
							ptcount++;
							//dist = pointtmp2.distance(pointtmp1);
							*objpt += pointtmp2;
							//if(tritmp.distance(pointtmp1) < POGEL::MATRIX(position, rotation).transformTriangle(gettriangle(triindex)).distance(pointtmp1)) {
								*tri = tritmp;
								triindex = a;
							//}
						}
					}
				}
		}
	}
	
	if(ptcount != 0)
		*objpt /= (float)ptcount;
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
	
	
	/*
	//POGEL::TRIANGLE ;
	//POGEL::POINT ;
	float origdist = this->position.distance(other->position);
	float dist = origdist;
	unsigned long ptcount = 0;
	
	for(unsigned long a = 0; a < this->getnumfaces(); a++) {
		
		POGEL::TRIANGLE obj1tritmp = POGEL::MATRIX(this->position, this->rotation).transformTriangle(this->gettriangle(a));
		
		//if(obj1tritmp.distcheck(other->position, origdist))
			for(unsigned long b = 0; b < other->getnumfaces(); b++) {
				
				POGEL::TRIANGLE obj2tritmp = POGEL::MATRIX(other->position, other->rotation).transformTriangle(other->gettriangle(b));
				
				//if(obj2tritmp.distcheck(this->position, origdist)) {
					for(unsigned int c = 0; c < 3; c++) {
						POGEL::POINT pointtmp1 = obj1tritmp.vertex[c].topoint();
						POGEL::POINT res2d, res3d;
						
						bool col = POGEL::PHYSICS::line_triangle_collision(pointtmp1, pointtmp1+(obj2tritmp.normal.topoint()*origdist)*(obj2tritmp.isinfront(pointtmp1) ? -1.0f : 1.0f), obj2tritmp, &res2d, &res3d);
						if(col && res2d.z < dist) {
							dist = res2d.z;
							*obj1pt = pointtmp1;
							*obj2pt = res3d;
							*tri = obj2tritmp;
							ptcount = 1;
						}
						else if(col && res2d.z == dist) {
							ptcount++;
							//dist = res2d.z;
							*obj1pt += pointtmp1;
							*obj2pt += res3d;
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
									ptcount = 1;
								}
								else if(pointtmp2.distance(pointtmp1) == dist) {
									ptcount++;
									//dist = pointtmp2.distance(pointtmp1);
									*obj1pt += pointtmp1;
									*obj2pt += pointtmp2;
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
							ptcount = 1;
						}
						else if(col && res2d.z == dist) {
							ptcount++;
							//dist = res2d.z;
							*obj2pt += pointtmp2;
							*obj1pt += res3d;
							*tri = obj2tritmp;
						}
						else {
							for(unsigned int d = 0; d < 3; d++) {
								POGEL::POINT pointtmp1 = obj1tritmp.vertex[d].topoint();
								if(pointtmp1.distance(pointtmp2) < dist) {
									dist = pointtmp1.distance(pointtmp2);
									*obj1pt = pointtmp1;
									*obj2pt = pointtmp2;
									*tri = obj2tritmp;
									ptcount = 1;
								}
								else if(pointtmp2.distance(pointtmp1) == dist) {
									ptcount++;
									//dist = pointtmp1.distance(pointtmp2);
									*obj1pt += pointtmp1;
									*obj2pt += pointtmp2;
									*tri = obj2tritmp;
								}
							}
						}
					//}
					
				}
				
			}
	}
	
	if(ptcount != 0) {
		*obj1pt /= (float)ptcount;
		*obj2pt /= (float)ptcount;
	}*/
};

