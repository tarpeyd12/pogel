#ifndef _POGEL_H
#define _POGEL_H

#include <time.h>

#include <iostream>
#include <sstream>
#include <string>

// the version number
#define POGEL_VERSION "version 0.0.0-01"

#define POGEL_FPS_UPDATE_AVERAGE 1

namespace POGEL {
	// pogel global variables
	extern char *logfilefilename;
	extern unsigned int properties;
	
	extern float start, finish, start_long, finish_long;
	extern unsigned long frames;
	extern float duration, fps, fps_long;
	
	extern float framerate_throtling_correction;
	
	// pogel global functions
	void getcmd(int,char**);
	
	unsigned int getproperties();
	void setproperties(unsigned int prop);
	void addproperty(unsigned int prop);
	void removeproperty(unsigned int prop);
	bool hasproperty(unsigned int prop);
	
	char* string(const char* fmt, ...);
	
	void fatality(int,const char* fmt, ...);
	void error(const char* fmt, ...);
	void warning(const char* fmt, ...);
	void message(const char* fmt, ...);
	void logtofile(const char* fmt, ...);
	
	float FloatRand(float);
	
	bool about(float,float,float);
	
	void InitFps();
	void IncrementFps();
	float GetFps();
	void PrintFps();
	
	void SetFramerateThrotle(float);
	float GetSecondsPerFrame();
	
	float GetTimePassed();
	
	void ThrotleFps(int);
	void UnthrotleFps();
	
	unsigned int getOccurrencesInString(char, std::string);
	std::string getStringSection(char, unsigned int, bool, char, unsigned int, bool, std::string);
	std::string getStringComponentLevel(char, bool, char, bool, std::string, std::string);
	std::string getStringComponentLevel(char, char, std::string, std::string);
	
	// pogel classes
	class POINT;
	class VECTOR;
	class VERTEX;
	
	class BOUNDING;
	
	class COLOR;
	class IMAGE;
	class VIEW;
	
	class QUAT;
	class MATRIX;
	
	class LINE;
	
	class TRIANGLE;
	class OBJECT;
	
	class FRACTAL;
	class SPRITE;
}

#include "pogel_classes.h"

#endif /* _POGEL_H */
