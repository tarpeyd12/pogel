#include "sprite_class.h"

void POGEL::SPRITE::draw() {
	matrix.get();
	
	rotation = matrix.getrotation()*-1.0f;
	
	POGEL::OBJECT::draw();
};
