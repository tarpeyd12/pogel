#ifndef _SIMULATION_CLASS_H
#define _SIMULATION_CLASS_H

namespace POGEL {
namespace PHYSICS {
class SIMULATION;
}
}

#include "physics.h"
#include "dynamics_class.h"
//#include "singularity_class.h"
#include "solid_class.h"

#define				BUPMAX						10

class POGEL::PHYSICS::SIMULATION : public POGEL::PHYSICS::DYNAMICS {
	private:
		unsigned int properties; // the mushed properties
	public:
		float precision;
		bool deactivation;
		
		SIMULATION();
		
		void increment();
		
		bool processcollision(POGEL::PHYSICS::SOLID*,POGEL::PHYSICS::SOLID*);
		
		void reactcollision(POGEL::PHYSICS::SOLID*,POGEL::PHYSICS::SOLID*,POGEL::POINT,POGEL::VECTOR*);
};

#endif /* _SIMULATION_CLASS_H */