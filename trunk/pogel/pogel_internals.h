#ifndef _POGEL_INTERNALS_H
#define _POGEL_INTERNALS_H

#define		PROPERTIES_METHODS		unsigned int getproperties() {return properties;} \
									void setproperties(unsigned int prop) {properties=prop;} \
									void addproperty(unsigned int prop) {properties|=prop;} \
									void removeproperty(unsigned int prop) {properties^=prop;} \
									bool hasproperty(unsigned int prop) {return (properties & prop);}

// the error return types
#define				POGEL_FATALITY_NULL_OBJECT_POINTER_RETNUM		1
#define				POGEL_FATALITY_NULL_OBJECT_POINTER_STRING		"Null Object Pointer"

#define				POGEL_FATALITY_NULL_ARRAY_POINTER_RETNUM		2
#define				POGEL_FATALITY_NULL_ARRAY_POINTER_STRING		"Null Array Pointer"

#define				POGEL_FATALITY_NULL_LIST_POINTER_RETNUM			4
#define				POGEL_FATALITY_NULL_LIST_POINTER_STRING			"Null Pointer in Refrence Array"

#define				POGEL_FATALITY_CIRCULAR_HIERARCHY_RETNUM		8
#define				POGEL_FATALITY_CIRCULAR_HIERARCHY_STRING		"Circular Linked List Hierarchy"

#define				POGEL_FATALITY_BAD_FILE_RETNUM					16
#define				POGEL_FATALITY_BAD_FILE_STRING					"Not a valid file"

// the pogel cmd option formats
#define				POGEL_CMD_BOOL_FORMAT_OOPS						"-oops"

#define				POGEL_CMD_BOOL_FORMAT_DEBUG						"-debug"
#define				POGEL_CMD_BOOL_FORMAT_WIREFRAME					"-wireframe"
#define				POGEL_CMD_BOOL_FORMAT_LOGTOFILE					"-logfile"
#define				POGEL_CMD_BOOL_FORMAT_BOUNDING					"-boundingbox"
#define				POGEL_CMD_BOOL_FORMAT_TRAILS					"-trails"
#define				POGEL_CMD_BOOL_FORMAT_COLLISIONS				"-collisions"
#define				POGEL_CMD_BOOL_FORMAT_ANCESTORY					"-ancestory"

// mashable pogel global properties
#define				POGEL_OOPS										( ( 1|2|4|8|16|32|64 ) ^ POGEL_LOGTOFILE )

#define				POGEL_LOGTOFILE									1
#define				POGEL_DEBUG										2
#define				POGEL_WIREFRAME									4
#define				POGEL_BOUNDING									8
#define				POGEL_TRAILS									16
#define				POGEL_COLLISIONS								32
#define				POGEL_ANCESTORY									64


#ifndef _POGEL_H
namespace POGEL {
	extern char *logfilefilename;
	extern unsigned int properties;
	extern unsigned long frames;
	
	extern unsigned int getproperties();
	extern void setproperties(unsigned int prop);
	extern void addproperty(unsigned int prop);
	extern void removeproperty(unsigned int prop);
	extern bool hasproperty(unsigned int prop);
	
	extern char* string(const char* fmt, ...);
	extern void fatality(int,const char* fmt, ...);
	extern void error(const char* fmt, ...);
	extern void warning(const char* fmt, ...);
	extern void message(const char* fmt, ...);
	extern void logtofile(const char* fmt, ...);
	
	extern float FloatRand(float);
	
	extern bool about(float,float,float);
	
	extern void InitFps();
	extern void IncrementFps();
	extern float GetFps();
	extern void PrintFps();
}
#endif

#endif /* _POGEL_INTERNALS_H */
