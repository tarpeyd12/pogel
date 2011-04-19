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

#define LOOP 0
#define NUM  1
#define FRQ  2
#define FLE  3
#define PRP  4
#define GRD  5
#define SZE  6
#define KTRI 7
#define L2D  8
#define RPOS 9

namespace qsim {
	bool minput[10];
	unsigned int savefreq, loopnum, objnum;
	
	bool usetime = false;
	bool savenotri = false;
	bool rendatsave = false;
	float massvar = 0.0;
	float randpos = 0.0;
	
	POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES objprps;
	
	std::string flnm;
	std::string imgflnm;
	std::string rendpath, rendcpos = "{[0],[0],[-1]}";
	
	POGEL::OBJECT *obj;
	HASHLIST<POGEL::PHYSICS::SOLID*> sphs;
	POGEL::PHYSICS::SIMULATION sim;
	
	float grid_space = 1.0/2.0;
	float ball_size = 1.0/20.0;
};

using namespace qsim;

#define grd objnum
#define numobjs ( minput[L2D] ? grd*grd : grd*grd*grd )

#define sps grid_space
#define size ball_size

void save() {
	std::ofstream outfile(flnm.c_str(), std::ios_base::trunc);
	for(unsigned long a=0;a<sphs.length();a++) {
		std::string s = sphs[a]->toString();
		if(savenotri && a > 0)
			s = POGEL::getStringSection('{',1,true,'<',1,true, s) + ">}";
		outfile << s << "\n";
	}
	outfile.close();
};

void load(char* s) {
	flnm = s;
	std::ifstream ifs(flnm.c_str(), std::ifstream::in);
	if(ifs.good()) {
		printf("\nLoading sphere positions ...\n");
		std::string line;
		unsigned int p = 0;
		POGEL::PHYSICS::SOLID* fst;
		while(ifs.good()) {
			printf("%u\r",p++);
			line.clear(); std::getline(ifs,line,'\n');
			if(!ifs.good() || !line.length()) break;
			POGEL::PHYSICS::SOLID* tmp = new POGEL::PHYSICS::SOLID(line);
			tmp->build();
			if(!minput[KTRI])
				if(p<=1) fst = tmp;
				else {
					tmp->cleartriangles();
					tmp->referencetriangles(fst);
				}
			sphs.add(tmp);
			sim.addSolid(tmp);
		}
		printf("\n");
		minput[FLE] = minput[NUM] = true;
	}
	ifs.close();
};

void generate() {
	//sphs = new POGEL::PHYSICS::SOLID*[numobjs];
	obj = new POGEL::OBJECT[numobjs];
	printf("Generating spheres ...\n");
	POGEL::IMAGE *img;
	if(imgflnm.length())
		img = POGEL::requestImage("{["+imgflnm+"],[0],[0],[0]}");
	else
		img = NULL;
	for(unsigned int i=0;i<numobjs;i++) {
		printf("\r%u",i);
		obj[i].setname(POGEL::string("sphere%d",i));
		if(i == 0)
			addDisk(&obj[0], 6, 1, size/2.0f, 0.0f, img,1, 1, 0|TRIANGLE_COLORED, false, POGEL::MATRIX());
		else
			obj[i].referencetriangles(&obj[0]);
		obj[i].setproperties(OBJECT_ROTATE_TOCAMERA|OBJECT_DRAW_CHILDREN/*|OBJECT_DEBUG|OBJECT_DRAW_DISPLAYLIST*/);
		if(minput[RPOS]) {
			obj[i].moveto(POGEL::POINT(
				POGEL::FloatRand(randpos*2.0)-randpos,
				POGEL::FloatRand(randpos*2.0)-randpos,
				POGEL::FloatRand(randpos*2.0)-randpos
			));
		}
		else
			obj[i].moveto(POGEL::POINT(
				((float)(i%grd)*sps)-( (float(grd)*sps)/2.0f-sps/2.0f),
				((float)((i/grd)%grd)*sps)-( (float(grd)*sps)/2.0f-sps/2.0f),
				(float)(i/(grd*grd))*(sps) - sps*float(grd)/2.0f + sps/2.0f /* - (10.0f-(sps/2.0f)), \*/
			));
		
		if(minput[L2D]) obj[i].position.z=0;
		
		if(i == 0)
			for(unsigned int a = 0; a < obj[i].getnumfaces(); a++) {
				POGEL::TRIANGLE t = obj[i].gettriangle(a);
				for(int b = 0; b < 3; b++) t.vertex[b].color = POGEL::COLOR(.2,.5,1,1);
				obj[i].settriangle(a, t);
			}
		POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES phprp;
		if(!minput[PRP])
			phprp = POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(1.0f, 0.75f, 5000.0f, 1.0f, 1.0f, 1.0f, false, (i%2==0?-1.0f:1.0f));
		else
			phprp = objprps;
		
		phprp.mass += POGEL::FloatRand(massvar*2.0)-massvar;
		
		sphs += new POGEL::PHYSICS::SOLID(&obj[i], phprp, 2|4|16);
		sphs[i]->turnto(POINT(0.0f,0.0f,0.0f));
		sphs[i]->build();
		sphs[i]->setstepstaken(0);
		sphs[i]->visable = true;
		sim.addSolid(sphs[i]);
	}
};

void init(int argc, char** argv) {
	POGEL::getcmd(argc,argv);
	for(unsigned int i = 0; i < 10; i++)
		minput[i] = false;
	
	// parse dangerous cmd options first
	for(unsigned int i = 0; i < argc; i++) {
		
		if(strlen(argv[i]) == 5 && !strncmp(argv[i],"-help", 5)) {
			printf(
				"\n"
				"cmd optins:\n"
				"\n"
				"-usetime\tmakes the loops option use seconds instead of itterations as an arg\n"
				"-keeptriangles\tforces all objects to fully load triangle data from specified file, assumes different shapes and sizes, see '-f'\n"
				"-savenotri\tforces the saving of the data to not include triangle data in the save file for all particles except the\n"
						"\t\t\tfirst, assumes identicle shape and size\n"
				"-f filename\tloads all properties of objects in specified file, all their triangles will be identical to the triangles\n"
						"\t\t\tof the first objects triangles in the file\n"
				"-i bitmap\tpath to bitmap (*.bmp) file to put on the particles internaly for viewing later\n"
				"-2d\t\tforces a 2 dimentional grid on init, makes '-objnum' squared not cubed\n"
				"-objprps {[friction],[bounce],[mass],[air friction],[dencity],[volume],[magnetic 1:0],[magnetic charge]}\n"
				"-gridspace=[space between particles on the initial grid]\n"
				"-ballsize=[particle radius at initilization]\n"
				"-loops=[itteratins to loop, or time to loop]\n"
				"-objnum=[number of objects to start with, will be cubed, ex: '-objnum=10' will be 1000 objects, see '-2d']\n"
				"-savefrq=[the number of itterations between data saves, 0 is never]\n"
				"-threads=[number of POSIX threads to be used in calculations]\n"
				"-massvar=[the ammount to vary the mass of each particle]\n"
				"-randpos=[tha x, y, and z varience from origin to palce the particle at init]\n"
				"-rend path"
				"\n"
				"example:\n"
					"\tsb -f testfile.dat -usetime -loops=1:30:15 -savefrq=100 -2d -objnum=10 -objprps {[1.0],[0.5],[100.0],[0.0],[1.0],[1.0],[0],[0.0]} -gridspace=0.5 -ballsize=0.25 -savenotri -i ./Data/particle.bmp -threads=2 -rend ./reders/ {[0],[0],[-2]}\n"
				"\n"
			);
			exit(0);
		} else
		
		if(strlen(argv[i]) == 8 && !strncmp(argv[i],"-usetime", 8)) {
			usetime = true; continue;
		} else
		
		if(!savenotri && strlen(argv[i]) == 14 && !strncmp(argv[i],"-keeptriangles", 14)) {
			minput[KTRI] = true; continue;
		} else
		
		if(strlen(argv[i]) == 2 && !strncmp(argv[i],"-f", 2)) {
			load(argv[++i]); continue;
		} else
		
		if(strlen(argv[i]) == 2 && !strncmp(argv[i],"-i", 2)) {
			imgflnm = argv[++i]; continue;
		} else
		
		if(strlen(argv[i]) == 10 && !strncmp(argv[i],"-savenotri", 10)) {
			savenotri = true; continue;
		} else
		
		if(strlen(argv[i]) == 8 && !strncmp(argv[i],"-objprps", 8)) {
			if(i+1<argc && argv[i+1][0] == '{')
				objprps = POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(std::string(argv[++i]));
			else {
				std::string s = "{";
				for(int p = 0; p < 8; p++) s += std::string(argv[++i]) + (p!=7?",":"}");
				objprps = POGEL::PHYSICS::SOLIDPHYSICALPROPERTIES(s);
			}
			minput[PRP] = true; continue;
		} else 
		
		if(strlen(argv[i]) == 5 && !strncmp(argv[i],"-rend", 5)) {
			rendatsave = true;
			rendpath = argv[++i];
			if(i+1 < argc && argv[i+1][0] == '{' && argv[i+1][strlen(argv[i+1])-1] == '}')
				rendcpos = argv[++i];
			continue;
		} else
		
		{ continue; }
	}
	
	// standard cmd options
	for(unsigned int i = 0; i < argc; i++) {
		
		if(!strncmp(argv[i],"-gridspace=%f", 10)) {
			if(minput[FLE]) continue;
			sscanf(argv[i], "-gridspace=%f", &grid_space);
			minput[GRD] = true; continue;
		} else
		
		if(!strncmp(argv[i],"-ballsize=%f", 9)) {
			if(minput[FLE]) continue;
			sscanf(argv[i], "-ballsize=%f", &ball_size);
			minput[SZE] = true; continue;
		} else
		
		if(!strncmp(argv[i],"-loops=%u", 6)) {
			if(usetime) {
				unsigned int h = 0, m = 0, s = 0, nc = 0;
				for(int c = 0; c < strlen(argv[i]); c++) if(argv[i][c] == ':') nc++;
				switch(nc) {
					default:
					case 0: sscanf(argv[i], "-loops=%u", &s); break;
					case 1: sscanf(argv[i], "-loops=%u:%u", &m, &s); break;
					case 2: sscanf(argv[i], "-loops=%u:%u:%u", &h, &m, &s); break;
				}
				while(s>60) { m+=1; s-=60; }
				while(m>60) { h+=1; m-=60; }
				loopnum = s + m*60 + h*3600;
			}
			else sscanf(argv[i], "-loops=%u", &loopnum);
			minput[LOOP] = true; continue;
		} else
		
		if(!strncmp(argv[i],"-objnum=%u", 7)) {
			sscanf(argv[i], "-objnum=%u", &objnum);
			minput[NUM] = true; continue;
		} else
		
		if(!strncmp(argv[i],"-savefrq=%u", 8)) {
			sscanf(argv[i], "-savefrq=%u", &savefreq);
			minput[FRQ] = true; continue;
		} else 
		
		if(!strncmp(argv[i],"-massvar=%f", 8)) {
			sscanf(argv[i], "-massvar=%f", &massvar);
			continue;
		} else
		
		if(!strncmp(argv[i],"-randpos=%f", 8)) {
			sscanf(argv[i], "-randpos=%f", &randpos);
			minput[RPOS] = true; continue;
		} else
		
		if(!strncmp(argv[i],"-2d", 3)) {
			minput[L2D] = true; continue;
		} else
		
		{ continue; }
	}
	
	if(!minput[LOOP]) { cout << "number of times to loop: "; cin >> loopnum; }
	if(!minput[NUM ]) { cout << "number of objects(will be " << (minput[L2D]? "squared" : "cubed") << "): "; cin >> objnum; }
	if(!minput[FRQ ]) { cout << "number of itterations between saves: "; cin >> savefreq; }
	
	sim.boundingskips = 0;
    addproperty(POGEL_DEBUG);
	srand((unsigned)time(NULL));
	
	if(!minput[3])
		generate();
	printf("Rebuilding Data Lists ...\n");
	unsigned int thnum = 1;
	for(unsigned int i = 0; i < argc; i++)
		if(!strncmp(argv[i],"-threads=%u", 8)) {
			sscanf(argv[i], "-threads=%u", &thnum);
			break;
		}
	if(thnum > 1)
		sim.setThreadsNum(thnum);
	else
		sim.FORCEfastAccessList();
	
	POGEL::InitFps();
};

void loop() {
	printf("\nStarting...\n");
	unsigned int i = 0;
	while( (usetime ? (unsigned int)POGEL::GetTimePassed() : i ) < loopnum ) {
		POGEL::PrintFps();
		POGEL::IncrementFps();
		POGEL::message(" Calculating ...\n");
		sim.increment();
		if( savefreq && i%savefreq == 0 ) {
			printf("\tSaving at itteration: %u ...\n", i);
			save();
			printf("\t\tDone.\n");
			if(rendatsave) {
				printf("\tRendering ...\n");
				char* frm = POGEL::string("%u", POGEL::frames);
				std::string fl = rendpath + "out." + flnm + "." + std::string(frm) + ".bmp"; free(frm);
				std::string command = "./viewer -label -f "+flnm+" -o "+fl+" -campos " + rendcpos;
				system(command.c_str());
				printf("\t\tDone.\n");
			}
		}
		i++;
	}
};

void end() {
	printf("\n\tSaving at completion ...");
	save();
	printf(" Done\n");
	printf("\n\tRuntime: %u:%02u:%05.2f\n", 
		(unsigned int)POGEL::GetTimePassed()/3600, 
		((unsigned int)POGEL::GetTimePassed()/60)%60, 
		fmod(POGEL::GetTimePassed(),60)
	);
	cout << endl;
};

int main (int argc, char const* argv[])
{
	init(argc,(char**)argv);
	loop();
	end();
	return 0;
}
