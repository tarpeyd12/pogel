#ifndef _HASHLIST_TEMPLATE_H
#define _HASHLIST_TEMPLATE_H

#include "classlist_template.h"
#include "arraylist.h"

template<typename T>
class HASHLIST {
	private:
		CLASSLIST<arraylist<T>*> list;
		/*  if insize is one, then this is just as effient as using CLASSLIST<T>.
		 *  if insize is greater than the maximum desired length of the list, then 
		 *    it is just as effient as using arraylist<T>.
		 */
		unsigned int insize;
		unsigned int pos;
	public:
		HASHLIST() { pos = 0; insize = 256; }
		HASHLIST(unsigned int s) { pos = 0; insize = s; }
		~HASHLIST() { 
			for(unsigned int i=0;i<list.length();i++) { delete list[i]; list -= i; }
			list.clear(); pos = 0; insize = 256;
		}
		
		unsigned int length() { return pos; }
		
		void clear() {
			for(unsigned int i = 0; i < list.length(); i++) {
				list[i]->empty(); delete list[i];
			}
			list.clear();
		}
		
		void add(T l) { if(pos%insize==0)list+=new arraylist<T>(); list[pos/insize]->add(l); pos++; }
		
		void add(HASHLIST<T> l) { for(unsigned int i = 0; i < l.length(); i++) add(l[i]); }
		void add(HASHLIST<T> *l) {
			unsigned int g = 0;
			while(pos%insize && g < l->length())
				add(l->get(g++));
			if(g >= l->length())
				return;
			g-=1;
			unsigned int lstln = l->length()-(g+1);
			unsigned int numtoplsts = lstln/insize + 1;
			unsigned int toplststrtln = list.length();
			for(unsigned int i = 0; i < numtoplsts; i++)
				list+=new arraylist<T>();
			for(unsigned int p = 0; p < lstln; p++) {
				if(toplststrtln + p/insize >= list.length())
					list+=new arraylist<T>();
				list[toplststrtln + p/insize]->add(l->get(p));
			}
			pos+=lstln;
		}
		
		void steal(HASHLIST<T> *l) { add(l); while(l->length()) l->remove(0); }
		void faststeal(HASHLIST<T> *l) { add(l); while(l->length()) l->remove(l->length()-1); }
		void pillage(HASHLIST<T> *l) { add(l); delete l; }
		
		T get(unsigned int l) { return list[ l/insize ]->operator[]( l%insize ); }
		
		void insert(T l, unsigned int c) {
			if(pos == 0 || c >= pos) { add(l); return; }
			unsigned int out = c/insize, in = c%insize;
			if(insize == 1) { arraylist<T>* p = new arraylist<T>(); p->add(l); list.insert(p,c); pos++; return; }
			list[out]->insert(l,in);
			if((unsigned int)list[out]->length() >= insize) {
				if((unsigned int)list[list.length()-1]->length()+1 >= insize) list += new arraylist<T>();
				for(unsigned int i = out; i < list.length()-1; i++) {
					if((unsigned int)list[list.length()-1]->length()+1 >= insize) list += new arraylist<T>();
					while((unsigned int)list[i]->length() > insize)
						if(list[i]->length()) {
							T itm = list[i]->operator[]( list[i]->length()-1 );
							list[i]->remove( list[i]->length()-1 );
							if(list[i+1]->length() == 0) list[i+1]->add( itm );
							else list[i+1]->insert( itm, 0 );
						}
						else break;
				}
			}
			pos++;
		}
		
		void remove(unsigned int l) {
			unsigned int out=l/insize, in=l%insize;
			list[out]->remove(in);
			if(list[out]->length() == 0) { delete list[out]; list-=out; }
			if(insize>1) for(unsigned int i = out; i < list.length(); i++)
				if(i+1<list.length() && list[i+1]->length()) { list[i]->add(list[i+1]->operator[](0)); list[i+1]->remove(0); }
			if(list.length() && list[list.length()-1]->length()==0) { delete list[list.length()-1]; list-=list.length()-1; }
			pos--;
		}
		
		void FORCEresizeInternalList(unsigned int s) {
			HASHLIST<T> tmp; tmp.faststeal(this);
			insize = s;
			faststeal(&tmp);
		}
		
		T operator[] (unsigned int l) { return get(l); }
		
		HASHLIST<T>& operator+= (T l) { add(l); return *this; }
};

#endif /* _HASHLIST_TEMPLATE_H */
