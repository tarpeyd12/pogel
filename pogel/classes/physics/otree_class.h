#ifndef _OTREE_CLASS_H
#define _OTREE_CLASS_H

namespace POGEL {
class otree;
}

#include "../../templates/templates.h"
#include "physics.h"
#include "../bounding_class.h"
#include "solid_class.h"

#define addboundings 1

#define		maxchildren		8
#define		dimentions		3
#define		defleastobjs	1

#define 	maxlevels		100

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

class POGEL::otree {
	private:
		HASHLIST<POGEL::PHYSICS::SOLID*>* allobjects;
		HASHLIST<POGEL::PHYSICS::SOLID*> objects;
		HASHLIST<unsigned int> indicies;
		
		HASHLIST<POGEL::otree*> children;
		POGEL::otree* parent;
		
		POGEL::POINT position;
		POGEL::BOUNDING bounding;
		
		unsigned int level;
		unsigned int leastobjs;
	protected:
		
		void setpos(POGEL::POINT p) {
			position = p;
		}
		
		POGEL::otree* bool_child(bool a, bool b, bool c) {
			if(children.length())
				return children[bool_child_num(a,b,c)];
			return NULL;
		}
		
		POGEL::otree* bin_child(unsigned char c) {
			if(children.length())
				return children[bin_child_num(c)];
			return NULL;
		}
		
		POGEL::otree* relevent_child(POGEL::POINT reff, POGEL::POINT pos) {
			return bool_child(pos.x >= reff.x, pos.y >= reff.y, pos.z >= reff.z);
		}
		
		POGEL::POINT corner(unsigned int c) {
			switch(c) {
				case TUR: return POGEL::POINT(bounding.max.x,bounding.max.y,bounding.max.z);
				case TBR: return POGEL::POINT(bounding.max.x,bounding.min.y,bounding.max.z);
				case TUL: return POGEL::POINT(bounding.min.x,bounding.max.y,bounding.max.z);
				case TBL: return POGEL::POINT(bounding.min.x,bounding.min.y,bounding.max.z);
				case AUR: return POGEL::POINT(bounding.max.x,bounding.max.y,bounding.min.z);
				case ABR: return POGEL::POINT(bounding.max.x,bounding.min.y,bounding.min.z);
				case AUL: return POGEL::POINT(bounding.min.x,bounding.max.y,bounding.min.z);
				case ABL: return POGEL::POINT(bounding.min.x,bounding.min.y,bounding.min.z);
			}
			return POGEL::POINT();
		}
		
		POGEL::POINT bin_corner(unsigned char c) {
			return corner(bin_child_num(c));
		}
		
		POGEL::otree* progen() {
			if(parent == NULL)
				return this;
			return parent;
		}
		
	public:
		
		otree() {
			leastobjs = defleastobjs;
			parent = NULL;
			level = 0;
		}
		
		otree(POGEL::otree* p, unsigned int lvl) {
			leastobjs = p->leastobjs;
			allobjects = p->allobjects;
			parent = p;
			level = lvl;
		}
		
		otree(POGEL::otree* p) {
			leastobjs = p->leastobjs;
			allobjects = p->allobjects;
			parent = p;
			level = p->level+1;
		}
		
		otree(HASHLIST<POGEL::PHYSICS::SOLID*>* l) {
			leastobjs = defleastobjs;
			allobjects = l;
			parent = NULL;
			level = 0;
			position = POGEL::POINT();
			objects.add(l);
			for(unsigned int i = 0; i < objects.length(); i++) {
				if(!i) position = objects[i]->position;
				bounding.addpoint(position,objects[i]->position);
				#ifdef addboundings
				bounding.addpoint(objects[i]->bounding.max);
				bounding.addpoint(objects[i]->bounding.min);
				#endif
				indicies += i;
			}
			position = (bounding.max+bounding.min)/2;
		}
		
		otree(HASHLIST<POGEL::PHYSICS::SOLID*>* l, unsigned int lobj) {
			leastobjs = (lobj > 1 ? lobj : 1);
			allobjects = l;
			parent = NULL;
			level = 0;
			position = POGEL::POINT();
			objects.add(l);
			for(unsigned int i = 0; i < objects.length(); i++) {
				if(!i) position = objects[i]->position;
				bounding.addpoint(position,objects[i]->position);
				#ifdef addboundings
				bounding.addpoint(objects[i]->bounding.max);
				bounding.addpoint(objects[i]->bounding.min);
				#endif
				indicies += i;
			}
			position = (bounding.max+bounding.min)/2;
		}
		
		~otree() {
			objects.clear();
			indicies.clear();
			if(allobjects) {
				if(allobjects->length())
					allobjects->clear();
				allobjects = NULL;
			}
			for(unsigned int i = 0; i < children.length(); i++)
				delete children[i];
			children.clear();
		}
		
		void FORCEresizelist(unsigned int s) {
			objects.FORCEresizeInternalList(s);
			indicies.FORCEresizeInternalList(s);
			children.FORCEresizeInternalList(s);
			for(unsigned int i = 0; i < children.length(); i++)
				children[i]->FORCEresizelist(s);
		}
		
		void FORCEfastlist() {
			FORCEresizelist(1);
		}
		
		unsigned int numobjs() {
			return objects.length();
		}
		
		void add(POGEL::PHYSICS::SOLID* s) {
			objects += s;
			bounding.addpoint(position,s->position);
			#ifdef addboundings
			bounding.addpoint(position,s->bounding.max);
			bounding.addpoint(position,s->bounding.min);
			#endif
		}
		
		void add(POGEL::PHYSICS::SOLID* s, unsigned int i) {
			objects += s;
			bounding.addpoint(position,s->position);
			indicies += i;
			#ifdef addboundings
			bounding.addpoint(position,s->bounding.max);
			bounding.addpoint(position,s->bounding.min);
			#endif
		}
		
		void grow() {
			if(objects.length() > leastobjs && level+1 <= maxlevels) {
				while(children.length() < maxchildren && level+1 <= maxlevels)
					children += new POGEL::otree(this);
				for(unsigned int i = 0; i < children.length(); i++) {
					children[i]->setpos((position+corner(i))/2);
					children[i]->bounding.addpoint(children[i]->position,position); 
					children[i]->bounding.addpoint(children[i]->position,corner(i));
				}
			}
			if(children.length() >= maxchildren && level+1 <= maxlevels)
				for(unsigned int i = 0; i < objects.length(); i++)
					relevent_child(position, objects[i]->position)->add(objects[i], indicies[i]);
			if(children.length() >= maxchildren && level+1 <= maxlevels)
				for(unsigned int i = 0; i < children.length(); i++)
					if(children[i]->numobjs())
						children[i]->grow();
		}
		
		HASHLIST<POGEL::PHYSICS::SOLID*>* relevent(POGEL::POINT p, POGEL::BOUNDING b) {
			HASHLIST<POGEL::PHYSICS::SOLID*> *ret = new HASHLIST<POGEL::PHYSICS::SOLID*>();
			if((objects.length() <= leastobjs && bounding.checkbounding(POGEL::POINT(), p, b)) || children.length() < maxchildren) {
				for(unsigned int i = 0; i < objects.length(); i++)
					if(objects[i]->bounding.checkbounding(POGEL::POINT(), p, b))
						ret->add(objects[i]);
				return ret;
			}
			if(b.surrounds(p,POGEL::POINT(),bounding)) {
				ret->add(&objects);
				return ret;
			}
			for(unsigned int i = 0; i < children.length(); i++)
				if(children[i]->bounding.checkbounding(POGEL::POINT(), p, b))
					ret->pillage(children[i]->relevent(p,b));
			return ret;
		}
		
		HASHLIST<POGEL::PHYSICS::SOLID*>* relevent(POGEL::BOUNDING b) {
			return relevent(POGEL::POINT(), b);
		}
		
		HASHLIST<unsigned int>* releventIndicies(POGEL::POINT p, POGEL::BOUNDING b) {
			HASHLIST<unsigned int>* ret = new HASHLIST<unsigned int>();
			if((objects.length() <= leastobjs && bounding.checkbounding(POGEL::POINT(), p, b)) || children.length() < maxchildren) {
				for(unsigned int i = 0; i < objects.length(); i++)
					if(objects[i]->bounding.checkbounding(POGEL::POINT(), p, b))
						ret->add(indicies[i]);
				return ret;
			}
			if(b.surrounds(p,POGEL::POINT(),bounding)) {
				ret->add(&indicies);
				return ret;
			}
			for(unsigned int i = 0; i < children.length(); i++)
				if(children[i]->bounding.checkbounding(POGEL::POINT(), p, b))
					ret->pillage(children[i]->releventIndicies(p,b));
			return ret;
		}
		
		HASHLIST<unsigned int>* releventIndicies(POGEL::BOUNDING b) {
			return releventIndicies(POGEL::POINT(),b);
		}
		
		void draw() {
			for(unsigned int i = 0; i < children.length(); i++)
				if(children[i]->numobjs())
					children[i]->draw();
			bounding.draw(POGEL::POINT());
		}
};

#endif /* _OTREE_CLASS_H */
