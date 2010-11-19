#ifndef _BOUNDING_CLASS_H
#define _BOUNDING_CLASS_H

namespace POGEL {
class BOUNDING;
}

#include "point_class.h"
#include "misc_class.h"

#define				BOUNDING_DEFAULT				0
#define				BOUNDING_OBJECT					1
#define				BOUNDING_TRIANGLE				2

#define				BOUNDING_DEFAULT_COLOR			POGEL::COLOR(0.0f,1.0f,1.0f,1.0f)
#define				BOUNDING_TRIANGLE_COLOR			POGEL::COLOR(0.0f,1.0f,0.0f,0.75f)
#define				BOUNDING_OBJECT_COLOR			POGEL::COLOR(1.0f,0.0f,1.0f,1.0f)

namespace POGEL {
class BOUNDING {
	private:
		unsigned long numpoints;
		bool isactual;
	public:
		float maxdistance;
		POGEL::POINT max;
		POGEL::POINT min;
		POGEL::COLOR color;
		
		BOUNDING()
			{maxdistance=0.0f; max=POGEL::POINT(); min=POGEL::POINT(); numpoints=0; color = BOUNDING_DEFAULT_COLOR; isactual = true;}
		BOUNDING(unsigned int type)
			{
				maxdistance=0.0f; max=POGEL::POINT(); min=POGEL::POINT(); numpoints=0;
				switch(type) {
					case BOUNDING_OBJECT: color = BOUNDING_OBJECT_COLOR; break;
					case BOUNDING_TRIANGLE: color = BOUNDING_TRIANGLE_COLOR; break;
					default: color = BOUNDING_DEFAULT_COLOR; break;
				}
				isactual = true;
			}
		BOUNDING(float maximum, float gx, float lx, float gy, float ly, float gz, float lz)
			{maxdistance=maximum; max.x=gx; min.x=lx; max.y=gy; min.y=ly; max.z=gz; min.z=lz; numpoints=1; color = BOUNDING_DEFAULT_COLOR;isactual = true;}
		
		void set(float maximum, float gx, float lx, float gy, float ly, float gz, float lz)
			{maxdistance=maximum; max.x=gx; min.x=lx; max.y=gy; min.y=ly; max.z=gz; min.z=lz; numpoints=1;isactual = true;}
		
		void addpoint(POGEL::POINT,POGEL::POINT);
		void addpoint(POGEL::POINT p)
			{ addpoint(POGEL::POINT(), p); }
		void fin(float f = 0.0f);
		
		void offset(POGEL::POINT);
		
		void clear()
			{set(0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f); maxdistance=0.0f; numpoints=0;isactual = true;}
		
		void finishactual()
			{isactual = false;}
		
		void unsetactual()
			{isactual = true;}
		
		void draw(POGEL::POINT);
		
		bool checkbounding(POGEL::POINT,POGEL::POINT,POGEL::BOUNDING);
		bool checkbounding(POGEL::BOUNDING b)
			{
				return checkbounding(POGEL::POINT(), POGEL::POINT(), b);
			}
		bool isinside(POGEL::POINT,POGEL::POINT);
		bool isoutside(POGEL::POINT,POGEL::POINT);
		
		bool surrounds(POGEL::POINT,POGEL::POINT,POGEL::BOUNDING);
		
		POGEL::BOUNDING& operator= (const POGEL::BOUNDING& b) {
			numpoints = b.numpoints;
			isactual = b.isactual;
			maxdistance = b.maxdistance;
			max = b.max;
			min = b.min;
			color = b.color;
			
			return *this;
		}
};
}

#endif /* _BOUNDING_CLASS_H */
