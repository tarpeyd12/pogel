#include "fractal_class.h"

POGEL::FRACTAL::FRACTAL() : POGEL::OBJECT() {
	data = NULL;
	creation = NULL;
	destruction = NULL;
	
	itterationLevel = 0;
	
	setproperties(8);
};

POGEL::FRACTAL::FRACTAL(
						void* dat, 
						void (*cons)(POGEL::FRACTAL*,unsigned long), 
						void (*dest)(POGEL::FRACTAL*,unsigned long), 
						unsigned long totalItterations, 
						unsigned long currentItteration
						) : POGEL::OBJECT()
	{
	
	itterationLevel = currentItteration;
	itterationMax = totalItterations;
	
	data = dat;
	
	creation = cons;
	destruction = dest;
	
	setproperties(8);
	
	//create();
};

POGEL::FRACTAL::FRACTAL(
						void* dat, 
						void (*cons)(POGEL::FRACTAL*,unsigned long), 
						void (*dest)(POGEL::FRACTAL*,unsigned long), 
						unsigned long totalItterations
						) : POGEL::OBJECT()
	{
	
	itterationLevel = 0;
	itterationMax = totalItterations;
	
	data = dat;
	
	creation = cons;
	destruction = dest;
	
	setproperties(8);
	
	//create();
};

POGEL::FRACTAL::~FRACTAL() {
	destroy();
	
	//deconstruct();
	
	//delete data;
	creation = NULL;
	destruction = NULL;
};

void POGEL::FRACTAL::step() {
	rotate(spin);
	translate(direction);
	for(unsigned long i = 0; i < numchildren ; i++)
			children[i]->step();
};

void POGEL::FRACTAL::create() {
	if(itterationLevel != itterationMax) {
		creation(this, itterationLevel);
		for(unsigned long i = 0; i < numchildren ; i++)
			children[i]->create();
	}
};

void POGEL::FRACTAL::grow() {
	itterationMax++;
	
	if(numchildren == 0)
		creation(this, itterationLevel);
	else
		for(unsigned long i = 0; i < numchildren ; i++)
			static_cast<POGEL::FRACTAL*>(children[i])->grow();
};

POGEL::FRACTAL* POGEL::FRACTAL::spawn() {
	//POGEL::message("spawning\n");
	if(itterationLevel+1 < itterationMax) {
		POGEL::FRACTAL* spawnling = new POGEL::FRACTAL(data, creation, destruction, itterationMax, itterationLevel+1);
		addobject(spawnling);
		return spawnling;
	}
	return NULL;
};

POGEL::OBJECT* POGEL::FRACTAL::condense() {
	POGEL::message("condensing level %u\n", itterationLevel);
	
	// the base-case
	if(numchildren == 0) {
		// create the transformation matrix
		POGEL::MATRIX mat(position, rotation);
		
		// object to be returned
		char* n;
		if(parent==NULL)
			n = getname();
		else
			n = POGEL::string("condense_level_%u", itterationLevel);
		POGEL::OBJECT* obj = new POGEL::OBJECT(n);
		
		/*if(parent!=NULL)
			delete[] n;*/
		
		// add the transformed triangles into the object
		for(unsigned long f = 0; f < this->getnumfaces() ; f++)
			obj->addtriangle(mat.transformTriangle(face[f]));
		
		// return
		return obj;
	}
	// the continuing case
	else if(numchildren > 0) {
		
		// create the transformation matrix
		POGEL::MATRIX mat(position, rotation);
		
		// create a new object with this objects triangles.
		char* n;
		if(parent==NULL)
			n = getname();
		else
			n = POGEL::string("condense_level_%u", itterationLevel);
		POGEL::OBJECT* obj = new POGEL::OBJECT(n);
		
		/*if(parent!=NULL)
			delete[] n;*/
		
		// add the transformed triangles into the object
		for(unsigned long f = 0; f < this->getnumfaces() ; f++)
			obj->addtriangle(mat.transformTriangle(face[f]));
		
		// pointer to temporary condensed childs
		POGEL::OBJECT* chld;
		
		// go through all the children
		for(unsigned long i = 0; i < numchildren; i++) {
			
			// get the condensed child. statically cast to use the condense method.
			POGEL::FRACTAL* tmp = static_cast <POGEL::FRACTAL*> ( children[i] );
			chld = tmp->condense();
			
			// add the childs triangles
			for(unsigned long f = 0; f < chld->getnumfaces() ; f++)
				obj->addtriangle(mat.transformTriangle(chld->gettriangle(f)));
			
			// cleanup
			delete chld;
		}
		
		// return
		return obj;
	}
	
	// if no cases were met, return NULL as a precaution
	return NULL;
};

