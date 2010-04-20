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
	POGEL::FRACTAL* spawnling = new POGEL::FRACTAL(data, creation, destruction, itterationMax, itterationLevel+1);
	addobject(spawnling);
	return spawnling;
};

POGEL::OBJECT* POGEL::FRACTAL::condense() {
	if(numchildren == 0) {
		POGEL::MATRIX mat(position, rotation);
		POGEL::TRIANGLE* tris = new POGEL::TRIANGLE[numfaces];
		for(unsigned long i = 0; i<numfaces;i++) {
			tris[i] = mat.transformTriangle(face[i]);
		}
		POGEL::OBJECT* obj = new POGEL::OBJECT();
		for(unsigned long f = 0; f < this->getnumfaces() ; f++)
				obj->addtriangle(tris[f]);
		return obj;
	}
	else if(numchildren > 0) {
		POGEL::OBJECT* obj = new POGEL::OBJECT(face, numfaces, 0);
		POGEL::OBJECT* chld;
		for(unsigned long i = 0; i < numchildren; i++) {
			chld = (static_cast<POGEL::FRACTAL*>(children[i]))->condense();
			for(unsigned long f = 0; f < chld->getnumfaces() ; f++)
				obj->addtriangle(chld->gettriangle(f));
			delete chld;
		}
		return obj;
	}
	return NULL;
};

