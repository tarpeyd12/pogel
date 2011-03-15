#include <string.h>

#include "singularity_class.h"

unsigned long POGEL::PHYSICS::FLOW::addfan(POGEL::PHYSICS::FAN fan) {
	gusts.add(fan);
	return numgusts++;
};

void POGEL::PHYSICS::FLOW::addfans(POGEL::PHYSICS::FAN *fan, unsigned long num) {
	if(fan == (POGEL::PHYSICS::FAN*)NULL)
		POGEL::fatality(POGEL_FATALITY_NULL_ARRAY_POINTER_RETNUM,"%s to Fan(s).",POGEL_FATALITY_NULL_ARRAY_POINTER_STRING);
	for(unsigned long i=0;i<num;i++) addfan(fan[i]);
};

void POGEL::PHYSICS::FLOW::generatecurve(POGEL::POINT* waypoints, unsigned long num, bool forwards) {
	if(waypoints == (POGEL::POINT*)NULL)
		POGEL::fatality(POGEL_FATALITY_NULL_ARRAY_POINTER_RETNUM,"%s to Point(s).",POGEL_FATALITY_NULL_ARRAY_POINTER_STRING);
	for(unsigned long i=0;i<num;i++) {
		POGEL::VECTOR v;
		v.frompoints(waypoints[abs(i+(forwards?0:1))%num],waypoints[abs(i+(forwards?1:0))%num]);
		v.normalize();
		addfan(POGEL::PHYSICS::FAN(waypoints[i], v, waypoints[abs(i+(forwards?0:1))%num].distance(waypoints[abs(i+(forwards?1:0))%num])));
	}
};

unsigned long POGEL::PHYSICS::GRAVITYCLUSTER::addsingularity(POGEL::PHYSICS::SINGULARITY sig) {
	singularities.add(sig);
	return numsingularities++;
};

void POGEL::PHYSICS::GRAVITYCLUSTER::addsingularities(POGEL::PHYSICS::SINGULARITY* sig, unsigned long num) {
	if(sig == (POGEL::PHYSICS::SINGULARITY*)NULL)
		POGEL::fatality(POGEL_FATALITY_NULL_ARRAY_POINTER_RETNUM,"%s to Singularity(s).",POGEL_FATALITY_NULL_ARRAY_POINTER_STRING);
	for(unsigned long i=0;i<num;i++) addsingularity(sig[i]);
};

