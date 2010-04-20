#ifndef _MATRIX_CLASS_H
#define _MATRIX_CLASS_H

#include <GL/gl.h>
#include <math.h>

namespace POGEL {
class MATRIX;
}

#include "quat_class.h"
#include "point_class.h"
#include "triangle_class.h"
#include "../pogel_internals.h"

#ifndef PI
	#define PI 3.1415926535897
#endif

//#define				MATRIX_ROTATE_XYZ				2
//#define				MATRIX_ROTATE_ZYX				4
//#define				MATRIX_ROTATE_FIRST				8
//#define				MATRIX_

#define				MATRIX_CONSTRUCT_X_ROTATION		0
#define				MATRIX_CONSTRUCT_Y_ROTATION		1
#define				MATRIX_CONSTRUCT_Z_ROTATION		2

#define				MATRIX_CONSTRUCT_ROTATION		0
#define				MATRIX_CONSTRUCT_TRANSLATION	1
#define				MATRIX_CONSTRUCT_SCALE			2

namespace POGEL {
float RadiansToDegrees(float);
float DegreesToRadians(float);


class MATRIX {
	public:
		float matrix[16];
		//unsigned int properties; // the mushed properties
		
		MATRIX();
		MATRIX(float*);
		MATRIX(const POGEL::MATRIX&);
		MATRIX(float,int);
		MATRIX(POGEL::POINT,int);
		MATRIX(POGEL::VECTOR,float);
		MATRIX(POGEL::POINT,POGEL::POINT);
		
		~MATRIX()
			{/*delete[] matrix;*/}
		
		//unsigned int getproperties() {return properties;}
		//void setproperties(unsigned int prop) {properties=prop;}
		
		void get()
			{glGetFloatv(GL_MODELVIEW_MATRIX,matrix);}
		
		void set(float*);
		void set(POGEL::MATRIX);
		
		//void setRotation(POGEL::POINT);
		//void setPosition(POGEL::POINT);
		
		POGEL::POINT getposition();
		POGEL::POINT getrotation();
		
		float *getcolumn(int);
		float *getrow(int);
		
		float getvalue(int c, int r)
			{return matrix[c*4+r];}
		void setvalue(int c, int r, float v)
			{matrix[c*4+r]=v;}
		
		void multiplyby(float);
		void raistopower(int);
		
		float determinant();
		bool invert();
		
		void print();
		
		void transformPoint(POGEL::POINT*);
		POGEL::POINT transformPoint(POGEL::POINT);
		void transformVector(POGEL::VECTOR*);
		POGEL::VECTOR transformVector(POGEL::VECTOR);
		void transformVertex(POGEL::VERTEX*);
		POGEL::VERTEX transformVertex(POGEL::VERTEX);
		
		void transformTriangle(POGEL::TRIANGLE*);
		POGEL::TRIANGLE transformTriangle(POGEL::TRIANGLE);
		
		QUAT toquat();
		
		void fromaxis(POGEL::VECTOR,float);
		
		POGEL::MATRIX operator+(POGEL::MATRIX);
		POGEL::MATRIX operator-(POGEL::MATRIX);
		POGEL::MATRIX operator*(POGEL::MATRIX);
		POGEL::MATRIX operator*(float a)
			{
				POGEL::MATRIX ret;
				for(int i=0;i<16;i++)
					ret.matrix[i] = matrix[i] * a;
				return ret;
			}
		POGEL::MATRIX& operator=(const POGEL::MATRIX&);
		
		//friend class QUAT;
};
}

#endif /* _MATRIX_CLASS_H */
