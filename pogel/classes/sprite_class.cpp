#include "sprite_class.h"

#include <math.h>

void POGEL::SPRITE::draw() {
	matrix.get();
	POGEL::POINT cam_pos = matrix.getposition()*-1;
	POGEL::POINT r = matrix.getrotation();
	//POGEL::MATRIX(POGEL::POINT(r.x,r.y,r.z), MATRIX_CONSTRUCT_ROTATION)
	matrix.invert();
	matrix.transformPoint(&cam_pos);
	//cam_pos*-1;
	cam_pos.print();
	float radius = cam_pos.distance(POGEL::POINT());
	POGEL::message("radius = %f\n",radius);
	
	rotation.x = POGEL::RadiansToDegrees(acos(cam_pos.y/radius))+90;
	rotation.y = -1*(90+POGEL::RadiansToDegrees(atan2(cam_pos.z, cam_pos.x)))+180;
	//rotation.z = 0;
	
	addproperty(OBJECT_ROTATE_TOCAMERA);
	
	POGEL::OBJECT::draw();
};
