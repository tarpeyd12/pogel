#include "sprite_class.h"

#include <math.h>

void POGEL::SPRITE::draw() {
	/*matrix.get();
	POGEL::POINT cam_pos = matrix.getposition() - position;
	POGEL::MATRIX(matrix.getrotation()*1, MATRIX_CONSTRUCT_ROTATION).transformPoint(&cam_pos);
	cam_pos*-1;
	cam_pos.print();
	float radius = cam_pos.distance(position);
	POGEL::message("radius = %f\n",radius);
	
	rotation.x = POGEL::RadiansToDegrees(acos(cam_pos.y/radius))+90;
	rotation.y = -1*(90+POGEL::RadiansToDegrees(atan2(cam_pos.z, cam_pos.x)))+180;
	rotation.z = 0;*/
	
	//matrix.print();
	POGEL::POINT rot = matrix.getrotation();
	rotation = (POGEL::MATRIX(rot.y*-1, MATRIX_CONSTRUCT_Y_ROTATION) * MATRIX(rot.x, MATRIX_CONSTRUCT_X_ROTATION)).getrotation();
	rotation.x +=90;
	rotation.y +=180;
	rotation.z = 0;
	//rotation.print();
	
	//POGEL::OBJECT::draw();
	
	
	
	// gives the proper rotation of a sprite to face a camera
	// NOTE: when rotating the sprite rotate in the order: Y, X, Z.
	/*point3f ret;
	point3f cam_pos=inrelto(campos, sprpos);
	float radius=distance(sprpos, campos);
	//float tmp;
	
	ret.x=rtd(acos(cam_pos.y/radius))+90;
	ret.y=-1*(90+rtd(atan2(cam_pos.z, cam_pos.x)))+180;
	ret.z=0;
	
	return ret;*/
	
	
	
	
	POGEL::OBJECT::draw();
};
