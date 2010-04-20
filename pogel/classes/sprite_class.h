#ifndef _SPRITE_CLASS_H
#define _SPRITE_CLASS_H

namespace POGEL {
class SPRITE;
}

#include "point_class.h"
#include "object_class.h"

class POGEL::SPRITE : public POGEL::OBJECT {
	public:
		/*SPRITE();
		~SPRITE();*/
		
		void draw();
};

#endif /* _SPRITE_CLASS_H */
