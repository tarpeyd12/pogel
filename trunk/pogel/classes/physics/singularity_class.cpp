#include <string.h>

#include "singularity_class.h"

unsigned long POGEL::PHYSICS::FLOW::addfan(POGEL::PHYSICS::FAN fan) {
	POGEL::PHYSICS::FAN *tmp = new POGEL::PHYSICS::FAN[numgusts+1];
	if(numgusts > 0)
		memcpy(tmp, gusts, sizeof(POGEL::PHYSICS::FAN)*numgusts);
	tmp[numgusts]=fan;
	//printf("reallocating fans from %p to: %p\n",gusts,tmp);
	if(gusts)
		delete[] gusts;
	gusts = NULL;
	gusts=tmp;
	numgusts++;
	return numgusts-1;
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
	POGEL::PHYSICS::SINGULARITY *tmp = new POGEL::PHYSICS::SINGULARITY[numsingularities+1];
	if(numsingularities > 0) memcpy(tmp, singularities, sizeof(POGEL::PHYSICS::SINGULARITY)*numsingularities);
	tmp[numsingularities]=sig;
	//printf("reallocating singularities from %p to: %p\n",singularities,tmp);
	if(singularities)
		delete[] singularities;
	singularities = NULL;
	singularities=tmp;
	numsingularities++;
	return numsingularities-1;
};

void POGEL::PHYSICS::GRAVITYCLUSTER::addsingularities(POGEL::PHYSICS::SINGULARITY* sig, unsigned long num) {
	if(sig == (POGEL::PHYSICS::SINGULARITY*)NULL)
		POGEL::fatality(POGEL_FATALITY_NULL_ARRAY_POINTER_RETNUM,"%s to Singularity(s).",POGEL_FATALITY_NULL_ARRAY_POINTER_STRING);
	for(unsigned long i=0;i<num;i++) addsingularity(sig[i]);
};

