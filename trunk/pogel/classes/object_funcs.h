#ifndef _OBJECT_FUNCS_H
#define _OBJECT_FUNCS_H

#include "point_class.h"
#include "triangle_class.h"
#include "object_class.h"

#ifndef PI
	#define PI 3.1415926535897
#endif

void addSphere(POGEL::OBJECT *obj, float spacev, float spaceh, float R, POGEL::IMAGE *img, float imgscalev, float imgscaleh, unsigned int triprop);
void addSphere(POGEL::OBJECT *obj, float spacev, float spaceh, float R, POGEL::IMAGE *img, float imgscalev, float imgscaleh, unsigned int triprop, POGEL::MATRIX mat);
void addDisk(POGEL::OBJECT *obj, float divisions, float rings, float outer_radius, float inner_radius, POGEL::IMAGE *img, float imgscalev, float imgscaleh, unsigned int triprop);
void addDisk(POGEL::OBJECT *obj, float divisions, float rings, float outer_radius, float inner_radius, POGEL::IMAGE *img, float imgscalev, float imgscaleh, unsigned int triprop, bool radial_tex, POGEL::MATRIX mat);

void addCylinder(POGEL::OBJECT *obj, float divisions, float rings, float height, float lower_radius, float upper_radius, POGEL::IMAGE *img, float imgscalev, float imgscaleh, unsigned int triprop, POGEL::MATRIX mat);

void addCube(POGEL::OBJECT *obj, float height, float width, float depth, POGEL::IMAGE* img, float imgscale_h, float imgscale_w, unsigned int triprop, POGEL::MATRIX mat);

#endif /* _OBJECT_FUNCS_H */
