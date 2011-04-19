		void setpos(POGEL::POINT p) {
			position = p;
		}
		
		OCTREE<T>* bool_child(bool a, bool b, bool c) {
			if(children.length())
				return children[bool_child_num(a,b,c)];
			return NULL;
		}
		
		OCTREE<T>* bin_child(unsigned char c) {
			if(children.length())
				return children[bin_child_num(c)];
			return NULL;
		}
		
		OCTREE<T>* relevent_child(POGEL::POINT reff, POGEL::POINT pos) {
			return bool_child(pos.x >= reff.x, pos.y >= reff.y, pos.z >= reff.z);
		}
		
		POGEL::POINT corner(unsigned int c) {
			switch(c) {
				case TUR: return POGEL::POINT(getbounding().max.x,getbounding().max.y,getbounding().max.z);
				case TBR: return POGEL::POINT(getbounding().max.x,getbounding().min.y,getbounding().max.z);
				case TUL: return POGEL::POINT(getbounding().min.x,getbounding().max.y,getbounding().max.z);
				case TBL: return POGEL::POINT(getbounding().min.x,getbounding().min.y,getbounding().max.z);
				case AUR: return POGEL::POINT(getbounding().max.x,getbounding().max.y,getbounding().min.z);
				case ABR: return POGEL::POINT(getbounding().max.x,getbounding().min.y,getbounding().min.z);
				case AUL: return POGEL::POINT(getbounding().min.x,getbounding().max.y,getbounding().min.z);
				case ABL: return POGEL::POINT(getbounding().min.x,getbounding().min.y,getbounding().min.z);
			}
			return POGEL::POINT();
		}
		
		POGEL::POINT bin_corner(unsigned char c) {
			return corner(bin_child_num(c));
		}
		
		POGEL::OCTREE<T>* progen() {
			if(parent == NULL)
				return this;
			return parent;
		}
