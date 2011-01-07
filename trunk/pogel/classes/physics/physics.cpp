#include "physics.h"
#include "solid_class.h"

#include "../bounding_class.h"
#include "../matrix_class.h"
#include "../point_class.h"
//#include "../line_class.h"

#include "collision.h"

bool POGEL::PHYSICS::line_triangle_collision(POGEL::POINT start, POGEL::POINT end, POGEL::TRIANGLE triangle, POGEL::POINT* collision2d, POGEL::POINT* collision3d) {
	float orig[] = { start.x, start.y, start.z };
	POGEL::VECTOR vct(start,end);
	vct.normalize();
	float dir[] = { vct.x, vct.y, vct.z };
	
	// put the verticies into arrays so that the collision function can read them
	float vert0[] = {triangle.vertex[0].x, triangle.vertex[0].y, triangle.vertex[0].z};
	float vert1[] = {triangle.vertex[1].x, triangle.vertex[1].y, triangle.vertex[1].z};
	float vert2[] = {triangle.vertex[2].x, triangle.vertex[2].y, triangle.vertex[2].z};
	
	// process the collision
	bool result = (bool)intersect_triangle(orig, dir, vert0, vert1, vert2, &collision2d->z, &collision2d->x, &collision2d->y);
	
	// make it positive, for some reason it comes out negative sometimes.
	collision2d->z = fabs(collision2d->z);
	
	// get the 3d position of the collision
	*collision3d = start + vct.topoint() * collision2d->z;
	
	#ifdef PHYSICS_COLLISION_LOGSTATS
		if(result && (collision2d->z <= start.distance(end)))
			POGEL::logtofile("\t\ttriangle_line_hit, dist = %7.3f", collision2d->z);
		else
			POGEL::logtofile("\t\tno, dist = %7.3f", collision2d->z);
	#endif /* PHYSICS_COLLISION_LOGSTATS */
	
	return result && (collision2d->z <= start.distance(end));
};

bool POGEL::PHYSICS::triangle_collision(POGEL::TRIANGLE tria, POGEL::TRIANGLE trib, POGEL::POINT* p1, POGEL::POINT* p2) {
	POGEL::POINT p3d[6], p2d[6];
	bool collided[6];
	int cols = 0;
	for(int a = 0 ; a < 6 && cols < 2 ; a++ ) {
		if(a<3)
			// check the rays of the first triangle, agenst the plane of the second triangle
			collided[a]=POGEL::PHYSICS::line_triangle_collision(tria.vertex[(a+0)%3].topoint(),tria.vertex[(a+1)%3].topoint(),trib,&p2d[a],&p3d[a]);
		else
			// check the rays of the second triangle, agenst the plane of the first triangle
			collided[a]=POGEL::PHYSICS::line_triangle_collision(trib.vertex[(a+0)%3].topoint(),trib.vertex[(a+1)%3].topoint(),tria,&p2d[a],&p3d[a]);
		
		if(collided[a])
			cols++;
		if(cols == 2)
			break;
	}
	
	// the number of total collisions must be two, can't be anything else.
	if(cols == 2)
		for( int a = 0; a < 6; a++ ) // go through the recorded collisions
			if(collided[a]) // if one of them collided
				for( int b = (a+1); b < 6; b++ ) // go through the rest
					// checking if one of the rest hit
					if(collided[b]) {
						// put the collision points into the pointers
						*p1 = p3d[a];
						*p2 = p3d[b];
						#ifdef PHYSICS_COLLISION_LOGSTATS
							POGEL::logtofile("\tcollision occurence");
						#endif /* PHYSICS_COLLISION_LOGSTATS */
						return true;
					}
	#ifdef PHYSICS_COLLISION_LOGSTATS
		POGEL::logtofile("\tno occurence");
	#endif /* PHYSICS_COLLISION_LOGSTATS */
	return false;
};

bool POGEL::PHYSICS::solid_line_collision(int type, POGEL::PHYSICS::SOLID* obj, POGEL::POINT start, POGEL::POINT end, POGEL::TRIANGLE* tri, POGEL::POINT* col2d, POGEL::POINT* col3d) {
	POGEL::MATRIX mat = POGEL::MATRIX(obj->position, obj->rotation);
	
	if(type == PHYSICS_LINESOLID_COLLISION_GREATEST) {
		float dist = 0.0f;
		int index = 0;
		bool ret = false;
		for(unsigned long i = 0; i < obj->getnumfaces(); i++)
			if(POGEL::PHYSICS::line_triangle_collision(start, end, obj->gettriangle(i).transform(&mat), col2d, col3d)) {
				if(col2d->z > dist) {
					*tri = obj->gettriangle(i).transform(&mat);
					dist = col2d->z;
					index = i;
				}
				ret = true;
			}
		return ret;
	}
	
	else if(type == PHYSICS_LINESOLID_COLLISION_LEAST) {
		float dist = obj->bounding.maxdistance;
		int index = 0;
		bool ret = false;
		for(unsigned long i = 0; i < obj->getnumfaces(); i++)
			if(POGEL::PHYSICS::line_triangle_collision(start, end, obj->gettriangle(i).transform(&mat), col2d, col3d)) {
				if(col2d->z < dist) {
					*tri = obj->gettriangle(i).transform(&mat);
					dist = col2d->z;
					index = i;
				}
				ret = true;
			}
		return ret;
	}
	
	else if(type == PHYSICS_LINESOLID_COLLISION_FIRST) {
		for(unsigned long i = 0; i < obj->getnumfaces(); i++)
			if(POGEL::PHYSICS::line_triangle_collision(start, end, obj->gettriangle(i).transform(&mat), col2d, col3d)) {
				*tri = obj->gettriangle(i).transform(&mat);
				return true;
			}
		return false;
	}
	
	else {
		// TODO: put warning here
		for(unsigned long i = 0; i < obj->getnumfaces(); i++)
			if(POGEL::PHYSICS::line_triangle_collision(start, end, obj->gettriangle(i).transform(&mat), col2d, col3d)) {
				*tri = obj->gettriangle(i).transform(&mat);
				return true;
			}
		return false;
	}
	
	return false;
};

bool POGEL::PHYSICS::solid_line_collision(POGEL::PHYSICS::SOLID* obj, POGEL::POINT start, POGEL::POINT end, POGEL::TRIANGLE* tri, POGEL::POINT* col2d, POGEL::POINT* col3d) {
	POGEL::MATRIX mat = POGEL::MATRIX(obj->position, obj->rotation);
	for(unsigned long i = 0; i < obj->getnumfaces(); i++)
		if(POGEL::PHYSICS::line_triangle_collision(start, end, obj->gettriangle(i).transform(&mat), col2d, col3d)) {
			*tri = obj->gettriangle(i).transform(&mat);
			return true;
		}
	return false;
};

// TODO: clean this up a bit:
bool POGEL::PHYSICS::solid_collision(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2, POGEL::POINT* contact, POGEL::VECTOR* normal1, POGEL::VECTOR* normal2, float *area) {
	POGEL::POINT p1, p2;
	POGEL::TRIANGLE t1, t2;
	POGEL::TRIANGLE obj1colidedtris[PHYSICS_NUMTRI_PROCESSED], obj2colidedtris[PHYSICS_NUMTRI_PROCESSED];
	unsigned long numcols = 0;
	unsigned long triprocess1 = 0, triprocess2 = 0;
	bool ret = false;
	*area = 0.0f;
	POGEL::POINT obj1cols;//, obj2cols;
	
	POGEL::MATRIX mat1(obj1->position*-1.0f, obj1->rotation*-1.0f);//, mat2(obj2->position*-1.0f, obj2->rotation*-1.0f);
	POGEL::MATRIX transformmat1(obj1->position, obj1->rotation);
	POGEL::MATRIX transformmat2(obj2->position, obj2->rotation);
	
	obj1->getbounding();
	obj2->getbounding();
	
	for(unsigned long a=0;a<obj1->numfaces;a++) {
		if(triprocess1 >= PHYSICS_NUMTRI_PROCESSED/* || triprocess2 >= PHYSICS_NUMTRI_PROCESSED*/)
			break;
		t1 = obj1->face[a].transform(&transformmat1);
		POGEL::POINT t1mid = t1.middle();
		t1.getbounding();
		t1.bounding.draw(POGEL::POINT());
		
		for(unsigned long b=0;b<obj2->numfaces;b++) {
			if(triprocess2 >= PHYSICS_NUMTRI_PROCESSED)
				break;
			t2 = obj2->face[b].transform(&transformmat2);
			POGEL::POINT t2mid = t2.middle();
			t2.getbounding();

			if( t1.bounding.checkbounding(POGEL::POINT(), POGEL::POINT(), t2.bounding) && t2.bounding.checkbounding(POGEL::POINT(), POGEL::POINT(), t1.bounding) )
			if(POGEL::PHYSICS::triangle_collision(t1, t2, &p1, &p2)) {
				
				obj1colidedtris[triprocess1] = t1;
				triprocess1++;
				obj2colidedtris[triprocess2] = t2;
				triprocess2++;
				
				*area += p1.distance(p2);
				
				if(POGEL::hasproperty(POGEL_COLLISIONS))
					POGEL::LINE(p1,p2,5,POGEL::COLOR(0,1,1,1)).draw();
				
				obj1cols += ( mat1.transformPoint(p1) + mat1.transformPoint(p2) )/2.0f;
				//obj2cols += ( mat2.transformPoint(p1) + mat2.transformPoint(p2) )/2.0f;
				
				if(!ret) ret = true;
				numcols++;
				//return true;
			}
		}
	}
	if(numcols > 0) {
		
		obj1cols /= (float)numcols;
		//obj2cols /= (float)numcols;
		
		transformmat1.transformPoint(&obj1cols);
		//transformmat2.transformPoint(&obj2cols);
		
		*contact = obj1cols;
		
		POGEL::POINT ptmp1, ptmp2;
		for(unsigned long i = 0; i < PHYSICS_NUMTRI_PROCESSED && i < triprocess1 && i < triprocess2 ; i++) {
			
			POGEL::VECTOR v1 = POGEL::VECTOR(obj1->position, *contact)*10.0f;
			if(POGEL::PHYSICS::line_triangle_collision(obj1->position, obj1->position+v1.topoint(), obj2colidedtris[i], &ptmp1, &ptmp2))
				*normal2 += ( obj2colidedtris[i].isinfront(obj1->position) ? obj2colidedtris[i].normal : obj2colidedtris[i].normal*-1.0f );
			
			POGEL::VECTOR v2 = POGEL::VECTOR(obj2->position, *contact)*10.0f;
			if(POGEL::PHYSICS::line_triangle_collision(obj2->position, obj2->position+v2.topoint(), obj1colidedtris[i], &ptmp1, &ptmp2))
				*normal1 += ( obj1colidedtris[i].isinfront(obj2->position) ? obj1colidedtris[i].normal : obj1colidedtris[i].normal*-1.0f );
				
		}
		normal1->normalize();
		normal2->normalize();
	}
	return ret;
};

// this is brute force
// TODO: find a mathematical way to do this quickly and acuratly for huge lines
float POGEL::PHYSICS::line_point_distance(POGEL::POINT point, POGEL::LINE line, POGEL::POINT* pol) {
	POGEL::POINT start = line.getStart();
	POGEL::POINT end = line.getEnd();
	POGEL::POINT middle = line.getMiddle();
	POGEL::POINT closest = middle;
	
	float pdist = 0;
	float curdist = point.distance(middle);
	unsigned int i = 0;
	while( pdist != curdist && i++ < 10) {
		if(point.distance(start)+point.distance(middle) < point.distance(end)+point.distance(middle)) {
			if(point.distance(start) < point.distance(closest)) closest = start;
			else closest = middle;
			pdist = curdist; curdist = point.distance(start)+point.distance(closest);
			end = middle; middle = (start+end)/2;
		}
		else {
			if(point.distance(end) < point.distance(closest)) closest = end;
			else closest = middle;
			pdist = curdist; curdist = point.distance(end)+point.distance(closest);
			start = middle; middle = (start+end)/2;
		}
		//middle.draw();
	}
	if(pol != NULL) *pol = closest;
	//closest.draw();
	//POGEL::LINE(point, closest, 1, POGEL::COLOR(0,0,1,1)).draw();
	return point.distance(closest);
};

// this is a brute force and direct way
float POGEL::PHYSICS::point_triangle_distance(POGEL::POINT point, POGEL::TRIANGLE triangle, POGEL::POINT* pol) {
	
	POGEL::POINT pointtmp1 = point;
	POGEL::POINT res2d, res3d;
	
	bool col = POGEL::PHYSICS::line_triangle_collision(
		point,
		point+(triangle.normal*point.distance(triangle.middle()))*(triangle.isinfront(point) ? -1 : 1), \
		triangle, &res2d, &res3d
	);
	
	if(col) {
		if(pol != NULL) *pol = res3d;
		//POGEL::LINE(point, res3d, 1, POGEL::COLOR(0,0,1,1)).draw();
		return res2d.z;
	}
	
	POGEL::LINE edges[3];
	POGEL::POINT verts[3];
	for(unsigned int i = 0; i< 3; i++) {
		edges[i] = triangle.getEdge(i);
		verts[i] = triangle.vertex[i].topoint();
	}
	
	for(unsigned int i = 0; i< 3; i++)
		if(point.distance(verts[(i+0)%3]) < point.distance(verts[(i+1)%3]) && point.distance(verts[(i+0)%3]) < point.distance(verts[(i+2)%3])) {
			POGEL::LINE a = edges[(i+0)%3];
			POGEL::LINE b = edges[(i+2)%3];
			POGEL::POINT tmp1, tmp2;
			float dist1 = POGEL::PHYSICS::line_point_distance(point, a, &tmp1);
			float dist2 = POGEL::PHYSICS::line_point_distance(point, b, &tmp2);
			
			if(dist1 <= dist2) {
				if(pol != NULL) *pol = tmp1;
				//POGEL::LINE(point, tmp1, 1, POGEL::COLOR(0,0,1,1)).draw();
				return dist1;
			}
			else {
				if(pol != NULL) *pol = tmp2;
				//POGEL::LINE(point, tmp2, 1, POGEL::COLOR(0,0,1,1)).draw();
				return dist2;
			}
		}
	
	return point.distance(triangle.middle());
};

inline float POGEL::PHYSICS::getvprime(float m1, float m2, float v1, float v2) {
	return (v1*(m1-m2)+(2*m2*v2))/(m1+m2);
};

void POGEL::PHYSICS::calcElasticDirections(POGEL::VECTOR vn, POGEL::PHYSICS::SOLID* s1, POGEL::PHYSICS::SOLID* s2, POGEL::VECTOR* v) {
	
	POGEL::VECTOR un;
	
	//vn.print();
	
	if(vn == POGEL::VECTOR())
		un.frompoints(s2->position, s1->position);
	else
		un = vn;
	
	float v1prime, v2prime;
	float v1n, v1t, v2n, v2t;
	
	un.normalize();
	
	// check to see if the objects can be treated as a 1 dimensional collision
	if(
		s1->direction.normal() == un || \
		s1->direction.normal()*-1.0f == un || \
		s2->direction.normal() == un || \
		s2->direction.normal()*-1.0f == un
	) {
		v1n = s1->direction.getdistance();
		v2n = s2->direction.getdistance();
		
		v1prime = POGEL::PHYSICS::getvprime(s1->behavior.mass, s2->behavior.mass, v1n, v2n);
		v2prime = POGEL::PHYSICS::getvprime(s2->behavior.mass, s1->behavior.mass, v2n, v1n);
		
		v[0] = s2->direction.normal() * v1prime;
		v[1] = s1->direction.normal() * v2prime;
		
		return;
	}
	
	POGEL::VECTOR ut1(s1->direction, un), ut2(s2->direction, un);
	
	ut1.normalize();
	ut2.normalize();
	
	v1n = un.dotproduct(s1->direction);
	v1t = ut1.dotproduct(s1->direction);
	
	v2n = un.dotproduct(s2->direction);
	v2t = ut2.dotproduct(s2->direction);
	
	v1prime = POGEL::PHYSICS::getvprime(s1->behavior.mass, s2->behavior.mass, v1n, v2n);
	v2prime = POGEL::PHYSICS::getvprime(s2->behavior.mass, s1->behavior.mass, v2n, v1n);
	
	POGEL::VECTOR v1nprimevector, v2nprimevector, v1tprimevector, v2tprimevector;
	
	v1nprimevector = un*v1prime;
	v2nprimevector = un*v2prime;
	
	v1tprimevector = s1->direction - un * s1->direction.dotproduct(un);
	v2tprimevector = s2->direction - un * s2->direction.dotproduct(un);
	
	v[0] = (v1nprimevector + v1tprimevector);
	v[1] = (v2nprimevector + v2tprimevector);
};

inline float POGEL::PHYSICS::getvf(float m1, float m2, float v1, float v2, float cr) {
	return (cr*m2*(v2-v1)+m1*v1+m2*v2)/(m1+m2);
};

void POGEL::PHYSICS::calcInelasticDirections(POGEL::VECTOR vn, POGEL::PHYSICS::SOLID* s1, POGEL::PHYSICS::SOLID* s2, POGEL::VECTOR* v) {
	
	// these next two lines of code are to compensate for a weard bug, that when the bounce of a solid object that is PHYSICS_SOLID_CONVEX, is 4, and the other objects bounce is 1, that then it is as if both were set to 1.
	float b1 = s1->behavior.bounce *(s1->hasOption(PHYSICS_SOLID_CONCAVE) && s1->hasOption(PHYSICS_SOLID_SPHERE) ? 5 : 1);
	float b2 = s2->behavior.bounce *(s2->hasOption(PHYSICS_SOLID_CONCAVE) && s2->hasOption(PHYSICS_SOLID_SPHERE) ? 5 : 1);
	
	float cn = (b1+b2)/2;
	float cn1 = cn;
	float cn2 = cn;
	
	POGEL::VECTOR un;
	
	//vn.print();
	
	//if(vn == POGEL::VECTOR())
		//un.frompoints(s2->position, s1->position);
	//else
		un = vn;
	
	float v1prime, v2prime;
	float v1n, v1t, v2n, v2t;
	
	un.normalize();
	
	// check to see if the objects can be treated as a 1 dimensional collision
	if(
		s1->direction.normal() == un || \
		s1->direction.normal()*-1.0f == un || \
		s2->direction.normal() == un || \
		s2->direction.normal()*-1.0f == un
	) {
		v1n = s1->direction.getdistance();
		v2n = s2->direction.getdistance();
		
		v1prime = POGEL::PHYSICS::getvf(s1->behavior.mass, s2->behavior.mass, v1n, v2n, cn1);
		v2prime = POGEL::PHYSICS::getvf(s2->behavior.mass, s1->behavior.mass, v2n, v1n, cn2);
		
		v[0] = s2->direction.normal() * v1prime;
		v[1] = s1->direction.normal() * v2prime;
		
		return;
	}
	
	POGEL::VECTOR ut1(s1->direction, un), ut2(s2->direction, un);
	
	ut1.normalize();
	ut2.normalize();
	
	v1n = un.dotproduct(s1->direction);
	v1t = ut1.dotproduct(s1->direction);
	
	v2n = un.dotproduct(s2->direction);
	v2t = ut2.dotproduct(s2->direction);
	
	v1prime = POGEL::PHYSICS::getvf(s1->behavior.mass, s2->behavior.mass, v1n, v2n, cn1);
	v2prime = POGEL::PHYSICS::getvf(s2->behavior.mass, s1->behavior.mass, v2n, v1n, cn2);
	
	POGEL::VECTOR v1nprimevector, v2nprimevector, v1tprimevector, v2tprimevector;
	
	v1nprimevector = un*v1prime;
	v2nprimevector = un*v2prime;
	
	v1tprimevector = s1->direction - un * s1->direction.dotproduct(un);
	v2tprimevector = s2->direction - un * s2->direction.dotproduct(un);
	
	v[0] = (v1nprimevector + v1tprimevector);
	v[1] = (v2nprimevector + v2tprimevector);
};



