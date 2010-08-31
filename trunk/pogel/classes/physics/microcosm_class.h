#ifndef _MICROCOSM_CLASS_H
#define _MICROCOSM_CLASS_H

namespace POGEL {
namespace PHYSICS {
class MICROCOSM;
}
}

#include "physics.h"
//#include "../point_class.h"
#include "solid_class.h"
#include "simulation_class.h"

#define				MICROCOSM_USE_MASTER_GRAVITY				1
#define				MICROCOSM_IGNORE_PARENT_TRANSLATION			2
#define				MICROCOSM_COLLIDE_WITH_PATRONS				4
//#define				MICROCOSM_

class POGEL::PHYSICS::MICROCOSM : public POGEL::PHYSICS::SIMULATION, public POGEL::PHYSICS::SOLID {
	private:
		unsigned int properties; // the mushed properties
	protected:
		
	public:
		MICROCOSM();
		~MICROCOSM();
		
		PROPERTIES_METHODS;
		
		POGEL::VECTOR getpull(POGEL::PHYSICS::SOLID*);
		
		void increment();
		
		void step() {
			increment();
			steptrail();
			getbounding();
		}
		
		void build();
		void draw();
};

#endif /* _MICROCOSM_CLASS_H */
