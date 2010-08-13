#include "sprite_class.h"

#include <math.h>

void POGEL::SPRITE::draw() {
	matrix.get();
	
	/*matrix.print();
	rotation = (matrix.getrotation() - POGEL::POINT(0.0f,360.0f,0.0f))*-1;
	rotation.print();*/
	
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
	
	
	//	POGEL::VECTOR 
	float r = POGEL::POINT().distance(matrix.getposition());
	POGEL::POINT cam_pos = matrix.getposition();
	POGEL::POINT rot = POGEL::POINT(POGEL::RadiansToDegrees(acos(cam_pos.y/r))+90, -1*(90+POGEL::RadiansToDegrees(atan2(cam_pos.z, cam_pos.x)))+180, 0);
	
	rotation = (POGEL::MATRIX(rot.x, MATRIX_CONSTRUCT_X_ROTATION) * MATRIX(rot.y, MATRIX_CONSTRUCT_Y_ROTATION) * MATRIX(rot.z, MATRIX_CONSTRUCT_Z_ROTATION)).getrotation();
	rot.print();printf("\n");
	rotation.print();
	//POGEL::OBJECT::draw();
	
	moveto(POGEL::POINT());
	rotate(POGEL::VECTOR(  ));
	
	/*printf("\n");
	rotation.print();*/
	
	POGEL::OBJECT::draw();
};
