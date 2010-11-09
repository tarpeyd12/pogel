#ifndef _PHYSICS_H
#define _PHYSICS_H

#include "../triangle_class.h"
#include "../line_class.h"
#include "../../pogel_internals.h"

#define PHYSICS_NUMTRI_PROCESSED 1000

//#define PHYSICS_COLLISION_LOGSTATS

#define			PHYSICS_LINESOLID_COLLISION_FIRST			0
#define			PHYSICS_LINESOLID_COLLISION_LEAST			1
#define			PHYSICS_LINESOLID_COLLISION_GREATEST		2


namespace POGEL {
namespace PHYSICS {
	
	class BOUNDINGCONTAINER;
	
	class SINGULARITY;
	class FAN;
	
	class GRAVITYCLUSTER;
	class FLOW;
	
	class DYNAMICS;
	class SIMULATION;
	
	class SOLID;
	class SOLIDPHYSICALPROPERTIES;
	
	class FOUNTAIN;
	class MICROCOSM;
	
	bool line_triangle_collision(POGEL::POINT, POGEL::POINT, POGEL::TRIANGLE, POGEL::POINT*, POGEL::POINT*);
	bool triangle_collision(POGEL::TRIANGLE, POGEL::TRIANGLE, POGEL::POINT*, POGEL::POINT*);
	bool solid_line_collision(int, POGEL::PHYSICS::SOLID*, POGEL::POINT, POGEL::POINT, POGEL::TRIANGLE*, POGEL::POINT*, POGEL::POINT*);
	bool solid_line_collision(POGEL::PHYSICS::SOLID*, POGEL::POINT, POGEL::POINT, POGEL::TRIANGLE*, POGEL::POINT*, POGEL::POINT*);
	bool solid_collision(POGEL::PHYSICS::SOLID*,POGEL::PHYSICS::SOLID*,POGEL::POINT*,POGEL::VECTOR*,POGEL::VECTOR*,float*);
	
	// TODO: implement these two methods:
	float line_point_distance(POGEL::POINT, POGEL::LINE, POGEL::POINT*);
	float point_triangle_distance(POGEL::POINT, POGEL::TRIANGLE, POGEL::POINT*);
	
	inline float getvprime(float m1, float m2, float v1, float v2);
	void calcElasticDirections(POGEL::VECTOR, POGEL::PHYSICS::SOLID*, POGEL::PHYSICS::SOLID*, POGEL::VECTOR*);
	
	inline float getvf(float m1, float m2, float v1, float v2, float cr);
	void calcInelasticDirections(POGEL::VECTOR vn, POGEL::PHYSICS::SOLID* s1, POGEL::PHYSICS::SOLID* s2, POGEL::VECTOR* v);
}
}

#endif /* _PHYSICS_H */
