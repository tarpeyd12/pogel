//#include <GL/glut.h>

#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "pogel.h"
#include "pogel_internals.h"

// pogel global variables
namespace POGEL {
	char *logfilefilename;
	unsigned int properties;
	
	float start, finish, start_long, finish_long;
	unsigned long frames;
	float fps, fps_long;
	
	clock_t inittime, curtime;
	float duration;
	unsigned long inittmpst;
	
	float framerate_throtling_correction = 1.0f;
}

void POGEL::getcmd(int argc, char **argv) {
	if(argc>1) {
		for(int i=1;i<argc;i++) {
			if(argv[i]!=NULL) {
				if(!true)
					{}
				
				#ifdef POGEL_OOPS
				else if(strcmp(argv[i], POGEL_CMD_BOOL_FORMAT_OOPS)==0)
					POGEL::addproperty(POGEL_OOPS);
				#endif
				
				#ifdef POGEL_DEBUG
				else if(strcmp(argv[i], POGEL_CMD_BOOL_FORMAT_DEBUG)==0)
					POGEL::addproperty(POGEL_DEBUG);
				#endif
				
				#ifdef POGEL_WIREFRAME
				else if(strcmp(argv[i], POGEL_CMD_BOOL_FORMAT_WIREFRAME)==0)
					POGEL::addproperty(POGEL_WIREFRAME);
				#endif
				
				#ifdef POGEL_BOUNDING
				else if(strcmp(argv[i], POGEL_CMD_BOOL_FORMAT_BOUNDING)==0)
					POGEL::addproperty(POGEL_BOUNDING);
				#endif
				
				#ifdef POGEL_TRAILS
				else if(strcmp(argv[i], POGEL_CMD_BOOL_FORMAT_TRAILS)==0)
					POGEL::addproperty(POGEL_TRAILS);
				#endif
				
				#ifdef POGEL_COLLISIONS
				else if(strcmp(argv[i], POGEL_CMD_BOOL_FORMAT_COLLISIONS)==0)
					POGEL::addproperty(POGEL_COLLISIONS);
				#endif
				
				#ifdef POGEL_ANCESTORY
				else if(strcmp(argv[i], POGEL_CMD_BOOL_FORMAT_ANCESTORY)==0)
					POGEL::addproperty(POGEL_ANCESTORY);
				#endif
				
				#ifdef POGEL_PAIRS
				else if(strcmp(argv[i], POGEL_CMD_BOOL_FORMAT_PAIRS)==0)
					POGEL::addproperty(POGEL_PAIRS);
				#endif
				
				#ifdef POGEL_LABEL
				else if(strcmp(argv[i], POGEL_CMD_BOOL_FORMAT_LABEL)==0)
					POGEL::addproperty(POGEL_LABEL);
				#endif
				
				#ifdef POGEL_LOGTOFILE
				else if(strcmp(argv[i], POGEL_CMD_BOOL_FORMAT_LOGTOFILE)==0) {
					if((i+1)>=argc || argv[i+1]==NULL) {
						printf("\n\n\t***\tFATALITY: %s\t***\n\n",POGEL_FATALITY_BAD_FILE_STRING);
						exit(POGEL_FATALITY_BAD_FILE_RETNUM);
					}
					POGEL::logfilefilename=argv[i+1];
					FILE* file = fopen(POGEL::logfilefilename, "w");
					if(file==NULL) {
						printf("\n\n\t***\tFATALITY: %s: \"%s\"\t***\n\n",POGEL_FATALITY_BAD_FILE_STRING,POGEL::logfilefilename);
						exit(POGEL_FATALITY_BAD_FILE_RETNUM);
					}
					time_t tm = time(NULL);
					fprintf(file, "POGEL debug file.\nPOGEL %s\nstarted at: %s",POGEL_VERSION, ctime(&tm) );
					fclose(file);
					POGEL::addproperty(POGEL_LOGTOFILE);
					i+=1;
				}
				#endif
				
				else
					{}
			}
		}
	}
};

unsigned int POGEL::getproperties() {return POGEL::properties;}
void POGEL::setproperties(unsigned int prop) {POGEL::properties=prop;}
void POGEL::addproperty(unsigned int prop) {POGEL::properties|=prop;}
void POGEL::removeproperty(unsigned int prop) {POGEL::properties^=prop;}
bool POGEL::hasproperty(unsigned int prop) {return (POGEL::properties & prop);}

char* POGEL::string(const char* fmt, ...) {
	char* tmpstr=(char*)malloc(sizeof(char)*1024*2); // create a temporary string of suffient size to handle this string
	char* output; // an empty string for later
	memset(tmpstr, '\0', 1024*2); // set the temporary string to all numm, as to avoid artifacts
	va_list args; // the argument list
	va_start(args, fmt); // initialize the argument list
	vsprintf(tmpstr, fmt, args); // take the format, and argument list and put it into the temporary string
	va_end(args);
	output=(char*)malloc(sizeof(char)*(strlen(tmpstr)+1));
	memcpy(output, tmpstr, strlen(tmpstr)+1);
	free(tmpstr);
	return output;
};

void POGEL::fatality(int ret,const char* fmt, ...) {
	char* tmpstr=(char*)malloc(sizeof(char)*1024*2); // create a temporary string of suffient size to handle this string
	char* output; // an empty string for later
	memset(tmpstr, '\0', 1024*2); // set the temporary string to all numm, as to avoid artifacts
	va_list args; // the argument list
	va_start(args, fmt); // initialize the argument list
	vsprintf(tmpstr, fmt, args); // take the format, and argument list and put it into the temporary string
	va_end(args);
	output=(char*)malloc(sizeof(char)*(strlen(tmpstr)+1));
	memcpy(output, tmpstr, strlen(tmpstr)+1);
	free(tmpstr);
	//if(POGEL::hasproperty(POGEL_DEBUG))
		printf("\n\n\t***\tFATALITY: %s\t***\n\n",output);
	if(POGEL::hasproperty(POGEL_LOGTOFILE))
		POGEL::logtofile("\n\n\t***\tFATALITY: %s\t***\n\n",output);
	free(output);
	exit(ret);
};

void POGEL::error(const char* fmt, ...) {
	char* tmpstr=(char*)malloc(sizeof(char)*1024*2); // create a temporary string of suffient size to handle this string
	char* output; // an empty string for later
	memset(tmpstr, '\0', 1024*2); // set the temporary string to all numm, as to avoid artifacts
	va_list args; // the argument list
	va_start(args, fmt); // initialize the argument list
	vsprintf(tmpstr, fmt, args); // take the format, and argument list and put it into the temporary string
	va_end(args);
	output=(char*)malloc(sizeof(char)*(strlen(tmpstr)+1));
	memcpy(output, tmpstr, strlen(tmpstr)+1);
	free(tmpstr);
	if(POGEL::hasproperty(POGEL_DEBUG))
		printf("\n\tError: %s",output);
	if(POGEL::hasproperty(POGEL_LOGTOFILE))
		POGEL::logtofile("Error: %s",output);
	free(output);
};

void POGEL::warning(const char* fmt, ...) {
	char* tmpstr=(char*)malloc(sizeof(char)*1024*2); // create a temporary string of suffient size to handle this string
	char* output; // an empty string for later
	memset(tmpstr, '\0', 1024*2); // set the temporary string to all numm, as to avoid artifacts
	va_list args; // the argument list
	va_start(args, fmt); // initialize the argument list
	vsprintf(tmpstr, fmt, args); // take the format, and argument list and put it into the temporary string
	va_end(args);
	output=(char*)malloc(sizeof(char)*(strlen(tmpstr)+1));
	memcpy(output, tmpstr, strlen(tmpstr)+1);
	free(tmpstr);
	if(POGEL::hasproperty(POGEL_DEBUG))
		printf("\n\tWarning: %s",output);
	if(POGEL::hasproperty(POGEL_LOGTOFILE))
		POGEL::logtofile("Warning: %s",output);
	
	free(output);
};

void POGEL::message(const char* fmt, ...) {
	char* tmpstr=(char*)malloc(sizeof(char)*1024*2); // create a temporary string of suffient size to handle this string
	char* output; // an empty string for later
	memset(tmpstr, '\0', 1024*2); // set the temporary string to all numm, as to avoid artifacts
	va_list args; // the argument list
	va_start(args, fmt); // initialize the argument list
	vsprintf(tmpstr, fmt, args); // take the format, and argument list and put it into the temporary string
	va_end(args);
	output=(char*)malloc(sizeof(char)*(strlen(tmpstr)+1));
	memcpy(output, tmpstr, strlen(tmpstr)+1);
	free(tmpstr);
	if(POGEL::hasproperty(POGEL_DEBUG))
		printf("%s",output);
	if(POGEL::hasproperty(POGEL_LOGTOFILE))
		POGEL::logtofile("%s",output);
	
	free(output);
};

void POGEL::logtofile(const char* fmt, ...) {
	if(POGEL::hasproperty(POGEL_LOGTOFILE)) {
		FILE *file = fopen(logfilefilename, "a");
		char* tmpstr=(char*)malloc(sizeof(char)*1024*2); // create a temporary string of suffient size to handle this string
		char* output; // an empty string for later
		memset(tmpstr, '\0', 1024*2); // set the temporary string to all numm, as to avoid artifacts
		va_list args; // the argument list
		va_start(args, fmt); // initialize the argument list
		vsprintf(tmpstr, fmt, args); // take the format, and argument list and put it into the temporary string
		va_end(args);
		output=(char*)malloc(sizeof(char)*(strlen(tmpstr)+1));
		memcpy(output, tmpstr, strlen(tmpstr)+1);
		free(tmpstr);
		time_t tm = time(NULL);
		if(strlen(output)==1 && output[0]=='\n') {
			fprintf(file,"\n");
		}
		else {
			char tmp[32];
			strcpy(tmp,ctime(&tm));
			for(unsigned int i=0;i<strlen(tmp);i++) if(tmp[i]=='\n') tmp[i]='\0';
			for(unsigned int i=0;i<strlen(output);i++)
				if(output[i]=='\r') output[i]=' ';
			fprintf(file,"\n%s:\t%s", tmp, output);
		}
		//free(tmp);
		//printf("log_1\n");
		free(output);
		fclose(file);
	}
};

//Returns a random number in the range (0.0f, 1.0f).  Every
//bit of the mantissa is randomized.
float rnd() {
  //Generate a random number in the range [0.5f, 1.0f).
  unsigned int ret = 0x3F000000 | (0x7FFFFF & ((rand() << 8) ^ rand()));
  unsigned short coinFlips;

  //If the coin is tails, return the number, otherwise
  //divide the random number by two by decrementing the
  //exponent and keep going. The exponent starts at 63.
  //Each loop represents 15 random bits, a.k.a. 'coin flips'.
  #define RND_INNER_LOOP() \
    if( coinFlips & 1 ) break; \
    coinFlips >>= 1; \
    ret -= 0x800000
  for(;;){
    coinFlips = rand();
    RND_INNER_LOOP(); RND_INNER_LOOP(); RND_INNER_LOOP();
    //At this point, the exponent is 60, 45, 30, 15, or 0.
    //If the exponent is 0, then the number equals 0.0f.
    if( ! (ret & 0x3F800000) ) return 0.0f;
    RND_INNER_LOOP(); RND_INNER_LOOP(); RND_INNER_LOOP();
    RND_INNER_LOOP(); RND_INNER_LOOP(); RND_INNER_LOOP();
    RND_INNER_LOOP(); RND_INNER_LOOP(); RND_INNER_LOOP();
    RND_INNER_LOOP(); RND_INNER_LOOP(); RND_INNER_LOOP();
  }
  float returned_value;
  float * tmpval_1 = (float *)&ret;
  returned_value = *tmpval_1;
  return returned_value;
}

float POGEL::FloatRand(float MaxVal) {return /*float(rand()%100)/((100/MaxVal)+0.1);*/MaxVal*rnd();}

bool POGEL::about(float a, float b, float pres) {
	float c = (a+b)/2.0f;
	pres /= 2.0f;
	return (a<=c+pres && a>=c-pres) && (b<=c+pres && b>=c-pres);
};

void POGEL::InitFps() {
	POGEL::start =  POGEL::start_long = 0.0f;
	POGEL::inittime = POGEL::curtime = clock();
	POGEL::inittmpst = 0;
	POGEL::duration = 0.0f;
};

void POGEL::IncrementFps() {
	float cur = POGEL::GetTimePassed();
	POGEL::finish = cur;
	if(frames%POGEL_FPS_UPDATE_AVERAGE == 0) {
		POGEL::finish_long = cur;
		POGEL::fps_long = 1.0/fabs(POGEL::finish_long - POGEL::start_long);
		POGEL::start_long = cur;
	}
	POGEL::frames++;
	POGEL::fps = 1.0/fabs(POGEL::finish - POGEL::start);
	//POGEL::fps += POGEL::fps_long;
	//POGEL::fps /= 2.0f;
	POGEL::start = cur;
};

float POGEL::GetFps() {
	return (POGEL::fps+POGEL::fps_long)/2.0;
};

void POGEL::PrintFps() {
	/*char buffer[32];
	sprintf(buffer, "Fps: %14.10f", POGEL::GetFps());
	int Nb = strlen(buffer);
	glDisable(GL_LIGHTING);
	glRasterPos2i(0,0);
	for (int i = 0; i < Nb; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)buffer[i]);
	glEnable(GL_LIGHTING);*/
	float timepassed = POGEL::GetTimePassed();
	POGEL::message("\rFrame: %u, Fps: %6.2f, Spf: %6.2f, Duration = %0.2fs (%d:%02d:%05.2f)",
		POGEL::frames,
		(POGEL::GetFps()>999.99?999.99:POGEL::GetFps()), POGEL::GetSecondsPerFrame(),
		timepassed,
		(unsigned int)timepassed/3600, ((unsigned int)timepassed/60)%60, fmod(timepassed,60)
	);
};

void POGEL::SetFramerateThrotle(float framerate) {
	POGEL::framerate_throtling_correction = framerate;
};

float POGEL::GetSecondsPerFrame() {
		return 1/POGEL::GetFps();
};

float POGEL::GetTimePassed() {
	clock_t c = clock();
	float t = float(c-POGEL::curtime)/float(CLOCKS_PER_SEC);
	if(t >= 1000.0f) { POGEL::curtime = c; POGEL::duration += t; t = 0.0f;}
	return t + duration;
	//return duration;
};

void POGEL::ThrotleFps(int desitredFramerate) {
	float dfr = (float)desitredFramerate - (POGEL::fps*2-POGEL::fps_long);
	if(dfr < 0.0f)
		dfr = 1.0f/dfr;
	POGEL::SetFramerateThrotle(1.0/fabs(dfr));
};

void POGEL::UnthrotleFps() {
	POGEL::SetFramerateThrotle(1.0f);
};

unsigned int POGEL::getOccurrencesInString(char c, std::string s) {
	unsigned int count = 0;
	for(unsigned int i = 0; i < s.length(); i++) if(s[i] == c) count++;
	return count;
};

// POGEL::getStringSection('[', 2, false, '>', 1, true, "{[5],[__2<33>]}");
//    will return "__2<33>"
std::string POGEL::getStringSection(char firstChar, unsigned int startingOccurrence, bool firstInclusive, char lastChar, unsigned int endingOccurrence, bool lastInclusive, std::string s) {
	if( firstChar == lastChar || s.empty()) return "";
	char chars[2]; chars[0]=firstChar; chars[1]=lastChar;
	unsigned int pos[2], occs[2], p[2], c;
	occs[0] = occs[1] = pos[0] = pos[1] = c = 0;
	p[0] = startingOccurrence; p[1] = endingOccurrence;
	for(unsigned int i = 0; i < s.length(); i++) if(s[i] == chars[c] && ++occs[c] == p[c]) {
		pos[c] = i+(c==0?(firstInclusive?0:1):(lastInclusive?1:0));
		if(++c>=2) break;
	}
	if(pos[0] >= pos[1]) return "";
	return s.substr(pos[0], pos[1]-pos[0]);
	
};

// POGEL::getStringComponentLevel( '{', false, '}', false, "{{} {} {{} {{{} {hello}} {} {}}} {}}", "0 2 1 0 1"); 
//    will return "hello"
std::string POGEL::getStringComponentLevel(char startChar, bool firstInclusive, char endChar, bool lastInclusive, std::string s, std::string path_s) {
	if( startChar == endChar || s.empty() || path_s.empty()) return "";
	std::stringstream path(std::stringstream::in | std::stringstream::out); path << path_s;
	unsigned int numEndChars = POGEL::getOccurrencesInString(endChar, s);
	if(numEndChars == 0) return "";
	unsigned int level = 0, levelocc = 0, branchpos = 0, branch; path >> branch;
	for(unsigned int i = 0; i < s.length(); i++) {
		if(s[i] == startChar && level++ == branchpos && levelocc++ >= branch) {
			branchpos++; levelocc = 0;
			if(path.good()) path >> branch;
			else for(unsigned int p = i; i < s.length(); i++)
				if(s[i] == startChar) level++;
				else if(s[i] == endChar && level-- == branchpos+1) return s.substr(p+(firstInclusive?0:1), i-p+(lastInclusive?1:0));
		}
		if(s[i] == endChar) { level--; if(--numEndChars <= 0) return ""; }
	}
	return "";
};

// POGEL::getStringComponentLevel( '{', '}', "{{} {} {{} {{{} {hello}} {} {}}} {}}", "0 2 1 0 1"); 
//    will return "{hello}"
std::string POGEL::getStringComponentLevel(char startChar, char endChar, std::string s, std::string path_s) {
	return POGEL::getStringComponentLevel(startChar, true, endChar, true, s, path_s);
};

