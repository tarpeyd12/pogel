#ifndef _OCTREE_TEMPLATE_H
#define _OCTREE_TEMPLATE_H

/*namespace POGEL {
//template < class T>
	class OCTREE<>;
}*/

#include "hashlist_template.h"
#include "../classes/bounding_class.h"
#include "../classes/physics/solid_class.h"
#include "../classes/physics/singularity_class.h"

#define		maxchildren		8
#define		dimentions		3
#define		defleastobjs	1

#define 	maxlevels		50

#define TUR 0 // towards up right    +x +y +z
#define TBR 1 // towards botom right +x -y +z
#define TUL 2 // towards up left     -x +y +z
#define TBL 3 // towards botom left  -x -y +z
#define AUR 4 // away up right       +x +y -z
#define ABR 5 // away botom right    +x -y -z
#define AUL 6 // away up left        -x +y -z
#define ABL 7 // away botom left     -x -y -z

#define pxpypz TUR // +x +y +z
#define pxpynz AUR // +x +y -z
#define pxnypz TBR // +x -y +z
#define pxnynz ABR // +x -y -z
#define nxpypz TUL // -x +y +z
#define nxpynz AUL // -x +y -z
#define nxnypz TBL // -x -y +z
#define nxnynz ABL // -x -y -z

#define bin_pxpypz (7) // 111 +x +y +z
#define bin_pxpynz (6) // 110 +x +y -z
#define bin_pxnypz (5) // 101 +x -y +z
#define bin_pxnynz (4) // 100 +x -y -z
#define bin_nxpypz (3) // 011 -x +y +z
#define bin_nxpynz (2) // 010 -x +y -z
#define bin_nxnypz (1) // 001 -x -y +z
#define bin_nxnynz (0) // 000 -x -y -z

inline unsigned char bn(bool a, bool b, bool c) { return ((a?4:0)|(b?2:0)|(c?1:0)); }

inline unsigned int bin_child_num(unsigned char c) {
	switch(c) {
		case bin_pxpypz: return pxpypz; // 111
		case bin_pxpynz: return pxpynz; // 110
		case bin_pxnypz: return pxnypz; // 101
		case bin_pxnynz: return pxnynz; // 100
		case bin_nxpypz: return nxpypz; // 011
		case bin_nxpynz: return nxpynz; // 010
		case bin_nxnypz: return nxnypz; // 001
		case bin_nxnynz: return nxnynz; // 000
	}
	return pxpypz;
}

inline unsigned int bool_child_num(bool a, bool b, bool c) {
	return bin_child_num(bn(a,b,c));
}

/* NOTE:
		the generic class T for the template must be like this:
		
		class T {
			public:
				POGEL::POINT getposition();
				POGEL::BOUNDING getbounding();
		}
*/

namespace POGEL {
template < class T >
class OCTREE {
	private:
		HASHLIST<T*>* allobjects;
		HASHLIST<T*> objects;
		HASHLIST<unsigned int> indicies;
		
		HASHLIST<OCTREE<T>*> children;
		POGEL::OCTREE<T>* parent;
		
		POGEL::POINT position;
		POGEL::BOUNDING bounding;
		
		unsigned int level;
		unsigned int leastobjs;
		
		bool addboundings;
	protected:
		
		#include "octree_template_protected.h"
		
	public:
		
		float getDencity() {
			if(getbounding().min != getbounding().max) {
				POGEL::VECTOR v(getbounding().max, getbounding().min);
				return float(objects.length())/((v.x?v.x:1)*(v.y?v.y:1)*(v.z?v.z:1));
			}
			return 0.0;
		}
		
		OCTREE() {
			leastobjs = defleastobjs;
			parent = NULL;
			allobjects = NULL;
			level = 0;
			addboundings = true;
		}
		
		OCTREE(OCTREE<T>* p, unsigned int lvl) {
			leastobjs = p->leastobjs;
			allobjects = p->allobjects;
			parent = p;
			level = lvl;
			addboundings = true;
		}
		
		OCTREE(OCTREE<T>* p) {
			leastobjs = p->leastobjs;
			allobjects = p->allobjects;
			addboundings = p->addboundings;
			parent = p;
			level = p->level+1;
		}
		
		OCTREE(HASHLIST<T*>* l, unsigned int lobj) {
			addboundings = true;
			leastobjs = (lobj > 1 ? lobj : 1);
			allobjects = l;
			parent = NULL;
			level = 0;
			objects.add(l);
			if(objects.length()) position = objects[0]->getposition();
			for(unsigned int i = 0; i < objects.length(); i++) {
				if(addboundings) {
					bounding.addpoint(objects[i]->getbounding().max);
					bounding.addpoint(objects[i]->getbounding().min);
				}
				else
					bounding.addpoint(getposition(),objects[i]->getposition());
				indicies += i;
			}
			position = (getbounding().max+getbounding().min)/2;
		}
		
		OCTREE(HASHLIST<T*>* l, unsigned int lobj, bool addbounds) {
			addboundings = addbounds;
			leastobjs = (lobj > 1 ? lobj : 1);
			allobjects = l;
			parent = NULL;
			level = 0;
			objects.add(l);
			if(objects.length()) position = objects[0]->getposition();
			for(unsigned int i = 0; i < objects.length(); i++) {
				if(addboundings) {
					bounding.addpoint(objects[i]->getbounding().max);
					bounding.addpoint(objects[i]->getbounding().min);
				}
				else
					bounding.addpoint(getposition(),objects[i]->getposition());
				indicies += i;
			}
			position = (getbounding().max+getbounding().min)/2;
		}
		
		OCTREE(HASHLIST<T*>* l) {
			addboundings = true;
			leastobjs = defleastobjs;
			allobjects = l;
			parent = NULL;
			level = 0;
			objects.add(l);
			if(objects.length()) position = objects[0]->getposition();
			for(unsigned int i = 0; i < objects.length(); i++) {
				if(addboundings) {
					bounding.addpoint(objects[i]->getbounding().max);
					bounding.addpoint(objects[i]->getbounding().min);
				}
				else
					bounding.addpoint(getposition(),objects[i]->getposition());
				indicies += i;
			}
			position = (getbounding().max+getbounding().min)/2;
		}
		
		void grow() {
			if(!(objects.length() > leastobjs && level+1 <= maxlevels && children.length() < maxchildren))
				return;
			while(children.length() < maxchildren && level+1 <= maxlevels)
				children += new POGEL::OCTREE<T>(this);
			for(unsigned int i = 0; i < children.length(); i++) {
				children[i]->setpos((getposition()+corner(i))/2);
				children[i]->bounding.addpoint(children[i]->getposition(),getposition());
				children[i]->bounding.addpoint(children[i]->getposition(),corner(i));
			}
			for(unsigned int i = 0; i < objects.length(); i++)
				relevent_child(getposition(), objects[i]->getposition())->place(objects[i], indicies[i]);
		}
		
		#include "octree_template_public.h"
};
}

namespace POGEL {
#define T POGEL::PHYSICS::SOLID
template < >
class OCTREE < POGEL::PHYSICS::SOLID > {
	private:
		HASHLIST<T*>* allobjects;
		HASHLIST<T*> objects;
		HASHLIST<unsigned int> indicies;
		
		HASHLIST<OCTREE<T>*> children;
		POGEL::OCTREE<T>* parent;
		
		POGEL::POINT position;
		POGEL::BOUNDING bounding;
		
		unsigned int level;
		unsigned int leastobjs;
		
		bool addboundings;
		
		float mass;
		bool masscenter_good, mass_good;
		POGEL::POINT masscenter;
		float theta;
	protected:
		
		#include "octree_template_protected.h"
		
		float avgside() {
			POGEL::VECTOR v(getbounding().max, getbounding().min);
			if(fabs(v.x) >= fabs(v.y) && fabs(v.x) >= fabs(v.z))
				return v.x;
			else if(fabs(v.y) >= fabs(v.x) && fabs(v.y) >= fabs(v.z))
				return v.y;
			else
				return v.z;
		}
		
	public:
		
		OCTREE() {
			leastobjs = defleastobjs;
			parent = NULL;
			allobjects = NULL;
			level = 0;
			addboundings = true;
			theta = .5;
			masscenter = position;
			mass_good = masscenter_good = false;
		}
		
		OCTREE(OCTREE<T>* p, unsigned int lvl) {
			leastobjs = p->leastobjs;
			allobjects = p->allobjects;
			parent = p;
			level = lvl;
			addboundings = true;
			theta = p->theta;
			masscenter = position;
			mass_good = masscenter_good = false;
		}
		
		OCTREE(OCTREE<T>* p) {
			leastobjs = p->leastobjs;
			allobjects = p->allobjects;
			addboundings = p->addboundings;
			parent = p;
			level = p->level+1;
			theta = p->theta;
			masscenter = position;
			mass_good = masscenter_good = false;
		}
		
		OCTREE(HASHLIST<T*>* l, unsigned int lobj) {
			addboundings = true;
			leastobjs = (lobj > 1 ? lobj : 1);
			allobjects = l;
			parent = NULL;
			level = 0;
			objects.add(l);
			if(objects.length()) position = objects[0]->getposition();
			for(unsigned int i = 0; i < objects.length(); i++) {
				if(addboundings) {
					bounding.addpoint(objects[i]->getbounding().max);
					bounding.addpoint(objects[i]->getbounding().min);
				}
				else
					bounding.addpoint(getposition(),objects[i]->getposition());
				indicies += i;
			}
			position = (getbounding().max+getbounding().min)/2;
			theta = .5;
			masscenter = position;
			mass_good = masscenter_good = false;
		}
		
		OCTREE(HASHLIST<T*>* l, unsigned int lobj, bool addbounds) {
			addboundings = addbounds;
			leastobjs = (lobj > 1 ? lobj : 1);
			allobjects = l;
			parent = NULL;
			level = 0;
			objects.add(l);
			if(objects.length()) position = objects[0]->getposition();
			for(unsigned int i = 0; i < objects.length(); i++) {
				if(addboundings) {
					bounding.addpoint(objects[i]->getbounding().max);
					bounding.addpoint(objects[i]->getbounding().min);
				}
				else
					bounding.addpoint(getposition(),objects[i]->getposition());
				indicies += i;
			}
			position = (getbounding().max+getbounding().min)/2;
			theta = .5;
			masscenter = position;
			mass_good = masscenter_good = false;
		}
		
		OCTREE(HASHLIST<T*>* l) {
			addboundings = true;
			leastobjs = defleastobjs;
			allobjects = l;
			parent = NULL;
			level = 0;
			objects.add(l);
			if(objects.length()) position = objects[0]->getposition();
			for(unsigned int i = 0; i < objects.length(); i++) {
				if(addboundings) {
					bounding.addpoint(objects[i]->getbounding().max);
					bounding.addpoint(objects[i]->getbounding().min);
				}
				else
					bounding.addpoint(getposition(),objects[i]->getposition());
				indicies += i;
			}
			position = (getbounding().max+getbounding().min)/2;
			theta = .5;
			masscenter = position;
			mass_good = masscenter_good = false;
		}
		
		float getMass() {
			if(mass_good)
				return mass;
			float ret = 0.0;
			if(numchildren())
				for(unsigned int i = 0; i < numchildren(); i++)
					ret += child(i)->getMass();
			else
				for(unsigned int i = 0; i < numobjs(); i++)
					ret += object(i)->behavior.mass;
			mass = ret;
			mass_good = true;
			return ret;
		}
		
		float getDencity() {
			if(getbounding().min != getbounding().max) {
				POGEL::VECTOR v(getbounding().max, getbounding().min);
				return getMass()/((v.x?v.x:1)*(v.y?v.y:1)*(v.z?v.z:1));
			}
			return 0.0;
		}
		
		void boolgrvs(bool* b) {
			for(unsigned int i = 0; i < objects.length(); i++)
				b[indicies[i]] = true;
		}
		
		void uintgrvs(unsigned int* b) {
			for(unsigned int i = 0; i < objects.length(); i++)
				b[indicies[i]] = level;
		}
		
		#include "octree_template_public.h"
		
		POGEL::POINT getmasscenter() {
			if(masscenter_good)
				return masscenter;
			if(numobjs()) {
				masscenter = object(0)->getposition()/(float)numobjs();
				for(unsigned int i = 1; i < numobjs(); i++)
					masscenter += object(i)->getposition()*(object(i)->behavior.mass/getMass());
			}
			for(unsigned int i = 0; i < numchildren(); i++)
				if(!child(i)->masscenter_good)
					child(i)->getmasscenter();
			masscenter_good = true;
			return masscenter;
		}
		
		void grow() {
			if(!(objects.length() > leastobjs && level+1 <= maxlevels && children.length() < maxchildren))
				return;
			while(children.length() < maxchildren && level+1 <= maxlevels)
				children += new POGEL::OCTREE<T>(this);
			for(unsigned int i = 0; i < children.length(); i++) {
				children[i]->setpos((getposition()+corner(i))/2);
				children[i]->bounding.addpoint(children[i]->getposition(),getposition());
				children[i]->bounding.addpoint(children[i]->getposition(),corner(i));
			}
			masscenter = POGEL::POINT(); mass = 0;
			mass_good = masscenter_good = false;
			for(unsigned int i = 0; i < objects.length(); i++) {
				if(!masscenter_good)
					masscenter += object(i)->getposition()/(float)numobjs();
				if(!mass_good)
					mass += object(i)->behavior.mass;
				relevent_child(getposition(), objects[i]->getposition())->place(objects[i], indicies[i]);
			}
			mass_good = masscenter_good = true;
		}
		
		bool objhaspos(POGEL::POINT p) {
			for(unsigned int i = 0; i < objects.length(); i++)
				if(object(i)->position == p)
					return true;
			return false;
		}
		
		virtual POGEL::VECTOR getpull(POGEL::POINT p, float m) {
			if((numobjs() <= leastobjs && !objhaspos(p)) || bounding.min.distance(bounding.max)/getmasscenter().distance(p) < theta)
				return POGEL::PHYSICS::SINGULARITY(getmasscenter(), getMass()).getpull(p, m);
			POGEL::VECTOR ret;
			for(unsigned int i = 0; i < numchildren(); i++)
				if(child(i)->numobjs())
					ret += child(i)->getpull(p, m);
			return ret;
		}
		
		POGEL::VECTOR drawgetpull(POGEL::POINT p, float m) {
			if((numobjs() <= leastobjs && !objhaspos(p)) || bounding.min.distance(bounding.max)/getmasscenter().distance(p) < theta) {
				bounding.color = POGEL::COLOR(.25,.25,.25,getMass()/progen()->getMass());
				bounding.draw(POGEL::POINT());
				//getmasscenter().draw(3,bounding.color);
				return POGEL::PHYSICS::SINGULARITY(getmasscenter(), getMass()).getpull(p, m);
			}
			POGEL::VECTOR ret;
			for(unsigned int i = 0; i < numchildren(); i++)
				if(child(i)->numobjs())
					ret += child(i)->drawgetpull(p, m);
			return ret;
		}
		
		// http://arborjs.org/docs/barnes-hut
		
};
#ifdef T
#undef T
#endif
}

#endif /* _OCTREE_TEMPLATE_H */
