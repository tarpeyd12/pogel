#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "object_class.h"
#include "../pogel_internals.h"

POGEL::OBJECT::OBJECT() {
	POGEL::POINT p(0.0f,0.0f,0.0f);
	face=NULL;
	numfaces=0;
	triangle_allocation_total=0;
	base=NULL;
	position=p;
	rotation=p;
	children=NULL;
	numchildren=0;
	properties=NULL;
	name=(char*)NULL;
	parent=(POGEL::OBJECT*)NULL;
	visable=true;
	create();
};

POGEL::OBJECT::OBJECT(unsigned int prop) {
	POGEL::POINT p(0.0f,0.0f,0.0f);
	face=NULL;
	numfaces=0;
	triangle_allocation_total=0;
	base=NULL;
	position=p;
	rotation=p;
	children=NULL;
	numchildren=0;
	properties=prop;
	name=(char*)NULL;
	parent=(POGEL::OBJECT*)NULL;
	visable=true;
	create();
};

POGEL::OBJECT::OBJECT(POGEL::TRIANGLE *tri, unsigned long num, unsigned int prop) {
	POGEL::POINT p(0.0f,0.0f,0.0f);
	numfaces=0;
	triangle_allocation_total=0;
	base=NULL;
	position=p;
	rotation=p;
	children=NULL;
	numchildren=0;
	properties=prop;
	addtriangles(tri,num);
	name=(char*)NULL;
	parent=(POGEL::OBJECT*)NULL;
	visable=true;
	create();
};

POGEL::OBJECT::OBJECT(POGEL::TRIANGLE *tri, unsigned long num, unsigned int prop, POGEL::POINT pos, POGEL::POINT rot) {
	numfaces=0;
	triangle_allocation_total=0;
	base=NULL;
	position=pos;
	rotation=rot;
	children=NULL;
	numchildren=0;
	properties=prop;
	addtriangles(tri,num);
	name=(char*)NULL;
	parent=(POGEL::OBJECT*)NULL;
	visable=true;
	create();
};

POGEL::OBJECT::OBJECT(const char* n) {
	POGEL::POINT p(0.0f,0.0f,0.0f);
	triangle_allocation_total=0;
	face=NULL;
	numfaces=0;
	base=NULL;
	position=p;
	rotation=p;
	children=NULL;
	numchildren=0;
	properties=NULL;
	name=(char*)n;
	parent=(POGEL::OBJECT*)NULL;
	visable=true;
	create();
};

POGEL::OBJECT::OBJECT(const char* n, unsigned int prop) {
	POGEL::POINT p(0.0f,0.0f,0.0f);
	face=NULL;
	triangle_allocation_total=0;
	numfaces=0;
	base=NULL;
	position=p;
	rotation=p;
	children=NULL;
	numchildren=0;
	properties=prop;
	name=(char*)n;
	parent=(POGEL::OBJECT*)NULL;
	visable=true;
	create();
};

POGEL::OBJECT::OBJECT(const char* n, POGEL::TRIANGLE *tri, unsigned long num, unsigned int prop) {
	POGEL::POINT p(0.0f,0.0f,0.0f);
	numfaces=0;
	triangle_allocation_total=0;
	base=NULL;
	position=p;
	rotation=p;
	children=NULL;
	numchildren=0;
	properties=prop;
	addtriangles(tri,num);
	name=(char*)n;
	parent=(POGEL::OBJECT*)NULL;
	visable=true;
	create();
};

POGEL::OBJECT::OBJECT(const char* n, POGEL::TRIANGLE *tri, unsigned long num, unsigned int prop, POGEL::POINT pos, POGEL::POINT rot) {
	numfaces=0;
	triangle_allocation_total=0;
	base=NULL;
	position=pos;
	rotation=rot;
	children=NULL;
	numchildren=0;
	properties=prop;
	addtriangles(tri,num);
	name=(char*)n;
	parent=(POGEL::OBJECT*)NULL;
	visable=true;
	create();
};

POGEL::OBJECT::~OBJECT() {
	
	destroy();
	
	char *n = getancestoryhash();
	POGEL::message("deconstructing %s\n", n);
	if(n!=getname()) free(n);
	
	if(face!=NULL) {
		delete[] face;
		face=NULL;
	}
	
	if(children != NULL) {
		killchildren();
		/*if(numchildren > 0 && children != NULL)
			for(unsigned long i=0;i<numchildren;i++) {
				children[i]->killchildren();
				delete children[i];
				children[i]=NULL;
			}
		delete[] children;*/
		children=NULL;
	}
	
	parent=NULL;
	
	if(name!=NULL) {
		delete[] name;
		name=NULL;
	}
};

void POGEL::OBJECT::killchildren() {
	if(numchildren > 0 && children != NULL)
		for(unsigned long i=0;i<numchildren;i++) {
			children[i]->killchildren();
			delete children[i];
			children[i]=NULL;
		}
	delete[] children;
	children=NULL;
};

void POGEL::OBJECT::translate(POGEL::VECTOR v) {
	if(!v.isbad()) position += v.topoint();
};

void POGEL::OBJECT::translate(POGEL::VECTOR v, float s) {
	translate(v*s);
};

void POGEL::OBJECT::rotate(POGEL::VECTOR v) {
	if(!v.isbad())
	rotation += POGEL::MATRIX(v.topoint()*-1.0f, MATRIX_CONSTRUCT_ROTATION).getrotation();
	/*rotation.x+=v.x;
	rotation.y+=v.y;
	rotation.z+=v.z;*/
	
	/*if(rotation.x<0.0f) rotation.x+=360.0f;
	if(rotation.x>360.0f) rotation.x-=360.0f;
	
	if(rotation.y<0.0f) rotation.y+=360.0f;
	if(rotation.y>360.0f) rotation.y-=360.0f;
	
	if(rotation.z<0.0f) rotation.z+=360.0f;
	if(rotation.z>360.0f) rotation.z-=360.0f;*/
};

void POGEL::OBJECT::rotate(POGEL::VECTOR v, float s) {
	rotate(v*s);
};

unsigned long POGEL::OBJECT::addtriangle(POGEL::TRIANGLE tri) {
	if((tri.vertex[0].topoint()==tri.vertex[1].topoint()) || (tri.vertex[0].topoint()==tri.vertex[2].topoint()) || (tri.vertex[1].topoint()==tri.vertex[2].topoint()))
		return (unsigned long)NULL;
	if( numfaces >= triangle_allocation_total )
		addtrianglespace(OBJECT_TRIAGLE_ALLOCATION_SKIP);
	face[numfaces]=tri;
	numfaces++;
	return numfaces-1;	
};

void POGEL::OBJECT::addtriangles(POGEL::TRIANGLE *tri, unsigned long num) {
	if(tri == (POGEL::TRIANGLE*)NULL)
		POGEL::fatality(POGEL_FATALITY_NULL_ARRAY_POINTER_RETNUM,"%s to Triangle(s).",POGEL_FATALITY_NULL_ARRAY_POINTER_STRING);
	addtrianglespace(num);
	for(unsigned long i=0;i<num;i++)
		addtriangle(tri[i]);
};

void POGEL::OBJECT::addtrianglespace(unsigned long num) {
	POGEL::TRIANGLE *tmp = new POGEL::TRIANGLE[numfaces+num];
	for(unsigned long i = 0; i < numfaces; i++) tmp[i] = face[i];
	if(face) delete[] face;
	face = NULL;
	face = tmp;
	triangle_allocation_total += num;
};

unsigned long POGEL::OBJECT::addobject(POGEL::OBJECT *obj) {
	if(obj == (POGEL::OBJECT*)NULL)
		POGEL::fatality(POGEL_FATALITY_NULL_OBJECT_POINTER_RETNUM,"%s to object.",POGEL_FATALITY_NULL_OBJECT_POINTER_STRING);
	POGEL::OBJECT **tmp = new POGEL::OBJECT*[numchildren+1];
	for(unsigned long i=0;i<numchildren;i++) {
		tmp[i]=children[i];
		children[i] = NULL;
	}
	tmp[numchildren]=obj;
	tmp[numchildren]->parent=this;
	
	delete[] children;
	children=tmp;
	
	numchildren++;
	return numchildren-1;
};

void POGEL::OBJECT::addobjects(POGEL::OBJECT **obj, unsigned long num) {
	if(obj == (POGEL::OBJECT**)NULL)
		POGEL::fatality(POGEL_FATALITY_NULL_ARRAY_POINTER_RETNUM,"%s to object(s).",POGEL_FATALITY_NULL_ARRAY_POINTER_STRING);
	for(unsigned long i=0;i<num;i++) {
		if(obj[i] == NULL)
			POGEL::fatality(POGEL_FATALITY_NULL_OBJECT_POINTER_RETNUM|POGEL_FATALITY_NULL_LIST_POINTER_RETNUM,"%s & %s to object.",POGEL_FATALITY_NULL_OBJECT_POINTER_STRING, POGEL_FATALITY_NULL_LIST_POINTER_STRING);
		addobject(obj[i]);
	}
};

void POGEL::OBJECT::scroll_all_tex_values(float s, float t) {
	for(unsigned long i=0;i<numfaces;i++) // loop through all the triangles
		face[i].scroll_tex_values(s,t); // scroll thier texture values
};

POGEL::OBJECT* POGEL::OBJECT::getchild(const char* n) {
	for(unsigned long i=0;i<numchildren;i++) // loop through all the children
		if(strlen(n)==strlen(children[i]->getname()) && strcmp(n,children[i]->getname())==0) // if the childs name matches the desired one
			return children[i]; // return it
	return NULL; // otherwise null
};

POGEL::OBJECT* POGEL::OBJECT::getdecendant(const char* n, bool self) {
	if(getchild(n)!=NULL) // if the decendant is the imediate child
		return getchild(n); // return it
	else if(getchild(n)==NULL) // if the desired object is not an imediate child
		for(unsigned long i=0;i<numchildren;i++) // loop throgh all the children
			if(children[i]->getdecendant(n)!=NULL) // if the desired object is one of the childrens decendants
				return children[i]->getdecendant(n); // return the childs decendant
	else if(strlen(n)==strlen(getname()) && strcmp(getname(), n)==0 && self) // if the decendant you are looking for is this object
		return this; // return this
	return NULL; // otherwise null
};

POGEL::OBJECT* POGEL::OBJECT::getdecendant(const char* n) {
	return getdecendant(n,true); // same as above but it can always check for itself
};

POGEL::OBJECT* POGEL::OBJECT::getprogenitor() {
	if(parent==NULL) // if this object has no parent
		return this; // return this object
	else if(parent!=NULL) // if this object has a parent
		return parent->getprogenitor(); // return the parents progenitor
	return NULL; // otherwise null
};

POGEL::OBJECT* POGEL::OBJECT::getancestor(const char *n) {
	if(strlen(getname()) == strlen(n) && strcmp(getname(), n)==0) // if this object is the object you are looking for
		return this; // return this object
	else if(parent!=NULL) // if this object has a parent
		return parent->getancestor(n); // return the parents ancestor with name 'n'
	return NULL; // otherwise null
};

char* POGEL::OBJECT::getancestory() {
	if(parent==NULL) // if this object has no parent
		return getname(); // it is the first one, return its name
	else if(parent!=NULL) { // if it has a parent
		char *n=parent->getancestory(); // get the parents ancestory
		char *ret=POGEL::string("%s -> %s", n, getname()); // concatinate it with the current objects name
		if(n!=parent->getname()) // if the address of n does not equal the adress of the parents name string
			free(n); // free n (prevents manny memmory leaks)
		return ret; // and return
	}
	POGEL::error("object: \"%s\" has corrupted pointer to parent object.",getname());
	POGEL::warning("nonfatal, continuing");
	return (char*)"ERROR"; // otherwise complain
};

char* POGEL::OBJECT::getancestoryhash() {
	if(parent==NULL) // if this object has no parent
		return getname(); // it is the first one, return its name
	else if(parent!=NULL) { // if it has a parent
		char *n=parent->getancestoryhash(); // get the parents ancestory
		char *ret=POGEL::string("%s:%x", n, getchildslot()); // concatinate it with the current objects name
		if(n!=parent->getname()) // if the address of n does not equal the adress of the parents name string
			free(n); // free n (prevents many memmory leaks)
		return ret; // and return
	}
	POGEL::error("object: \"%s\" has corrupted pointer to parent object.",getname());
	POGEL::warning("nonfatal, continuing");
	return (char*)"ERROR"; // otherwise complain
};

unsigned long POGEL::OBJECT::getchildslot() {
	if(parent!=NULL)
		// loop through all the children
		for(unsigned long i=0;i<parent->numchildren;i++) {
			// if the childs name matches the desired one
			if(parent->children[i] == this)
				return i;
		}
	return 0;
};

POGEL::MATRIX POGEL::OBJECT::getancestorialmatrix() {
	if(parent == NULL)
		return POGEL::MATRIX(position, rotation);
	else if(parent!=NULL)
		return parent->getancestorialmatrix() * POGEL::MATRIX(position, rotation);
	return POGEL::MATRIX();
};

void POGEL::OBJECT::build() {
	unsigned long i;
	if(hasproperty(OBJECT_DRAW_DISPLAYLIST)) {
		base=glGenLists(1);
		glNewList(base,GL_COMPILE);
			for(i=0;i<numfaces;i++)
				face[i].draw();
		glEndList();
	}
	matrix.get();
	
	if(getdecendant(this->getname(),false) != NULL) // if this object has a decendant with the same name, does not check self
		if(getdecendant(this->getname(),false) == this) { // if the object with the same name as this object is this object, does not sheck self
			POGEL::fatality(POGEL_FATALITY_CIRCULAR_HIERARCHY_RETNUM,"%s of object: \"%s\"",POGEL_FATALITY_CIRCULAR_HIERARCHY_STRING,this->getname()); // BADDNESS, complain, and exit the program
		}
};

void POGEL::OBJECT::draw() {
	if(visable) {
		if(hasproperty(OBJECT_ROTATE_TOCAMERA)) {
			matrix.get();
			POGEL::POINT cam_pos = matrix.getposition()*-1;
			matrix.invert();
			matrix.transformPoint(&cam_pos);
			float radius = cam_pos.distance(POGEL::POINT());
			
			rotation.x = POGEL::RadiansToDegrees(acos(cam_pos.y/radius))+90;
			rotation.y = -1*(90+POGEL::RadiansToDegrees(atan2(cam_pos.z, cam_pos.x)))+180;
			rotation.z = 0;
		}
		unsigned long i;
		#ifdef OBJECT_USE_OPNEGL_MATRIX_RECURSION
			glPushMatrix();
		#endif /* OBJECT_USE_OPNEGL_MATRIX_RECURSION */
		glTranslatef(position.x, position.y, position.z);
		
		if(hasproperty(OBJECT_ROTATE_XYZ)) {
			glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
			glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
			glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
		}
		else if(hasproperty(OBJECT_ROTATE_TOCAMERA)) {
			glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
			glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
			glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
		}
		else {
			glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
			glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
			glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
		}
		
		if(hasproperty(OBJECT_DRAW_DISPLAYLIST))
			glCallList(base);
		else
			for(i=0;i<numfaces;i++)
				face[i].draw();
		if(hasproperty(OBJECT_DRAW_CHILDREN))
			for(i=0;i<numchildren;i++) {
				if(POGEL::hasproperty(POGEL_ANCESTORY))
					POGEL::LINE(POGEL::POINT(), children[i]->position, 1, POGEL::COLOR(0,.5,1,1)).draw();
				children[i]->draw();
			}
		
		if(hasproperty(OBJECT_DEBUG)) {
			char *n = getancestory();
			matrix.get();
			POGEL::message("\nfor object: \"%s\":\n", n);
			if(n!=NULL && parent!=NULL)
				free(n);
			POGEL::message("object position:  ");
			position.print();
			POGEL::message("\nobject rotation:  ");
			rotation.print();
			POGEL::message("\nobject direction: ");
			direction.print();
			POGEL::message("\nobject spin:      ");
			spin.print();
			POGEL::message("\nposition from matrix: ");
			matrix.getposition().print();
			POGEL::message("\nrotation from matrix: ");
			matrix.getrotation().print();
			POGEL::message("\nthe matrix: ");
			matrix.print();
			POGEL::message("as quaternion: ");
			matrix.toquat().print();
			/*if(matrix.invert()) {
				POGEL::message("\ninverted matrix: ");
				matrix.print();
				POGEL::message("\nrotation from inverted matrix: ");
				matrix.getrotation().print();
				POGEL::message("\nas quaternion: ");
				matrix.toquat().print();
			}*/
			POGEL::message("\n\n");
		}
		#ifdef OBJECT_USE_OPNEGL_MATRIX_RECURSION
			glPopMatrix();
		#else
		if(hasproperty(OBJECT_ROTATE_XYZ)) {
			glRotatef(rotation.z, 0.0f, 0.0f, -1.0f);
			glRotatef(rotation.y, 0.0f, -1.0f, 0.0f);
			glRotatef(rotation.x, -1.0f, 0.0f, 0.0f);
		}
		else if(hasproperty(OBJECT_ROTATE_TOCAMERA)) {
			glRotatef(rotation.z, 0.0f, 0.0f, -1.0f);
			glRotatef(rotation.x, -1.0f, 0.0f, 0.0f);
			glRotatef(rotation.y, 0.0f, -1.0f, 0.0f);
		}
		else {
			glRotatef(rotation.x, -1.0f, 0.0f, 0.0f);
			glRotatef(rotation.y, 0.0f, -1.0f, 0.0f);
			glRotatef(rotation.z, 0.0f, 0.0f, -1.0f);
		}
		glTranslatef(-position.x, -position.y, -position.z);
		#endif /* OBJECT_USE_OPNEGL_MATRIX_RECURSION */
	}
	if(POGEL::hasproperty(POGEL_LABEL)) {
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_LIGHTING);
		glColor3f(0.2f,0.5f,1.0f);
		glPointSize(4);
		glBegin(GL_POINTS);
			glVertex3f(position.x, position.y, position.z);
		glEnd();
		glPointSize(1);
		glColor3f(1.0f,1.0f,1.0f);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);
	}
};

