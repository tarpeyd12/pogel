#ifndef _DYNAMICS_CLASS_H
#define _DYNAMICS_CLASS_H

namespace POGEL {
namespace PHYSICS {
class DYNAMICS;
}
}

#include "physics.h"
#include "singularity_class.h"
#include "solid_class.h"

#define				DYNAMICS_HAS_MAGNETIC_OBJECT			1

class POGEL::PHYSICS::DYNAMICS {
	private:
		unsigned int properties; // the mushed properties
	protected:
		POGEL::PHYSICS::SOLID** objects;
		unsigned long numobjects;
		
		POGEL::PHYSICS::GRAVITYCLUSTER objectmasses;
		
		POGEL::PHYSICS::GRAVITYCLUSTER singularities;
		POGEL::PHYSICS::FLOW gusts;
		
		//POGEL::PHYSICS::FLOW** currents;
		//unsigned long numcurrents;
		
		//POGEL::PHYSICS::GRAVITYCLUSTER** ;
		//unsigned long ;
	public:
		unsigned long boundingskips;
		
		POGEL::VECTOR gravity;
		float air_dencity;
		
		DYNAMICS();
		~DYNAMICS() {
			if(objects) {
				//for(unsigned long i=0;i<numobjects;i++)
					//delete objects[i];
				delete[] objects;
			}
			//for(unsigned long i=0;i<numcurrents;i++)
				//delete currents[i];
			//delete[] currents;
		}
		
		PROPERTIES_METHODS;
		
		unsigned long addsingularity(POGEL::PHYSICS::SINGULARITY sig) {return singularities.addsingularity(sig);}
		void addsingularities(POGEL::PHYSICS::SINGULARITY* sig, unsigned long num) {singularities.addsingularities(sig,num);}
		
		unsigned long addfan(POGEL::PHYSICS::FAN fan) {return gusts.addfan(fan);}
		void addfans(POGEL::PHYSICS::FAN* fan, unsigned long num) {gusts.addfans(fan,num);}
		
		unsigned long addSolid(POGEL::PHYSICS::SOLID*);
		void addSolids(POGEL::PHYSICS::SOLID**,unsigned long);
		
		virtual POGEL::VECTOR getpull(POGEL::PHYSICS::SOLID*);
		
		virtual void increment();
		
		virtual void draw();
		
		friend class POGEL::PHYSICS::SIMULATION;
};

#endif /* _SIMULATION_CLASS_H */
