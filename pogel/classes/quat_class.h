#ifndef _QUAT_CLASS_H
#define _QUAT_CLASS_H

#include <stdio.h>

namespace POGEL {
class QUAT;
}

#include "matrix_class.h"
#include "point_class.h"
#include "../pogel_internals.h"

namespace POGEL {
class QUAT {
	public:
		float x;
		float y;
		float z;
		float w;
		
		QUAT()
			{ x=0.0; y=0.0; z=0.0; w=0.0; }
		QUAT(POGEL::VECTOR, float);
		QUAT(POGEL::MATRIX&);
		
		POGEL::MATRIX tomatrix();
		
		POGEL::VECTOR getvector();
		
		void print()
			{printf("%9.3f,%9.3f,%9.3f,%9.3f",x,y,z,w);}
		
		
		POGEL::QUAT		operator+  (float);
		POGEL::QUAT		operator+  (POGEL::VECTOR);
		
		POGEL::QUAT&	operator=  (const POGEL::QUAT&);
		
		POGEL::QUAT&	operator+= (POGEL::VECTOR);
		POGEL::QUAT&	operator+= (float);
		
		
		//friend class MATRIX;
};
}

#endif /* _QUAT_CLASS_H */
