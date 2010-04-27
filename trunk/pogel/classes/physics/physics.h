#ifndef _PHYSICS_H
#define _PHYSICS_H

#include "../triangle_class.h"
#include "../../pogel_internals.h"

#define PHYSICS_NUMTRI_PROCESSED 10

//#define PHYSICS_COLLISION_LOGSTATS

namespace POGEL {
namespace PHYSICS {
	
	class SINGULARITY;
	class FAN;
	
	class GRAVITYCLUSTER;
	class FLOW;
	
	class SOLID;
	class SOLIDPHYSICALPROPERTIES;
	
	class DYNAMICS;
	class SIMULATION;
	
	class FOUNTAIN;
	class MICROCOSM;
	
	bool line_triangle_collision(POGEL::POINT, POGEL::POINT, POGEL::TRIANGLE, POGEL::POINT*, POGEL::POINT*);
	bool triangle_collision(POGEL::TRIANGLE, POGEL::TRIANGLE, POGEL::POINT*, POGEL::POINT*);
	bool solid_collision(POGEL::PHYSICS::SOLID*,POGEL::PHYSICS::SOLID*,POGEL::POINT*,POGEL::VECTOR*,POGEL::VECTOR*,float*);
}
}

#endif /* _PHYSICS_H */