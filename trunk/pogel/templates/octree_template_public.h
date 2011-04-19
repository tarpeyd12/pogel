		
		
		~OCTREE() {
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
			addboundings = true;
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
		
		POGEL::POINT getposition() {
			return position;
		}
		
		POGEL::BOUNDING getbounding() {
			return bounding;
		}
		
		unsigned int numobjs() {
			return objects.length();
		}
		
		unsigned int getlevel() {
			return level;
		}
		
		void add(T* s) {
			objects += s;
			if(addboundings) {
				bounding.addpoint(getposition(),s->getbounding().max);
				bounding.addpoint(getposition(),s->getbounding().min);
			}
			else
				bounding.addpoint(getposition(),s->getposition());
		}
		
		void add(T* s, unsigned int i) {
			objects += s;
			indicies += i;
			if(addboundings) {
				bounding.addpoint(getposition(),s->getbounding().max);
				bounding.addpoint(getposition(),s->getbounding().min);
			}
			else
				bounding.addpoint(getposition(),s->getposition());
		}
		
		void place(T* s, unsigned int i) {
			objects += s;
			indicies += i;
			if(addboundings) {
				bounding.addpoint(getposition(),s->getbounding().max);
				bounding.addpoint(getposition(),s->getbounding().min);
			}
			else
				bounding.addpoint(getposition(),s->getposition());
			if(objects.length() > leastobjs && level+1 <= maxlevels) {
				if(children.length() >= maxchildren)
					relevent_child(getposition(), s->getposition())->place(s,i);
				else
					grow();
			}
		}
		
		/*void grow() {
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
		}*/
		
		void regrow() {
			if(children.length()) {
				for(unsigned int i = 0; i < children.length(); i++)
					delete children[i];
				children.clear();
			}
			grow();
		}
		
		HASHLIST<T*>* relevent(POGEL::POINT p, POGEL::BOUNDING b) {
			HASHLIST<T*> *ret = new HASHLIST<T*>();
			if((objects.length() <= leastobjs && getbounding().checkbounding(POGEL::POINT(), p, b)) || children.length() < maxchildren) {
				for(unsigned int i = 0; i < objects.length(); i++)
					if(objects[i]->getbounding().checkbounding(POGEL::POINT(), p, b))
						ret->add(objects[i]);
				return ret;
			}
			if(b.surrounds(p,POGEL::POINT(),getbounding())) {
				ret->add(&objects);
				return ret;
			}
			for(unsigned int i = 0; i < children.length(); i++)
				if(children[i]->getbounding().checkbounding(POGEL::POINT(), p, b))
					ret->pillage(children[i]->relevent(p,b));
			return ret;
		}
		
		HASHLIST<T*>* relevent(POGEL::BOUNDING b) {
			return relevent(POGEL::POINT(), b);
		}
		
		HASHLIST<unsigned int>* releventIndicies(POGEL::POINT p, POGEL::BOUNDING b) {
			HASHLIST<unsigned int>* ret = new HASHLIST<unsigned int>();
			if((objects.length() <= leastobjs && getbounding().checkbounding(POGEL::POINT(), p, b)) || children.length() < maxchildren) {
				for(unsigned int i = 0; i < objects.length(); i++)
					if(objects[i]->getbounding().checkbounding(POGEL::POINT(), p, b))
						ret->add(indicies[i]);
				return ret;
			}
			if(b.surrounds(p,POGEL::POINT(),getbounding())) {
				ret->add(&indicies);
				return ret;
			}
			for(unsigned int i = 0; i < children.length(); i++)
				if(children[i]->getbounding().checkbounding(POGEL::POINT(), p, b))
					ret->pillage(children[i]->releventIndicies(p,b));
			return ret;
		}
		
		HASHLIST<unsigned int>* releventIndicies(POGEL::BOUNDING b) {
			return releventIndicies(POGEL::POINT(),b);
		}
		
		HASHLIST<OCTREE<T>*>* dencityReleventChildren(float dencity) {
			HASHLIST<OCTREE<T>*>* ret = new HASHLIST<POGEL::OCTREE<T>*>();
			if(getDencity() >= dencity) {
				int chlddncecnt = 0, chlddncecnt_wo = 0;
				for(unsigned int i = 0; i < children.length(); i++) {
					if(children[i]->numobjs())
						chlddncecnt_wo++;
					if(children[i]->getDencity() >= getDencity()*1.5)
						chlddncecnt++;
				}
				if(!chlddncecnt || chlddncecnt >= maxchildren || chlddncecnt >= chlddncecnt_wo)
					ret->add(this);
				else
					for(unsigned int i = 0; i < children.length(); i++)
						if(children[i]->getDencity() >= getDencity()*1.5)
							ret->pillage(children[i]->dencityReleventChildren(dencity));
			}
			else
				for(unsigned int i = 0; i < children.length(); i++)
					ret->pillage(children[i]->dencityReleventChildren(dencity));
			return ret;
		}
		
		unsigned int maxLevel() {
			unsigned int max = level;
			if(children.length())
				for(unsigned int i = 0; i < children.length(); i++) {
					unsigned int chml = children[i]->maxLevel();
					if(chml > max)
						max = chml;
				}
			return max;
		}
		
		HASHLIST<OCTREE<T>*>* levelReleventChildren(unsigned int lvl_s, unsigned int lvl_e) {
			HASHLIST<OCTREE<T>*>* ret = new HASHLIST<POGEL::OCTREE<T>*>();
			if(level > lvl_e)
				return ret;
			if(children.length() > 0 && level < lvl_e)
				for(unsigned int i = 0; i < children.length(); i++)
					ret->pillage(children[i]->levelReleventChildren(lvl_s,lvl_e));
			if(level == lvl_e || (level >= lvl_s && !children.length() && objects.length())) {
				ret->add(this);
				return ret;
			}
			return ret;
		}
		
		unsigned int numchildren() {
			return children.length();
		}
		
		T* object(unsigned int o) {
			return objects[o];
		}
		
		OCTREE<T>* child(unsigned int c) {
			return children[c];
		}
		
		void draw() {
			bounding.color = POGEL::COLOR(.05,.125,1,(float)getlevel()/(float)maxlevels);
			if(children.length()) {
				for(unsigned int i = 0; i < children.length(); i++)
					if(children[i]->numobjs())
						children[i]->draw();
			}
			//else
				getbounding().draw(POGEL::POINT());
		}
