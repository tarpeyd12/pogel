#include "physics.h"
#include "solid_class.h"

#include "../bounding_class.h"
#include "../matrix_class.h"
#include "../point_class.h"

#include "collision.h"

bool POGEL::PHYSICS::line_triangle_collision(POGEL::POINT start, POGEL::POINT end, POGEL::TRIANGLE triangle, POGEL::POINT* collision2d, POGEL::POINT* collision3d) {
	float orig[] = {start.x, start.y, start.z};
	POGEL::VECTOR vct;
	vct.frompoints(start,end);
	vct.normalize();
	float dir[] = {vct.x, vct.y, vct.z};
	
	// put the verticies into arrays so that the collision function can read them
	float vert0[] = {triangle.vertex[0].x, triangle.vertex[0].y, triangle.vertex[0].z};
	float vert1[] = {triangle.vertex[1].x, triangle.vertex[1].y, triangle.vertex[1].z};
	float vert2[] = {triangle.vertex[2].x, triangle.vertex[2].y, triangle.vertex[2].z};
	
	// process the collision
	bool result = (bool)intersect_triangle(orig, dir, vert0, vert1, vert2, &collision2d->x, &collision2d->y, &collision2d->z);
	//vct.normalize();
	
	collision2d->x = fabs(collision2d->x);
	
	*collision3d = start + vct.topoint() * collision2d->x;
	
	#ifdef PHYSICS_COLLISION_LOGSTATS
		if(result && (collision2d->x <= start.distance(end)))
			POGEL::logtofile("\t\ttriangle_line_hit, dist = %7.3f", collision2d->x);
		else
			POGEL::logtofile("\t\tno, dist = %7.3f", collision2d->x);
	#endif /* PHYSICS_COLLISION_LOGSTATS */
	
	return result && (collision2d->x <= start.distance(end));// && Pa+Pb+Pc == 1.0;
};

bool POGEL::PHYSICS::triangle_collision(POGEL::TRIANGLE tria, POGEL::TRIANGLE trib, POGEL::POINT* p1, POGEL::POINT* p2) {
	//printf("\nhello\n");
	POGEL::POINT p3d[6], p2d[6];
	bool collided[6];
	int cols = 0;
	for(int a = 0 ; a < 6 && cols < 2 ; a++ ) {
		if(a<3) // check the rays of the first triangle, agenst the plane of the second triangle
			collided[a]=POGEL::PHYSICS::line_triangle_collision(tria.vertex[(a+0)%3].topoint(),tria.vertex[(a+1)%3].topoint(),trib,&p2d[a],&p3d[a]);
		else    // check the rays of the second triangle, agenst the plane of the first triangle
			collided[a]=POGEL::PHYSICS::line_triangle_collision(trib.vertex[(a+0)%3].topoint(),trib.vertex[(a+1)%3].topoint(),tria,&p2d[a],&p3d[a]);
		if(collided[a])
			cols++;
		if(cols == 2)
			break;
	}
	if(cols == 2)
		for(int a=0;a<6;a++)
			if(collided[a])
				for(int b=0;b<6;b++)
					if(a!=b && collided[b]) {
						(*p1) = p3d[a];
						(*p2) = p3d[b];
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

bool POGEL::PHYSICS::solid_collision(POGEL::PHYSICS::SOLID* obj1, POGEL::PHYSICS::SOLID* obj2, POGEL::POINT* contact, POGEL::VECTOR* normal1, POGEL::VECTOR* normal2, float *area) {
	POGEL::POINT p1, p2;
	POGEL::TRIANGLE t1, t2;
	unsigned long numcols = 0;
	unsigned long triprocess1 = 0, triprocess2 = 0;
	bool ret = false;
	*area = 0.0f;
	POGEL::POINT obj1cols;//, obj2cols;
	//POGEL::MATRIX mat;
	//mat.get();
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
		//triprocess2 = 0;
		for(unsigned long b=0;b<obj2->numfaces;b++) {
			if(triprocess2 >= PHYSICS_NUMTRI_PROCESSED)
				break;
			t2 = obj2->face[b].transform(&transformmat2);
			POGEL::POINT t2mid = t2.middle();
			t2.getbounding();
			//t2.bounding.draw(POGEL::POINT());
			
			//if( obj1->bounding.checkbounding(obj1->position, POGEL::POINT(), t2.bounding) || obj2->bounding.checkbounding(obj2->position, POGEL::POINT(), t1.bounding) )
			if( t1.bounding.checkbounding(POGEL::POINT(), POGEL::POINT(), t2.bounding) && t2.bounding.checkbounding(POGEL::POINT(), POGEL::POINT(), t1.bounding) )
			if(POGEL::PHYSICS::triangle_collision(t1, t2, &p1, &p2)) {
				// *contact += (p1+p2)/2.0f;
				
				triprocess1++;
				triprocess2++;
				
				*area += p1.distance(p2);
				
				if(POGEL::hasproperty(POGEL_COLLISIONS)) {
					glDisable(GL_TEXTURE_2D);
					glDisable(GL_LIGHTING);
					glLineWidth(5);
					glColor3f(0.0f,1.0f,1.0f);
					glBegin(GL_LINES);
						glVertex3f(p1.x,p1.y,p1.z);
						glVertex3f(p2.x,p2.y,p2.z);
					glEnd();
					glLineWidth(1);
					glColor3f(1.0f,1.0f,1.0f);
					glEnable(GL_LIGHTING);
					glEnable(GL_TEXTURE_2D);
				}
				
				obj1cols += ( mat1.transformPoint(p1) + mat1.transformPoint(p2) )/2.0f;
				//obj2cols += ( mat2.transformPoint(p1) + mat2.transformPoint(p2) )/2.0f;
				
				*normal1 += ( t1.isinfront(obj2->position) ? t1.normal : t1.normal*-1.0f );
				*normal2 += ( t2.isinfront(obj1->position) ? t2.normal : t2.normal*-1.0f );
				if(!ret) ret = true;
				numcols++;
				//return true;
			}
		}
	}
	if(numcols > 0) {
		
		obj1cols /= (float)numcols;
		//obj2cols /= (float)numcols;
		
		/* make it that the normals are the actual normals of the facing triangles
		vector from pos1 to col, collision point with triangle of obj2 ect. */
		
		/**normal1 /= (float)numcols;
		*normal2 /= (float)numcols;*/
		
		transformmat1.transformPoint(&obj1cols);
		//transformmat2.transformPoint(&obj2cols);
		
		*contact = (obj1cols/*+obj2cols*/)/1.0f;
		
		// *contact /= (float)numcols;
		
		//transformmat1.transformVector(normal1);
		//transformmat2.transformVector(normal2);
	}
	//return false;
	return ret;
};
