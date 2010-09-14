#ifndef _PHYSICS_H
#define _PHYSICS_H

#include "../triangle_class.h"
#include "../../pogel_internals.h"

#define PHYSICS_NUMTRI_PROCESSED 1000

//#define PHYSICS_COLLISION_LOGSTATS

#define			PHYSICS_LINESOLID_COLLISION_FIRST			0
#define			PHYSICS_LINESOLID_COLLISION_LEAST			1
#define			PHYSICS_LINESOLID_COLLISION_GREATEST		2


namespace POGEL {
namespace PHYSICS {
	
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
	
	inline float getvprime(float m1, float m2, float v1, float v2);
	void calcElasticDirections(POGEL::PHYSICS::SOLID*, POGEL::PHYSICS::SOLID*, POGEL::VECTOR*);
}
}

#endif /* _PHYSICS_H */
