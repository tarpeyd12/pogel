#ifndef _TRIANGLE_CLASS_H
#define _TRIANGLE_CLASS_H

namespace POGEL {
class TRIANGLE;
}

#include "point_class.h"
#include "image_class.h"
#include "matrix_class.h"
#include "bounding_class.h"
#include "../pogel_internals.h"

#define			TRIANGLE_LIT				2
#define			TRIANGLE_COLORED			4
#define			TRIANGLE_VERTEX_NORMALS		8
#define			TRIANGLE_INVERT_NORMALS		16

namespace POGEL {
class TRIANGLE {
	private:
		unsigned int properties; // the mushed options
	public:
		POGEL::BOUNDING bounding;
		POGEL::VERTEX vertex[3]; // the 3 verticies of the triangle
		POGEL::IMAGE *texture; // a pointer to the image to use as the texture
		POGEL::VECTOR normal; // the normal vector
		
		TRIANGLE() {texture=NULL;}
		TRIANGLE(POGEL::VERTEX a,POGEL::VERTEX b,POGEL::VERTEX c,POGEL::IMAGE *tex,unsigned int prop) {load(a,b,c,tex,prop);}
		TRIANGLE(POGEL::POINT a,POGEL::POINT b,POGEL::POINT c,POGEL::IMAGE *tex,unsigned int prop) {load(POGEL::VERTEX(a),POGEL::VERTEX(b),POGEL::VERTEX(c),tex,prop);}
		TRIANGLE(POGEL::VERTEX* verts,POGEL::IMAGE *tex,unsigned int prop) {load(verts,tex,prop);}
		
		void load(POGEL::VERTEX,POGEL::VERTEX,POGEL::VERTEX,POGEL::IMAGE*,unsigned int);
		void load(POGEL::VERTEX*,POGEL::IMAGE*,unsigned int);
		
		void settexture(POGEL::IMAGE *tex) {texture=tex;}
		POGEL::IMAGE *gettexture() {return texture;}
		
		PROPERTIES_METHODS;
		
		void scroll_tex_values(float,float);
		
		void print() {
			printf("\n");
			vertex[0].topoint().print();
			printf("\n");
			vertex[1].topoint().print();
			printf("\n");
			vertex[2].topoint().print();
			printf("\n");
		}
		
		POGEL::TRIANGLE transform(POGEL::MATRIX*);

		POGEL::POINT middle();
		
		bool isinfront(POGEL::POINT);
		
		bool distcheck(POGEL::POINT, float);
		
		void getbounding();
		
		void draw();
};
}

#endif /* _TRIANGLE_CLASS_H */
