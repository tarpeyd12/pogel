#ifndef _CLASSLIST_TEMPLATE_H
#define _CLASSLIST_TEMPLATE_H

#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class CLASSLIST {
	private:
		T *list;
		unsigned int len;
		bool sc;
	protected:
		T* getList() { return list; }
	public:
		CLASSLIST() { list=NULL; len=0; sc=false;}
		CLASSLIST(T *c, unsigned int l) { list=c; len=l; }
		~CLASSLIST() { if(list!=NULL)delete[]list; list=NULL; len=0; }
		
		void clear() { if(list!=NULL)delete[]list; list=NULL; len=0; }
		void safeclear() { if(list!=NULL)delete list; list=NULL; len=0; }
		
		unsigned int length() { return len; }
		
		void add(T l) {
			T *tmp = new T[length()+1];
			for(unsigned int i=0;i<length();i++) tmp[i]=list[i];
			tmp[len++]=l; if(list!=NULL)delete[]list; list=tmp;
		}
		
		void add(T *c, unsigned int l) {
			if(c==NULL||l==0) return;
			T *tmp = new T[length()+l];
			for(unsigned int i=0;i<(length()>l?length():l);i++) { if(i<length())tmp[i]=list[i]; if(i<l)tmp[i+length()]=c[i]; }
			len+=l; if(list!=NULL)delete[]list; list=tmp;
		}
		
		void remove(unsigned int l) {
			if(l>=length()) return;
			T *tmp = new T[--len];
			for(unsigned int i=0;i<length();i++) if(i<l)tmp[i]=list[i]; else tmp[i]=list[i+1];
			if(list!=NULL)delete[]list; list=tmp;
		}
		
		void insert(T c, unsigned int l) {
			if(l>=length()) return;
			T *tmp = new T[++len];
			for(unsigned int i=0;i<length()-1;i++) if(i<l)tmp[i]=list[i]; else tmp[i+1]=list[i];
			tmp[l]=c; if(list!=NULL)delete[]list; list=tmp;
		}
		
		T get(unsigned int i) { return list[i]; }
		
		T first() { return get(0); }
		T last() { return get(length()-1); }
		
		T operator[] (unsigned int i) { return get(i); }
		
		CLASSLIST<T>& operator = (CLASSLIST<T> c)	{ if(list!=NULL)delete[]list; list=c.getList(); len=c.length(); return *this;}
		
		CLASSLIST<T>  operator + (T l)				{ CLASSLIST<T> v; v.add(getList(),length()); v.add(l); return v; }
		CLASSLIST<T>& operator+= (T l)				{ add(l); return*this; }
		
		CLASSLIST<T>  operator + (CLASSLIST<T> l)	{ CLASSLIST<T> v; v.add(getList(),length()); v.add(l.getList(),l.length()); return v; }
		CLASSLIST<T>& operator+= (CLASSLIST<T> l)	{ add(l.getList(),l.length()); return*this; }
		
		CLASSLIST<T>  operator - (unsigned int l)	{ CLASSLIST<T> v; v.add(getList(),length()); v.remove(l); return v; }
		CLASSLIST<T>& operator-= (unsigned int l)	{ remove(l); return*this; }
};

#endif /* _CLASSLIST_TEMPLATE_H */

