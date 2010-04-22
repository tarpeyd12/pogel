#ifndef _SOLID_CLASS_H
#define _SOLID_CLASS_H

namespace POGEL {
namespace PHYSICS {
class SOLIDPHYSICALPROPERTIES;
class SOLID;
}
}

#include "physics.h"
#include "../point_class.h"
#include "../object_class.h"

#define			PHYSICS_SOLID_STATIONARY		1
#define			PHYSICS_SOLID_VOLITAL			2
//#define			PHYSICS_SOLID_

#define			PHYSICS_SOLID_TRAILSIZE			25
#define			PHYSICS_SOLID_TRAILINDEX		(PHYSICS_SOLID_TRAILSIZE/10)

#define			SOLID_DISPLAY_TRAIL_FADING
//#define			SOLID_DISPLAY_ROTATION_TRAIL
//#define			SOLID_DISPLAY_NEGATIVE_ROTATION_TRAIL
//#define			SOLID_DISPLAY_STIPPLED_NEGATIVE_ROTATION_TRAIL

#define			SOLID_FNC_DEF					POGEL::PHYSICS::SOLID* obj, char* othername

class POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES {
	public:
		float friction;
		float bounce;
		float mass;
		float air_friction;
		float dencity;
		float volume;
		
		bool magnetic;
		float charge;
		
		SOLIDPHYSICALPROPERTIES() {
			friction=1.0f; bounce=1.0f; mass=1.0f; air_friction=1.0f; dencity=1.0f; volume=1.0f; magnetic=false; charge=0.0f;
		}
		SOLIDPHYSICALPROPERTIES(float f, float b, float m, float a, float d, float v, bool mg, float c) {
			friction=f; bounce=b; mass=m; air_friction=a; dencity=d; volume=v; magnetic=mg; charge=c;
		}
};

class POGEL::PHYSICS::SOLID : public POGEL::OBJECT {
	private:
		unsigned int physproperties;
	protected:
		float maximumdistance;
		POGEL::POINT *trail;
		POGEL::POINT *rots;
		
		POGEL::PHYSICS::DYNAMICS* container;
		
		void (*callback)(POGEL::PHYSICS::SOLID*,char*);
	public:
		POGEL::BOUNDING bounding;
		POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES behavior;
		
		SOLID();
		SOLID(POGEL::OBJECT*, POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES, unsigned int);
		~SOLID();
		
		//PROPERTIES_METHODS;
		
		unsigned int getOptions() {return physproperties;}
		void setOptions(unsigned int prop) {physproperties=prop;}
		void addOption(unsigned int prop) {physproperties|=prop;}
		void removeOption(unsigned int prop) {physproperties^=prop;}
		bool hasOption(unsigned int prop) {return (physproperties & prop);}
		
		void setCallback(void (*func)(POGEL::PHYSICS::SOLID*,char*) )
			{ callback = func; }
		
		void steprottrail();
		void steppostrail();
		void steptrail();
		
		bool sameposlegacy(float);
		bool samerotlegacy(float);
		bool samelegacy(float);
		
		void getbounding();
		
		void build();
		void draw();
		
		void step() {
			increment();
			steptrail();
			getbounding();
		}
		
		friend class POGEL::PHYSICS::DYNAMICS;
		friend class POGEL::PHYSICS::SIMULATION;
		
		friend class POGEL::PHYSICS::FOUNTAIN;
		friend class POGEL::PHYSICS::MICROCOSM;
		
		friend bool POGEL::PHYSICS::solid_collision(POGEL::PHYSICS::SOLID*,POGEL::PHYSICS::SOLID*,POGEL::POINT*,float*);
};

#endif /* _SOLID_CLASS_H */
