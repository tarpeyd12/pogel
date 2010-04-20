#ifndef _SINGULARITY_CLASS_H
#define _SINGULARITY_CLASS_H

#include <stdlib.h>

namespace POGEL {
namespace PHYSICS {
class SINGULARITY;
class FAN;
class FLOW;
class GRAVITYCLUSTER;
}
}

#include "physics.h"
#include "../point_class.h"

class POGEL::PHYSICS::SINGULARITY {
	public:
		bool active;
		POGEL::POINT center;
		float intencity;
		
		SINGULARITY() {center=POGEL::POINT(); intencity=0.0f; active=true;}
		SINGULARITY(POGEL::POINT p, float i) {center=p; intencity=i; active=true;}
		SINGULARITY(float x, float y, float z, float i) {center=POGEL::POINT(x,y,z); intencity=i; active=true;}
		
		virtual POGEL::VECTOR getpull(POGEL::POINT p, float mass) {
			if(active) {
				float dist = p.distance(center);
				POGEL::VECTOR v;
				if(p==center)
					return POGEL::VECTOR();
				else
					v.frompoints(p, center);
				v.normalize();
				v *= intencity*mass * 0.0000000000667;
				v /= (dist*dist);
				return v;
			}
			return POGEL::VECTOR();
		}
};

class POGEL::PHYSICS::FAN : public POGEL::PHYSICS::SINGULARITY {
	public:
		POGEL::VECTOR direction;
		
		FAN() : SINGULARITY()
			{ direction=POGEL::VECTOR(); }
		FAN(POGEL::POINT p, POGEL::VECTOR v, float i) : SINGULARITY(p,i)
			{ direction=v; }
		FAN(float x, float y, float z, POGEL::VECTOR v, float i) : SINGULARITY(x,y,z,i)
			{ direction=v; }
		FAN(POGEL::POINT p, float x, float y, float z, float i) : SINGULARITY(p,i)
			{ direction=POGEL::VECTOR(x,y,z); }
		FAN(float x, float y, float z, float vx, float vy, float vz, float i) : SINGULARITY(x,y,z,i)
			{ direction=POGEL::VECTOR(vx,vy,vz); }
		
		POGEL::VECTOR getpull(POGEL::POINT p, float mass) {
			if(active)
				return ((direction*intencity)/p.distance(center))/(mass+1.0f);
			return POGEL::VECTOR();
		}
};

class POGEL::PHYSICS::GRAVITYCLUSTER {
		POGEL::PHYSICS::SINGULARITY* singularities;
		unsigned long numsingularities;
	public:
		
		GRAVITYCLUSTER()
			{ singularities=(POGEL::PHYSICS::SINGULARITY*)NULL; numsingularities = 0; }
		~GRAVITYCLUSTER()
			{ free(singularities); }
		
		unsigned long addsingularity(POGEL::PHYSICS::SINGULARITY);
		void addsingularities(POGEL::PHYSICS::SINGULARITY*,unsigned long);
		
		POGEL::VECTOR getpull(POGEL::POINT p, float mass) {
			if(numsingularities==0)
				return POGEL::VECTOR();
			POGEL::VECTOR v;
			//unsigned int diff=0;
			for(unsigned int i=0;i<numsingularities;i++)
				//if(singularities[i].active)
					v+=singularities[i].getpull(p, mass);
				//else
					//diff++;
			//if(diff<numsingularities)
				return v;//((float)numsingularities - (float)diff);
			//else
				//return POGEL::VECTOR();
		}
};

class POGEL::PHYSICS::FLOW {
		POGEL::PHYSICS::FAN* gusts;
		unsigned long numgusts;
	public:
		
		FLOW() {gusts=(POGEL::PHYSICS::FAN*)NULL; numgusts = 0; }
		FLOW(POGEL::POINT* waypoints,unsigned long num,bool forwards) {generatecurve(waypoints,num,forwards);}
		~FLOW() {free(gusts);}
		
		unsigned long addfan(POGEL::PHYSICS::FAN);
		void addfans(POGEL::PHYSICS::FAN*,unsigned long);
		
		void generatecurve(POGEL::POINT*,unsigned long,bool);
		
		POGEL::VECTOR getpull(POGEL::POINT p, float mass) {
			if(numgusts==0)
				return POGEL::VECTOR();
			POGEL::VECTOR v;
			unsigned int diff=0;
			for(unsigned int i=0;i<numgusts;i++)
				if(gusts[i].active)
					v=v+gusts[i].getpull(p, mass);
				else
					diff++;
			if(diff<numgusts)
				return v/((float)numgusts - (float)diff);
			else
				return POGEL::VECTOR();
		}
};

#endif /* _SINGULARITY_CLASS_H */

