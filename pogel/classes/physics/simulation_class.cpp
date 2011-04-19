#include <string.h>

#include "simulation_class.h"
#include "../threads.h"

/* Just remember, in here, there is usualy no method to the madenss. */

POGEL::PHYSICS::SIMULATION::SIMULATION() : POGEL::PHYSICS::DYNAMICS() {
	precision=0.01f;
	deactivation = false;
	inactive_index = 25;
	stepstaken = 0;
	collitters = 1;
	#ifdef THREADSOK
	threads = 1;
	#endif
};

void POGEL::PHYSICS::SIMULATION::addpulls(unsigned long s, unsigned long e) {
	float r = (POGEL::hasproperty(POGEL_TIMEBASIS) ? POGEL::GetSecondsPerFrame() : 1);
	for(unsigned long a = s; a < e && a < numobjs(); a++) {
		if(objects[a]->hasOption(PHYSICS_SOLID_VOLITAL) && !objects[a]->hasOption(PHYSICS_SOLID_STATIONARY)) {
			
			if(
				(
					deactivation
					&&
					(
						(
							(objects[a]->direction+objects[a]->force).getdistance() <= precision*r
							&& 
							objects[a]->sameposlegacy(precision*r, inactive_index)
						)
						&& 
						objects[a]->stepstaken > objects[a]->trailsize
						&& 
						stepstaken > 100
						&& 
						objects[a]->stepstaken > inactive_index
					)
					//||
					//objects[a]->napping()
				)
			) {
				objects[a]->direction = POGEL::VECTOR();
				if(!objects[a]->napping()) { objects[a]->forcegetbounding(); objects[a]->sleep(); }
			}
			else if(!deactivation) {
				objects[a]->direction += getpull(objects[a]);
				if(objects[a]->napping()) objects[a]->wake();
			}
			else if(objects[a]->napping()) { }
			else {
				objects[a]->direction += getpull(objects[a]);
				if(objects[a]->napping()) objects[a]->wake();
			}
			
			// TODO: get the air friction to work with timing
			float airslowdown = ( ( objects[a]->behavior.air_friction * air_dencity ) / PARTICLE_SLOWDOWN ) + 1.0f;
			objects[a]->spin /= airslowdown;
			objects[a]->direction /= airslowdown;
		}
		//objects[a]->increment();
		objects[a]->clearForce();
	}
};

#ifdef THREADSOK
class thdat {
	public:
		POGEL::PHYSICS::SIMULATION* sim;
		unsigned int itt, nth;
		thdat(POGEL::PHYSICS::SIMULATION* s, unsigned int i, unsigned int n) { sim = s; itt = i; nth = n; }
		~thdat() { }
};

THREADTYPE pullrun(THREADARGS data) {
	thdat *dat = (thdat*)data;
	//printf("calculating: %u to %u\n", dat->itt*dat->nth, (dat->itt+1)*dat->nth);
	dat->sim->addpulls(dat->itt*dat->nth, (dat->itt+1)*dat->nth + (dat->itt==dat->sim->numThreads()-1?dat->sim->numobjs():0));
	delete dat;
	return NULL;
};
#endif

void POGEL::PHYSICS::SIMULATION::addpulls() {
	#ifdef THREADSOK
	if(threads > 1) {
		if(threads > numobjs()) threads = numobjs();
		POGEL::THREAD* tharr = new POGEL::THREAD[threads];
		unsigned int nth = numobjs()/threads;
		for(unsigned int i = 0; i < threads; i++) {
			tharr[i].setThread(pullrun);
			tharr[i].setData(new thdat(this, i, nth));
			tharr[i].startThread();
		}
		for(unsigned int i = 0; i < threads; i++) {
			tharr[i].joinThread();
		}
		delete[] tharr;
	}
	else
	#endif
		addpulls(0, numobjects);
};

inline void objectIntersectionProcessing(POGEL::PHYSICS::SIMULATION* sim, unsigned long a, unsigned long b) {
	if( a!=b && boundingcheck(sim->objs(a), sim->objs(b)) ) {
		if( sim->processcollision(sim->objs(a), sim->objs(b))) {
			if(sim->objs(a)->callback != NULL) {
				char* n = new char[strlen(sim->objs(b)->getname())+1];
				memset(n, '\0', strlen(sim->objs(b)->getname())+1);
				strcpy(n, sim->objs(b)->getname());
				sim->objs(a)->callback(sim->objs(a), n);
				delete[] n;
			}
			if(sim->objs(b)->callback != NULL) {
				char* n = new char[strlen(sim->objs(a)->getname())+1];
				memset(n, '\0', strlen(sim->objs(a)->getname())+1);
				strcpy(n, sim->objs(a)->getname());
				sim->objs(b)->callback(sim->objs(b), n);
				delete[] n;
			}
			if(sim->objs(a)->napping()) sim->objs(a)->wake();
			if(sim->objs(b)->napping()) sim->objs(b)->wake();
		}
		
		if(POGEL::hasproperty(POGEL_PAIRS) && b > a) {
			if(sim->objs(a)->hasOption(PHYSICS_SOLID_CONCAVE) && sim->objs(a)->hasOption(PHYSICS_SOLID_SPHERE)) {
				POGEL::VECTOR vr(sim->objs(a)->position, sim->objs(a)->position);
				vr.normalize();
				vr *= sim->objs(a)->bounding.maxdistance;
				vr += sim->objs(a)->position;
				POGEL::LINE(vr.topoint(),sim->objs(b)->position,1,POGEL::COLOR(1,.75,.75,1)).draw();
			}
			else if(sim->objs(b)->hasOption(PHYSICS_SOLID_CONCAVE) && sim->objs(b)->hasOption(PHYSICS_SOLID_SPHERE)) {
				POGEL::VECTOR vr(sim->objs(b)->position, sim->objs(a)->position);
				vr.normalize();
				vr *= sim->objs(b)->bounding.maxdistance;
				vr += sim->objs(b)->position;
				POGEL::LINE(sim->objs(a)->position,vr.topoint(),1,POGEL::COLOR(1,.75,.75,1)).draw();
			}
			else if(sim->objs(a)->hasOption(PHYSICS_SOLID_CONCAVE)) {
				
			}
			else if(sim->objs(b)->hasOption(PHYSICS_SOLID_CONCAVE)) {
				
			}
			else {
				POGEL::LINE(sim->objs(a)->position,sim->objs(b)->position,1,POGEL::COLOR(1,.75,.75,1)).draw();
			}
		}
	}
};

inline void checkcollision(POGEL::PHYSICS::SIMULATION* sim, unsigned long s, unsigned long e) {
	for( unsigned long a = s; a < e && a < sim->numobjs(); a++ ) {
		if(!sim->objs(a)->napping() || sim->objs(a)->hasOption(PHYSICS_SOLID_STATIONARY)) {
			HASHLIST<unsigned int> *objs = sim->getotree()->releventIndicies(sim->objs(a)->bounding);
			for( unsigned long b = 0; b < objs->length(); b++ )
				if(objs->get(b) < sim->numobjs())
					objectIntersectionProcessing(sim, a, objs->get(b));
			delete objs;
		}
	}
};

inline void checkcollision(POGEL::PHYSICS::SIMULATION* sim) { checkcollision(sim, 0, sim->numobjs()); };

#ifdef THREADSOK
THREADTYPE collrun(THREADARGS data) {
	thdat *dat = (thdat*)data;
	//printf("calculating: %u to %u\n", dat->itt*dat->nth, (dat->itt+1)*dat->nth);
	checkcollision(dat->sim, dat->itt*dat->nth, (dat->itt+1)*dat->nth + (dat->itt==dat->sim->numThreads()-1?dat->sim->numobjs():0));
	delete dat;
	return NULL;
};
#endif

void POGEL::PHYSICS::SIMULATION::checkcollisions() {
	#ifdef THREADSOK
	if(threads > 1) {
		if(threads > numobjs()) threads = numobjs();
		POGEL::THREAD* tharr = new POGEL::THREAD[threads];
		unsigned int nth = numobjs()/threads;
		for(unsigned int i = 0; i < threads; i++) {
			tharr[i].setThread(collrun);
			tharr[i].setData(new thdat(this, i, nth));
			tharr[i].startThread();
		}
		for(unsigned int i = 0; i < threads; i++)
			tharr[i].joinThread();
		delete[] tharr;
	}
	else
	#endif
		checkcollision(this);
};

void POGEL::PHYSICS::SIMULATION::stepobjs() {
	unsigned long l = objects.length();
	for(unsigned long a=0;a<l;a++) {
		if(!objects[a]->napping())
			objects[a]->step();
		else if(objects[a]->function)
			objects[a]->function(objects[a]);
		if(objects.length() < l) { l--; a--; }
	}
};

void POGEL::PHYSICS::SIMULATION::collincrement() {
	for(int g = 0; g < collitters; g++) checkcollisions();
};

void POGEL::PHYSICS::SIMULATION::increment() {
	buildot();
	
	addpulls();
	collincrement();
	stepobjs();
	stepstaken++;
	
	destroyot();
};

