#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <iostream>
#include <fstream>

// make sure that in "pogel/pogel_internals.h" that NO_OPENGL is defined.

#include "pogel/pogel.h"
#include "pogel/classes/physics/physics.h"

using namespace std;
using namespace POGEL;

unsigned int loopnum, objnum;

#define grd objnum
#define numobjs (grd*grd*grd)

#define svfrq 1000/numobjs

#define sps 1.0f/2
#define size 1.0f/20
POGEL::OBJECT *obj;
POGEL::PHYSICS::SOLID **sphs;
POGEL::PHYSICS::SIMULATION sim;

void exitingfunction() {
	char *flnm = POGEL::string("log%d.txt",numobjs);
	std::ofstream outfile(flnm, std::ios_base::trunc); free(flnm);
	for(unsigned long a=0;a<numobjs;a++) outfile << sphs[a]->toString() << "\n";
	outfile.close();
};

int main (int argc, char const* argv[])
{
	POGEL::getcmd(argc,(char**)argv);
	unsigned int savefreq;
	bool minput = true;
	for(unsigned int i = 0; i < argc; i++)
		if(!strncmp(argv[i],"-runprops[loops:%u,objnum:%u,savefrq:%u]", 16)) {
			minput = false;
			sscanf(argv[i], "-runprops[loops:%u,objnum:%u,savefrq:%u]", &loopnum, &objnum, &savefreq);
			break;
		}
	if(minput) {
		cout << "number of times to loop: "; cin >> loopnum;
		cout << "number of objects(will be cubed): "; cin >> objnum;
		cout << "number of itterations between saves: "; cin >> savefreq;
	}
	sim.boundingskips = 0;
    addproperty(POGEL_DEBUG);
	srand((unsigned)time(NULL));
	sphs = new POGEL::PHYSICS::SOLID*[numobjs];
	obj = new POGEL::OBJECT[numobjs];
	for(unsigned int i=0;i<numobjs;i++) {
		obj[i].setname(POGEL::string("sphere%d",i));
		if(i == 0) addDisk(&obj[0], 6, 1, size/2.0f, 0.0f, NULL,1, 1, 0|TRIANGLE_COLORED, false, POGEL::MATRIX());
		else obj[i].copytriangles(&obj[0]);
		obj[i].setproperties(OBJECT_ROTATE_TOCAMERA|OBJECT_DRAW_CHILDREN/*|OBJECT_DEBUG|OBJECT_DRAW_DISPLAYLIST*/);
		obj[i].moveto(POGEL::POINT(
			((float)(i%grd)*sps)-( (float(grd)*sps)/2.0f-sps/2.0f),
			((float)((i/grd)%grd)*sps)-( (float(grd)*sps)/2.0f-sps/2.0f),
			(float)(i/(grd*grd))*(sps) - sps*float(grd)/2.0f + sps/2.0f /* - (10.0f-(sps/2.0f)), \*/
		));
		if(i == 0)
		for(unsigned int a = 0; a < obj[i].getnumfaces(); a++) {
			POGEL::TRIANGLE t = obj[i].gettriangle(a);
			for(int b = 0; b < 3; b++) t.vertex[b].color = POGEL::COLOR(.2,.5,1,1);
			obj[i].settriangle(a, t);
		}
		sphs[i] = new POGEL::PHYSICS::SOLID(&obj[i], POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1.0f, 0.75f, 5000.0f, 1.0f, 1.0f, 1.0f, false, (i%2==0?-1.0f:1.0f)), 2|4|(i%2==0 && false ? 0 : 16));
		sphs[i]->turnto(POINT(0.0f,0.0f,0.0f));
		//sphs[i]->build();
		sphs[i]->setstepstaken(0);
		sphs[i]->visable = true;
		sim.addSolid(sphs[i]);
	}
	char *flnm = POGEL::string("log%d.txt",numobjs);
	std::ifstream ifs(flnm, std::ifstream::in); free(flnm);
	if(ifs.good()) {
		std::string line, pos_str, dir_str;
		for(unsigned int i=0;i<numobjs;i++) {
			if(!ifs.good()) break;
			line.clear(); std::getline(ifs,line,'\n');
			sphs[i]->position  = POGEL::POINT (POGEL::getStringComponentLevel('{','}',line,"0 0"));
			sphs[i]->direction = POGEL::VECTOR(POGEL::getStringComponentLevel('{','}',line,"0 2"));
			sphs[i]->build();
		}
	} ifs.close();
	POGEL::InitFps();
	for(unsigned int i = 0; i < loopnum; i++) {
		POGEL::IncrementFps();
		POGEL::PrintFps();
		sim.increment();
		if(i%savefreq==0) exitingfunction();
	}
	exitingfunction();
	cout << endl;
	return 0;
}
