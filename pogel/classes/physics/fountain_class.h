#ifndef _FOUNTAIN_CLASS_H
#define _FOUNTAIN_CLASS_H

namespace POGEL {
namespace PHYSICS {
class FOUNTAIN;
}
}

#include "physics.h"
//#include "../point_class.h"
#include "solid_class.h"
#include "simulation_class.h"

#define				FOUNTAIN_USE_MASTER_GRAVITY					1
//#define				FOUNTAIN_

class POGEL::PHYSICS::FOUNTAIN : public POGEL::PHYSICS::DYNAMICS, public POGEL::PHYSICS::SOLID {
	private:
		unsigned int properties; // the mushed properties
	protected:
		
	public:
		FOUNTAIN();
		~FOUNTAIN();
		
		PROPERTIES_METHODS;
		
		void increment();
		
		//void step();
		
		void build();
};

#endif /* _FOUNTAIN_CLASS_H */
