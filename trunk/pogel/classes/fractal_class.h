#ifndef _FRACTAL_CLASS_H
#define _FRACTAL_CLASS_H

namespace POGEL {
class FRACTAL;
}

#include "object_class.h"
#include "point_class.h"

#define			FRACTAL_FNC_DEFS		POGEL::FRACTAL* obj, unsigned long itter

class POGEL::FRACTAL : public POGEL::OBJECT {
	protected:
		void *data;
		void (*creation)(POGEL::FRACTAL* thisinstance, unsigned long itteration);
		void (*destruction)(POGEL::FRACTAL* thisinstance, unsigned long itteration);
		
		unsigned long itterationLevel;
		unsigned long itterationMax;
	public:
		FRACTAL();
		FRACTAL(void* dat, void (*cons)(POGEL::FRACTAL*,unsigned long), void (*dest)(POGEL::FRACTAL*,unsigned long), unsigned long totalItterations, unsigned long currentItteration);
		FRACTAL(void* dat, void (*cons)(POGEL::FRACTAL*,unsigned long), void (*dest)(POGEL::FRACTAL*,unsigned long), unsigned long totalItterations);
		~FRACTAL();
		
		void setCreationFunction(void (*func)(POGEL::FRACTAL*,unsigned long)) 
			{ creation = func; }
		void setDestructionFunction(void (*func)(POGEL::FRACTAL*,unsigned long)) 
			{ destruction = func; }
		void setDataStructure(void * dat)
			{ data = dat; }
		
		unsigned long getItteration()
			{ return itterationLevel; }
		unsigned long getMaxItteration()
			{ return itterationMax; }
		
		void *getData()
			{ return data; }
		
		//void build();
		//void draw();
		
		void step();
		
		void create();
		void grow();
		
		void destroy()
			{ destruction(this, itterationLevel); }
		
		POGEL::FRACTAL* spawn();
		
		POGEL::OBJECT* condense();
};

#endif /* _FRACTAL_CLASS_H */
